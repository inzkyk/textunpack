#include "textunpack.hpp"

#include <ix_Buffer.hpp>
#include <ix_CmdArgsEater.hpp>
#include <ix_Mutex.hpp>
#include <ix_StringView.hpp>
#include <ix_SystemManager.hpp>
#include <ix_ThreadPool.hpp>
#include <ix_UniquePointer.hpp>
#include <ix_Writer.hpp>
#include <ix_assert.hpp>
#include <ix_atomic.hpp>
#include <ix_defer.hpp>
#include <ix_environment.hpp>
#include <ix_file.hpp>
#include <ix_filepath.hpp>
#include <ix_scanf.hpp>

#if ix_DO_TEST
#include <ix_test.hpp>
#endif

static constexpr const char *HELP_TEXT = R"(textunpck version 1.0.0

USAGE:
  textunpack --help
  textunpack [OPTIONS] [--unpack] ARCHIVE_FILE
  textunpack [OPTIONS] [--unpack] --stdin BASE_DIRECTORY
  textunpack --pack FILE...

OPTIONS:
  --list, -l                List the paths of entries in the archive.
  --stdin, -s               Read from stdin instead of FILE.
  --dry-run, -d             Process the archive file normally, but do not unpack them.
  --disable-warning, -W     Disable warning.
  --num-threads <N>, -n <N>  Secify the number of threads to use.

)";

static constexpr const char *ERROR_TEXT_INVALID_INVOCATION = "Invalid invocation: no cmd args provided!\n";
static constexpr const char *ERROR_TEXT_STDIN_LOAD_FAILED = "Failed to read from stdin.\n";
static constexpr const char *ERROR_TEXT_FILE_OPEN_FAILED = "Failed to open %s: %s\n";
static constexpr const char *ERROR_TEXT_FILE_READ_FAILED = "File load failed: %s\n";
static constexpr const char *ERROR_TEXT_NO_FILE = "No input file.\n";
static constexpr const char *ERROR_TEXT_TOO_MANY_FILES = "Too many input files. You must specify one input file.\n";
static constexpr const char *ERROR_TEXT_NO_DIRECTORY = "No base directory. You must specify one base directory.\n";
static constexpr const char *ERROR_TEXT_TOO_MANY_DIRECTORY =
    "Too many base directories. You must specify one base directory.\n";
static constexpr const char *ERROR_TEXT_INVALID_NUM_THREADS = "Invalid number of threads: %s\n";
static constexpr const char *ERROR_TEXT_NOT_A_NUMBER = "Not a number: %s\n";

struct G
{
    const ix_FileHandle *stdout_handle;
    const ix_FileHandle *stderr_handle;
    ix_Mutex stderr_handle_mutex;
    ix_CmdArgsEater *args;
    const char *input;
    const char *input_end;
    const char *input_filename;
    size_t input_filename_length;
    size_t input_dirname_length;
    const char *first_entry_path_start;

    int num_failed_entries;
    ix_Vector<ix_Writer> file_writers;
};

static int textunpack_unpack(G &g, size_t num_threads)
{
    ix_ThreadPoolOption thread_pool_option;
    thread_pool_option.num_worker_threads = num_threads;
    thread_pool_option.initial_job_queue_capacity = 128;
    thread_pool_option.job_pool_size = 128;
    thread_pool_option.spwan_workes_lazily = true;
    ix_ThreadPool pool(thread_pool_option);
    ix_Job *root = pool.add_empty_job();

    const size_t writer_buffer_size = 32 * 1024;
    const size_t writer_buffer_total_size = thread_pool_option.num_worker_threads * writer_buffer_size;
    ix_UniquePointer<char[]> writer_buffer = ix_make_unique_array<char>(writer_buffer_total_size);

    for (size_t i = 0; i < thread_pool_option.num_worker_threads; i++)
    {
        char *p = writer_buffer.get() + (i * writer_buffer_size);
        g.file_writers.emplace_back(ix_Writer::from_existing_region(p, writer_buffer_size));
    }

    g.num_failed_entries = 0;
    const char *entry_path_start = g.first_entry_path_start;
    while (entry_path_start != nullptr)
    {
        pool.add_job(root, [&g, entry_path_start]() {
            const size_t worker_index = ix_ThreadPool::get_current_worker()->index();
            char buf[ix_MAX_PATH + 1];
            ix_memcpy(buf, g.input_filename, g.input_dirname_length);
            ix_Writer &file_writer = g.file_writers[worker_index];
            const size_t entry_path_length = tup_calculate_entry_path_length(entry_path_start);
            if (g.input_dirname_length + entry_path_length > ix_MAX_PATH)
            {
                ix_atomic_fetch_add(&g.num_failed_entries, 1, ix_MEMORY_ORDER_SEQ_CST);
                g.stderr_handle_mutex.lock();
                g.stderr_handle->write_stringf(
                    "Path too long: %.*s%.*s\n",
                    static_cast<int>(g.input_dirname_length),
                    g.input_filename,
                    static_cast<int>(entry_path_length),
                    entry_path_start
                );
                g.stderr_handle_mutex.unlock();
                return;
            }
            ix_memcpy(buf + g.input_dirname_length, entry_path_start, entry_path_length);
            buf[g.input_dirname_length + entry_path_length] = '\0';
            const ix_Option<ix_FileHandle> maybe_file = ix_create_directories_and_file(buf);
            if (maybe_file.is_error())
            {
                ix_atomic_fetch_add(&g.num_failed_entries, 1, ix_MEMORY_ORDER_SEQ_CST);
                g.stderr_handle_mutex.lock();
                g.stderr_handle->write_stringf("Failed to open: %s\n", buf);
                g.stderr_handle_mutex.unlock();
                return;
            }
            file_writer.substitute(&maybe_file.unwrap());
            tup_unpack_entry(
                ix_StringView(entry_path_start, entry_path_length),
                g.input_end,
                &file_writer,
                g.stderr_handle
            );
            file_writer.flush();
        });

        entry_path_start = tup_find_next_entry_path_start(entry_path_start);
    }

    pool.wait(root);

    return g.num_failed_entries;
}

static int textunpack_dry_run(G &g)
{
    const char *entry_path_start = g.first_entry_path_start;
    while (entry_path_start != nullptr)
    {
        const ix_StringView entry_path(entry_path_start, tup_calculate_entry_path_length(entry_path_start));
        tup_unpack_entry(entry_path, g.input_end, nullptr, g.stderr_handle);
        entry_path_start = tup_find_next_entry_path_start(entry_path_start);
    }
    return 0;
}

static int textunpack_list_entries(G &g)
{
    ix_Writer stdout_writer(4096, g.stdout_handle);

    const char *entry_path_start = g.first_entry_path_start;
    while (entry_path_start != nullptr)
    {
        const size_t entry_path_length = tup_calculate_entry_path_length(entry_path_start);
        stdout_writer.write_stringf(
            "%.*s%.*s\n",
            static_cast<int>(g.input_dirname_length),
            g.input_filename,
            static_cast<int>(entry_path_length),
            entry_path_start
        );
        const ix_StringView entry_path(entry_path_start, entry_path_length);
        tup_unpack_entry(entry_path, g.input_end, nullptr, g.stderr_handle);
        entry_path_start = tup_find_next_entry_path_start(entry_path_start);
    }

    return 0;
}

static int textunpack_pack(G &g, const char *output_filename, const char *const *filenames, size_t num_files)
{
    ix_Buffer buffer(4096);
    for (size_t i = 0; i < num_files; i++)
    {
        const char *filename = filenames[i];
        buffer.push_stringf("#+EXPORT_BEGIN %s\n", filename);
        ix_Option<ix_FileHandle> file_opt = ix_open_file(filename, ix_READ_ONLY);
        if (file_opt.is_error())
        {
            g.stderr_handle
                ->write_stringf(ERROR_TEXT_FILE_OPEN_FAILED, filename, file_opt.result_description().data());
            return 1;
        }
        ix_Option<size_t> length_opt = buffer.load_file_handle(file_opt.unwrap());
        if (length_opt.is_error())
        {
            g.stderr_handle->write_stringf(ERROR_TEXT_FILE_READ_FAILED, filename);
            return 1;
        }
        buffer.push_stringf("\n#+EXPORT_END %s\n", filename);
    }

    ix_FileHandle output_file;
    if (output_filename == nullptr)
    {
        g.stdout_handle->write(buffer.data(), buffer.size());
    }
    else
    {
        const ix_Option<ix_FileHandle> output_file_opt = ix_open_file(output_filename, ix_WRITE_ONLY);
        if (output_file_opt.is_error())
        {
            g.stderr_handle->write_stringf(
                ERROR_TEXT_FILE_OPEN_FAILED,
                output_filename,
                output_file_opt.result_description().data()
            );
            return 1;
        }
        output_file_opt.unwrap().write(buffer.data(), buffer.size());
    }

    return 0;
}

static int textunpack_main(
    const ix_FileHandle &stdin_handle,
    const ix_FileHandle &stdout_handle,
    const ix_FileHandle &stderr_handle,
    ix_CmdArgsEater &&args
)
{
    if (args.size() == 0)
    {
        stderr_handle.write_string(ERROR_TEXT_INVALID_INVOCATION);
        return 1;
    }

#if ix_DO_TEST
    const bool do_test = args.eat_boolean("--test");
    if (do_test)
    {
        return ix_do_test(args.argc(), args.argv());
    }
#endif

    const bool show_help = args.eat_boolean({"--help", "-h"}) || (args.size() == 1);
    if (show_help)
    {
        stdout_handle.write_string(HELP_TEXT);
        return 0;
    }

    G g;

    ix_FileHandle null = ix_FileHandle::null();
    g.stdout_handle = &stdout_handle;
    g.stderr_handle = args.eat_boolean({"--disable-warning", "-W"}) ? &null : &stderr_handle;

    const bool pack = args.eat_boolean({"--pack", "p"});

    if (pack)
    {
        const char *output_filename = args.eat_kv({"--output", "-o"});
        if (args.size() == 1)
        {
            stderr_handle.write_string(ERROR_TEXT_NO_FILE);
            return 1;
        }
        return textunpack_pack(g, output_filename, args.argv() + 1, args.size() - 1);
    }

    const bool dry_run = args.eat_boolean({"--dry-run", "-d"});
    const bool read_from_stdin = args.eat_boolean({"--stdin", "-s"});
    const bool list_entries = args.eat_boolean({"--list", "-l"});
    const bool unpack = args.eat_boolean("--unpack") || (!list_entries && !dry_run && !pack); // default

    size_t num_threads = ix_hardware_concurrency();
    if (unpack)
    {
        const char *num_threads_str = args.eat_kv({"--num-threads", "-n"});
        if (num_threads_str != nullptr)
        {
            const ix_Option<uint64_t> result = ix_string_convert<uint64_t>(num_threads_str);
            if (result.is_error())
            {
                stderr_handle.write_stringf(ERROR_TEXT_NOT_A_NUMBER, num_threads_str);
                return 1;
            }
            num_threads = result.unwrap();
        }

        if (1024 < num_threads)
        {
            stderr_handle.write_stringf(ERROR_TEXT_INVALID_NUM_THREADS, num_threads_str);
            return 1;
        }
    }

    if (args.size() == 1)
    {
        stderr_handle.write_string(read_from_stdin ? ERROR_TEXT_NO_DIRECTORY : ERROR_TEXT_NO_FILE);
        return 1;
    }

    if (args.size() >= 3)
    {
        stderr_handle.write_string(read_from_stdin ? ERROR_TEXT_TOO_MANY_DIRECTORY : ERROR_TEXT_TOO_MANY_FILES);
        return 1;
    }

    ix_ASSERT(args.size() == 2);
    g.input_filename = args[1];

    ix_Buffer input_buffer(4096);
    if (read_from_stdin)
    {
        const ix_Option<size_t> maybe_size = input_buffer.load_file_handle(stdin_handle);
        if (maybe_size.is_error())
        {
            stderr_handle.write_string(ERROR_TEXT_STDIN_LOAD_FAILED);
            return 1;
        }
    }
    else
    {
        const ix_Option<ix_FileHandle> maybe_file = ix_open_file(g.input_filename, ix_READ_ONLY);
        if (maybe_file.is_error())
        {
            stderr_handle
                .write_stringf(ERROR_TEXT_FILE_OPEN_FAILED, g.input_filename, maybe_file.result_description().data());
            return 1;
        }

        const ix_Option<size_t> maybe_bytes_read = input_buffer.load_file_handle(maybe_file.unwrap());
        if (maybe_bytes_read.is_error())
        {
            stderr_handle.write_stringf(ERROR_TEXT_FILE_READ_FAILED, g.input_filename);
            return 1;
        }
    }

    input_buffer.push_char('\0');
    g.input = input_buffer.data();
    g.input_end = input_buffer.end();
    g.first_entry_path_start = tup_find_first_entry_path_start(g.input);

    g.input_filename_length = ix_strlen(g.input_filename);
    g.input_dirname_length =
        read_from_stdin ? g.input_filename_length : ix_dirname_length(g.input_filename, g.input_filename_length);

    if (dry_run)
    {
        return textunpack_dry_run(g);
    }

    if (list_entries)
    {
        return textunpack_list_entries(g);
    }

    if (unpack)
    {
        return textunpack_unpack(g, num_threads);
    }

    ix_UNREACHABLE();
}

#if ix_DO_TEST
#include <ix_TempFile.hpp>

ix_TEST_CASE("textunpack: cli")
{
    auto f = [](std::initializer_list<const char *> args,
                const char *input,
                const char *expected_stdout,
                const char *expected_stderr) {
        const ix_TempFileR in(input);
        ix_TempFileW out;
        ix_TempFileW err;
        textunpack_main(in.file_handle(), out.file_handle(), err.file_handle(), ix_CmdArgsEater(args));
        ix_EXPECT_EQSTR(out.data(), expected_stdout);
        ix_EXPECT_EQSTR(err.data(), expected_stderr);
    };

    // Errors
    f({}, nullptr, "", ERROR_TEXT_INVALID_INVOCATION);
    f({"textunpack", "-l"}, nullptr, "", ERROR_TEXT_NO_FILE);
    f({"textunpack", "-s"}, nullptr, "", ERROR_TEXT_NO_DIRECTORY);
    f({"textunpack", "non-existent.txt"}, nullptr, "", "Failed to open non-existent.txt: File not found\n");
    f({"textunpack", "foo.txt", "bar.txt"}, nullptr, "", ERROR_TEXT_TOO_MANY_FILES);
    f({"textunpack", "-l", "foo.txt", "bar.txt"}, nullptr, "", ERROR_TEXT_TOO_MANY_FILES);
    f({"textunpack", "-l", "--stdin", "./foo/", "./bar/"}, nullptr, "", ERROR_TEXT_TOO_MANY_DIRECTORY);
    f({"textunpack", "-l", "bar.txt", "--num-threads", "2"}, nullptr, "", ERROR_TEXT_TOO_MANY_FILES);
    f({"textunpack", "./", "--stdin", "--num-threads", "foo"}, "", "", "Not a number: foo\n");
    f({"textunpack", "./", "--stdin", "-n", "foo"}, "", "", "Not a number: foo\n");
    f({"textunpack", "./", "--stdin", "--num-threads", "100000"}, "", "", "Invalid number of threads: 100000\n");
    f({"textunpack", "./", "--stdin", "-n", "100000"}, "", "", "Invalid number of threads: 100000\n");
    f({"textunpack", "-s", "./"}, nullptr, "", ERROR_TEXT_STDIN_LOAD_FAILED);
    f({"textunpack", "--dry-run"}, "", "", ERROR_TEXT_NO_FILE);

    // Help
    f({"textunpack"}, nullptr, HELP_TEXT, "");
    f({"textunpack", "-h"}, nullptr, HELP_TEXT, "");
    f({"textunpack", "--help"}, nullptr, HELP_TEXT, "");

    // --dry-run
    f({"textunpack", "--dry-run", "--stdin", "./"}, "", "", "");
    f({"textunpack", "--dry-run", "--stdin", "./"}, "foo bar", "", "");
    f({"textunpack", "--dry-run", "--stdin", "./"},
      "#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "#+EXPORT_END foo.txt\n",
      "",
      "");
    ix_EXPECT(!ix_is_file("foo.txt"));
    f({"textunpack", "--dry-run", "--stdin", "./"},
      "#+EXPORT_BEGIN foo.txt\n",
      "",
      "Entry \"foo.txt\" is not terminated properly.\n");
    f({"textunpack", "--dry-run", "--stdin", "--disable-warning", "./"}, "#+EXPORT_BEGIN foo.txt\n", "", "");

    // -d
    f({"textunpack", "-d", "--stdin", "./"}, "", "", "");
    f({"textunpack", "-d", "--stdin", "./"}, "foo bar", "", "");
    f({"textunpack", "-d", "--stdin", "./"},
      "#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "#+EXPORT_END foo.txt\n",
      "",
      "");
    f({"textunpack", "-d", "--stdin", "./"},
      "#+EXPORT_BEGIN foo.txt\n",
      "",
      "Entry \"foo.txt\" is not terminated properly.\n");
    f({"textunpack", "-d", "--stdin", "--disable-warning", "./"}, "#+EXPORT_BEGIN foo.txt\n", "", "");

    // --disable-warning
    f({"textunpack", "--dry-run", "--disable-warning", "--stdin", "./"}, "#+EXPORT_BEGIN foo.txt\n", "", "");

    // -W
    f({"textunpack", "--dry-run", "-W", "--stdin", "./"}, "#+EXPORT_BEGIN foo.txt\n", "", "");

    // --list
    f({"textunpack", "--list", "--stdin", "./foo/"}, "", "", "");
    f({"textunpack", "--list", "--stdin", "./foo/"},
      "#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "#+EXPORT_END bar.txt\n"
      "#+EXPORT_BEGIN foobar.txt\n"
      "#+EXPORT_END foobar.txt\n",
      "./foo/foo.txt\n"
      "./foo/bar.txt\n"
      "./foo/foobar.txt\n",
      "");

    // -l
    f({"textunpack", "-l", "--stdin", "./foo/"}, "", "", "");
    f({"textunpack", "-l", "--stdin", "./foo/"},
      "#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n",
      "./foo/foo.txt\n"
      "./foo/bar.txt\n",
      "Entry \"foo.txt\" is not terminated properly.\n"
      "Entry \"bar.txt\" is not terminated properly.\n");
    f({"textunpack", "-l", "--stdin", "./foo/", "--disable-warning"},
      "#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "#+EXPORT_END bar.txt\n"
      "#+EXPORT_BEGIN foobar.txt\n"
      "#+EXPORT_END foobar.txt\n",
      "./foo/foo.txt\n"
      "./foo/bar.txt\n"
      "./foo/foobar.txt\n",
      "");

    // Main procedure
    // With a filename
    ix_EXPECT(ix_write_string_to_file(
                  "input.txt",
                  "#+EXPORT_BEGIN foo.txt\n"
                  "This is foo.\n"
                  "#+EXPORT_END foo.txt\n"
    )
                  .is_ok());
    f({"textunpack", "input.txt"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("foo.txt"));
    ix_EXPECT_EQSTR(ix_load_file("foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("input.txt").is_ok());

    // With --unpack
    ix_EXPECT(ix_write_string_to_file(
                  "input.txt",
                  "#+EXPORT_BEGIN foo.txt\n"
                  "This is foo.\n"
                  "#+EXPORT_END foo.txt\n"
    )
                  .is_ok());
    f({"textunpack", "input.txt", "--unpack"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("foo.txt"));
    ix_EXPECT_EQSTR(ix_load_file("foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("input.txt").is_ok());

    // --stdin
    f({"textunpack", "./", "--stdin"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("foo.txt"));
    ix_EXPECT_EQSTR(ix_load_file("foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());

    // Archive that has multiple files
    f({"textunpack", "./", "--stdin"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "This is bar.\n"
      "#+EXPORT_END bar.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("foo.txt"));
    ix_EXPECT(ix_is_file("bar.txt"));
    ix_EXPECT_EQSTR(ix_load_file("foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT_EQSTR(ix_load_file("bar.txt").unwrap().get(), "This is bar.");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("bar.txt").is_ok());

    // Archive that needs directory creation
    f({"textunpack", "./foo/", "--stdin"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "This is bar.\n"
      "#+EXPORT_END bar.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("./foo/foo.txt"));
    ix_EXPECT(ix_is_file("./foo/bar.txt"));
    ix_EXPECT_EQSTR(ix_load_file("./foo/foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT_EQSTR(ix_load_file("./foo/bar.txt").unwrap().get(), "This is bar.");
    ix_EXPECT(ix_remove_file("./foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("./foo/bar.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/").is_ok());

    // Archive that needs directory creation, with unnormalized paths.
    f({"textunpack", "./", "--stdin"},
      "#+EXPORT_BEGIN ./foo/foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END ./foo/foo.txt\n"
      "#+EXPORT_BEGIN ./foo/bar.txt\n"
      "This is bar.\n"
      "#+EXPORT_END ./foo/bar.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("./foo/foo.txt"));
    ix_EXPECT(ix_is_file("./foo/bar.txt"));
    ix_EXPECT_EQSTR(ix_load_file("./foo/foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT_EQSTR(ix_load_file("./foo/bar.txt").unwrap().get(), "This is bar.");
    ix_EXPECT(ix_remove_file("./foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("./foo/bar.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/").is_ok());

    // Warning (entry not terminated properly)
    f({"textunpack", "./", "--stdin", "--num-threads", "1"},
      "#+EXPORT_BEGIN ./foo/foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_BEGIN ./foo/bar.txt\n"
      "This is bar.\n",
      "",
      "Entry \"./foo/bar.txt\" is not terminated properly.\n"
      "Entry \"./foo/foo.txt\" is not terminated properly.\n");
    ix_EXPECT(ix_is_file("./foo/foo.txt"));
    ix_EXPECT(ix_is_file("./foo/bar.txt"));
    ix_EXPECT_EQSTR(
        ix_load_file("./foo/foo.txt").unwrap().get(),
        "This is foo.\n"
        "This is bar.\n"
    );
    ix_EXPECT_EQSTR(ix_load_file("./foo/bar.txt").unwrap().get(), "This is bar.\n");
    ix_EXPECT(ix_remove_file("./foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("./foo/bar.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/").is_ok());

    // --num-threads
    f({"textunpack", "./foo/", "--stdin", "--num-threads", "1"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "This is bar.\n"
      "#+EXPORT_END bar.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("./foo/foo.txt"));
    ix_EXPECT(ix_is_file("./foo/bar.txt"));
    ix_EXPECT_EQSTR(ix_load_file("./foo/foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT_EQSTR(ix_load_file("./foo/bar.txt").unwrap().get(), "This is bar.");
    ix_EXPECT(ix_remove_file("./foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("./foo/bar.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/").is_ok());

    // -n
    f({"textunpack", "./foo/", "--stdin", "-n", "1"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "This is bar.\n"
      "#+EXPORT_END bar.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("./foo/foo.txt"));
    ix_EXPECT(ix_is_file("./foo/bar.txt"));
    ix_EXPECT_EQSTR(ix_load_file("./foo/foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT_EQSTR(ix_load_file("./foo/bar.txt").unwrap().get(), "This is bar.");
    ix_EXPECT(ix_remove_file("./foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("./foo/bar.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/").is_ok());

    // Warning (path name too long)
    f({"textunpack", "./", "--stdin"},
      "#+EXPORT_BEGIN ./"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789.txt\n"
      "Path is too long!\n",
      "",
      "Path too long: ././"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789.txt\n");

    // Warning (path name too long) disabled
    f({"textunpack", "./", "--stdin", "--disable-warning"},
      "#+EXPORT_BEGIN ./"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789.txt\n"
      "Path is too long!\n",
      "",
      "");

    // Deep directory creation
    f({"textunpack", "./foo/foo/foo/foo/foo/", "--stdin"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "This is bar.\n"
      "#+EXPORT_END bar.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("./foo/foo/foo/foo/foo/foo.txt"));
    ix_EXPECT(ix_is_file("./foo/foo/foo/foo/foo/bar.txt"));
    ix_EXPECT_EQSTR(ix_load_file("./foo/foo/foo/foo/foo/foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT_EQSTR(ix_load_file("./foo/foo/foo/foo/foo/bar.txt").unwrap().get(), "This is bar.");
    ix_EXPECT(ix_remove_file("./foo/foo/foo/foo/foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("./foo/foo/foo/foo/foo/bar.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/foo/foo/foo/foo/").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/foo/foo/foo/").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/foo/foo/").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/foo/").is_ok());
    ix_EXPECT(ix_remove_directory("./foo/").is_ok());

    // Japanese characters
    f({"textunpack", "./あ/い/う/え/お/", "--stdin"},
      "#+EXPORT_BEGIN ひらがな.txt\n"
      "ひらがなです。\n"
      "#+EXPORT_END ひらがな.txt\n"
      "#+EXPORT_BEGIN カタカナ.txt\n"
      "カタカナです。\n"
      "#+EXPORT_END カタカナ.txt\n"
      "#+EXPORT_BEGIN 漢字.txt\n"
      "漢字です。\n"
      "#+EXPORT_END 漢字.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("./あ/い/う/え/お/ひらがな.txt"));
    ix_EXPECT(ix_is_file("./あ/い/う/え/お/カタカナ.txt"));
    ix_EXPECT(ix_is_file("./あ/い/う/え/お/漢字.txt"));
    ix_EXPECT_EQSTR(ix_load_file("./あ/い/う/え/お/ひらがな.txt").unwrap().get(), "ひらがなです。");
    ix_EXPECT_EQSTR(ix_load_file("./あ/い/う/え/お/カタカナ.txt").unwrap().get(), "カタカナです。");
    ix_EXPECT_EQSTR(ix_load_file("./あ/い/う/え/お/漢字.txt").unwrap().get(), "漢字です。");
    ix_EXPECT(ix_remove_file("./あ/い/う/え/お/ひらがな.txt").is_ok());
    ix_EXPECT(ix_remove_file("./あ/い/う/え/お/カタカナ.txt").is_ok());
    ix_EXPECT(ix_remove_file("./あ/い/う/え/お/漢字.txt").is_ok());
    ix_EXPECT(ix_remove_directory("./あ/い/う/え/お/").is_ok());
    ix_EXPECT(ix_remove_directory("./あ/い/う/え/").is_ok());
    ix_EXPECT(ix_remove_directory("./あ/い/う/").is_ok());
    ix_EXPECT(ix_remove_directory("./あ/い/").is_ok());
    ix_EXPECT(ix_remove_directory("./あ/").is_ok());

    // Export files with the same name from a single archive (single-threaded)
    f({"textunpack", "./", "--stdin", "-n", "1"},
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_BEGIN foo.txt\n"
      "This is also foo.\n"
      "#+EXPORT_END foo.txt\n",
      "",
      "");
    ix_EXPECT(ix_is_file("foo.txt"));
    ix_EXPECT_EQSTR(ix_load_file("foo.txt").unwrap().get(), "This is foo.");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());

    // Export the same file multiple times from a single archive (multi-threaded)
    // Files are overlapped so that the error (alomost) certainly happens.
    if (!ix_is_valgrind_active()) // Context switch gets too slow when valgrind is active.
    {
        const char *input = "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "#+EXPORT_BEGIN foo.txt\n"
                            "This is foo.\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n"
                            "#+EXPORT_END foo.txt\n";
        const ix_TempFileR in(input);
        const ix_TempFileW out;
        ix_TempFileW err;
        textunpack_main(in.file_handle(), out.file_handle(), err.file_handle(), {"textunpack", "./", "--stdin"});

        ix_EXPECT(ix_starts_with(err.data(), "Failed to open: ./foo.txt\n"));
        ix_EXPECT(ix_is_file("foo.txt"));
        ix_EXPECT_EQSTR(ix_load_file("foo.txt").unwrap().get(), "This is foo.");
        ix_EXPECT(ix_remove_file("foo.txt").is_ok());
    }

    // Large archive
    const int N = 1024;
    const char *test_archive_filename = "./test_archive.txt";
    ix_FileHandle file = ix_open_file(test_archive_filename, ix_WRITE_ONLY).unwrap();
    ix_Writer file_writer(4096, &file);
    for (int i = 0; i < N; i++)
    {
        file_writer.write_stringf("#+EXPORT_BEGIN ./0/1/2/3/4/5/6/7/8/9/%d.txt\n", i);
        file_writer.write_stringf("%d\n", i);
        file_writer.write_stringf("#+EXPORT_END ./0/1/2/3/4/5/6/7/8/9/%d.txt\n\n", i);
    }
    file_writer.flush();
    file.close();

    ix_EXPECT(!ix_is_directory("0"));
    f({"textunpack", "test_archive.txt"}, "", "", "");
    ix_EXPECT(ix_is_directory("./0/1/2/3/4/5/6/7/8/9/"));
    for (int i = 0; i < N; i++)
    {
        char filename[128];
        ix_snprintf(filename, sizeof(filename), "./0/1/2/3/4/5/6/7/8/9/%d.txt", i);
        ix_EXPECT(ix_is_file(filename));
        char expected[128];
        ix_snprintf(expected, sizeof(expected), "%d", i);
        ix_EXPECT_EQSTR(ix_load_file(filename).unwrap().get(), expected);
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }
    ix_EXPECT(ix_remove_directory("./0/1/2/3/4/5/6/7/8/9/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/3/4/5/6/7/8/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/3/4/5/6/7/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/3/4/5/6/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/3/4/5/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/3/4/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/3/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/2/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/1/").is_ok());
    ix_EXPECT(ix_remove_directory("./0/").is_ok());
    ix_EXPECT(ix_remove_file("./test_archive.txt").is_ok());

    // --pack
    f({"textunpack", "--pack"}, "", "", ERROR_TEXT_NO_FILE);
    f({"textunpack", "--pack", "-o", "foo.txt"}, "", "", ERROR_TEXT_NO_FILE);
    f({"textunpack", "--pack", "foo.txt"}, "", "", "Failed to open foo.txt: File not found\n");

    ix_EXPECT(ix_write_string_to_file("foo.txt", "foo").is_ok());
    ix_EXPECT(ix_write_string_to_file("bar.txt", "bar").is_ok());
    ix_EXPECT(ix_write_string_to_file("foobar.txt", "foobar").is_ok());
    const char *expected = R"(#+EXPORT_BEGIN foo.txt
foo
#+EXPORT_END foo.txt
#+EXPORT_BEGIN bar.txt
bar
#+EXPORT_END bar.txt
#+EXPORT_BEGIN foobar.txt
foobar
#+EXPORT_END foobar.txt
)";
    f({"textunpack", "--pack", "foo.txt", "bar.txt", "foobar.txt"}, "", expected, "");
    f({"textunpack", "-o", "out.txt", "--pack", "foo.txt", "bar.txt", "foobar.txt"}, "", "", "");
    ix_EXPECT_EQSTR(ix_load_file("out.txt").unwrap().get(), expected);

    ix_EXPECT(ix_remove_file("foo.txt").is_ok());
    ix_EXPECT(ix_remove_file("bar.txt").is_ok());
    ix_EXPECT(ix_remove_file("foobar.txt").is_ok());
    ix_EXPECT(ix_remove_file("out.txt").is_ok());
}
#endif

int main(int argc, const char *argv[])
{
    auto &sm = ix_SystemManager::initialize();
    ix_DEFER([]() { ix_SystemManager::deinitialize(); });
    sm.initialize_stdio().assert_ok();
    sm.initialize_time().assert_ok();
    sm.initialize_logger().assert_ok();

    const int ret = textunpack_main(
        ix_FileHandle::of_stdin(),
        ix_FileHandle::of_stdout(),
        ix_FileHandle::of_stderr(),
        ix_CmdArgsEater(argc, argv)
    );
    return ret;
}
