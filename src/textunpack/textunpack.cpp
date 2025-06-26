#include "textunpack.hpp"

#include <ix.hpp>
#include <ix_StringView.hpp>
#include <ix_Writer.hpp>
#include <ix_file.hpp>
#include <ix_filepath.hpp>
#include <ix_string.hpp>

ix_FORCE_INLINE static bool is_entry_path_terminater(char c)
{
    return (c == '\n') || (c == '\0');
}

size_t tup_calculate_entry_path_length(const char *entry_path_start)
{
    const char *p = entry_path_start;
    while (!is_entry_path_terminater(*p))
    {
        p += 1;
    }

    return static_cast<size_t>(p - entry_path_start);
}

static const char *find_next_export(const char *p)
{
    while (true)
    {
        const char *candidate = ix_strstr(p, "#+EXPORT_");
        if (candidate == nullptr)
        {
            return nullptr;
        }

        if (*(candidate - 1) == '\n')
        {
            return candidate;
        }

        p = candidate + ix_strlen("#+EXPORT_");
    }
}

static void write_entry_fragment(ix_Writer *writer, const char *start, const char *end)
{
    const char *write_start = start;
    const char *p = start;
    while (p < end)
    {
        p = static_cast<const char *>(ix_memchr(p, '\'', static_cast<size_t>(end - p)));
        if (ix_LIKELY(p == nullptr))
        {
            p = end;
            break;
        }

        p += 1;
        const char *next_write_start = p;
        while (*p == '\'')
        {
            p += 1;
        }

        if (!ix_starts_with(p, "#+EXPORT_"))
        {
            continue;
        }

        p += ix_strlen("#+EXPORT_");

        const bool export_begin = ix_starts_with(p, "BEGIN ");
        const bool export_end = ix_starts_with(p, "END ");

        if (!export_begin && !export_end)
        {
            continue;
        }

        size_t entry_path_length = 0;
        if (export_begin)
        {
            p += ix_strlen("BEGIN ");
            entry_path_length = tup_calculate_entry_path_length(p);
        }

        if (export_end)
        {
            p += ix_strlen("END ");
            entry_path_length = tup_calculate_entry_path_length(p);
        }

        if (entry_path_length == 0)
        {
            continue;
        }

        writer->write_between(write_start, next_write_start - ix_strlen("'"));
        write_start = next_write_start;
    }

    writer->write_between(write_start, p);
}

// TODO: Return ix_Buffer
void tup_unpack_entry(
    const ix_StringView &entry_path,
    const char *archive_end,
    ix_Writer *entry_writer,
    const ix_FileHandle *err_handle
)
{
    const char *write_start = entry_path.data() + entry_path.length() + ix_strlen("\n");
    while (true)
    {
        const char *next_export = find_next_export(write_start);

    CHECK_NEXT_EXPORT:
        const bool entry_is_not_terminated = (next_export == nullptr);
        if (ix_UNLIKELY(entry_is_not_terminated))
        {
            if (entry_writer != nullptr)
            {
                entry_writer->write_between(write_start, archive_end);
            }
            if (err_handle != nullptr)
            {
                err_handle->write_stringf(
                    "Entry \"%.*s\" is not terminated properly.\n",
                    static_cast<int>(entry_path.length()),
                    entry_path.data()
                );
            }
            return;
        }

        const char *p = next_export + ix_strlen("#+EXPORT_");
        const bool export_begin = ix_starts_with(p, "BEGIN ") && //
                                  !is_entry_path_terminater(*(p + ix_strlen("BEGIN ")));
        const bool export_end = ix_starts_with(p, "END ") && //
                                !is_entry_path_terminater(*(p + ix_strlen("END ")));

        if (!export_begin && !export_end)
        {
            next_export = find_next_export(p);
            goto CHECK_NEXT_EXPORT;
        }

        bool entry_end = false;
        if (export_end)
        {
            const char *ending_path_start = next_export + ix_strlen("#+EXPORT_END ");
            const size_t ending_path_length = tup_calculate_entry_path_length(ending_path_start);
            const ix_StringView ending_path(ending_path_start, ending_path_length);
            entry_end = (ending_path == entry_path);
        }

        if (entry_writer != nullptr)
        {
            if (write_start < next_export)
            {
                const size_t offset = entry_end ? ix_strlen("\n") : 0;
                write_entry_fragment(entry_writer, write_start, next_export - offset);
            }
        }

        if (entry_end)
        {
            return;
        }

        write_start = next_export + (export_begin ? ix_strlen("#+EXPORT_BEGIN ") : ix_strlen("#+EXPORT_END "));
        while (!is_entry_path_terminater(*write_start))
        {
            write_start += 1;
        }
        if (*write_start == '\n')
        {
            write_start += ix_strlen("\n");
        }
    }
}

const char *tup_find_next_entry_path_start(const char *p)
{
    while (true)
    {
        const char *next_export_begin = ix_strstr(p, "\n#+EXPORT_BEGIN ");
        if (next_export_begin == nullptr)
        {
            return nullptr;
        }

        const char *entry_path_start = next_export_begin + ix_strlen("\n#+EXPORT_BEGIN ");
        const bool entry_path_empty = is_entry_path_terminater(*entry_path_start);
        if (entry_path_empty)
        {
            p = entry_path_start;
            continue;
        }

        return entry_path_start;
    }
}

const char *tup_find_first_entry_path_start(const char *p)
{
    if (ix_starts_with(p, "#+EXPORT_BEGIN"))
    {
        const char *entry_path_start = p + ix_strlen("#+EXPORT_BEGIN ");
        const bool entry_path_exists = !is_entry_path_terminater(*entry_path_start);
        if (entry_path_exists)
        {
            return entry_path_start;
        }
    }

    return tup_find_next_entry_path_start(p);
}

#if ix_DO_TEST
#include <ix_TempFile.hpp>
#include <ix_Vector.hpp>
#include <ix_test.hpp>

ix_TEST_CASE("textunpack: entry detection")
{
    ix_Vector<const char *> v;

    auto f = [&](const char *input) {
        v.clear();
        const char *entry_path_start = tup_find_first_entry_path_start(input);
        while (entry_path_start != nullptr)
        {
            v.emplace_back(entry_path_start);
            entry_path_start = tup_find_next_entry_path_start(entry_path_start);
        }
    };

    f("");
    ix_EXPECT(v.empty());

    f("\n");
    ix_EXPECT(v.empty());

    f("foo\n");
    ix_EXPECT(v.empty());

    f("#+EXPORT_BEGIN ");
    ix_EXPECT(v.empty());

    f("#+EXPORT_BEGIN \n");
    ix_EXPECT(v.empty());

    f("#+EXPORT_BEGIN foo.txt");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT(ix_starts_with(v[0], "foo.txt"));

    f("foo\n"
      "#+EXPORT_BEGIN foo.txt");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT(ix_starts_with(v[0], "foo.txt"));

    f("#+EXPORT_BEGIN foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT(ix_starts_with(v[0], "foo.txt"));

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "#+EXPORT_BEGIN foobar.txt\n");
    ix_EXPECT(v.size() == 3);
    ix_EXPECT(ix_starts_with(v[0], "foo.txt"));
    ix_EXPECT(ix_starts_with(v[1], "bar.txt"));
    ix_EXPECT(ix_starts_with(v[2], "foobar.txt"));

    f("foo\n"
      "#+EXPORT_BEGIN foo.txt\n"
      "bar\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "foobat\n"
      "#+EXPORT_BEGIN foobar.txt\n");
    ix_EXPECT(v.size() == 3);
    ix_EXPECT(ix_starts_with(v[0], "foo.txt"));
    ix_EXPECT(ix_starts_with(v[1], "bar.txt"));
    ix_EXPECT(ix_starts_with(v[2], "foobar.txt"));
}

ix_TEST_CASE("textunpack: tup_calculate_entry_path_length")
{
    ix_Vector<ix_StringView> v;

    auto f = [&](const char *input) {
        v.clear();
        const char *entry_path_start = tup_find_first_entry_path_start(input);
        while (entry_path_start != nullptr)
        {
            v.emplace_back(entry_path_start, tup_calculate_entry_path_length(entry_path_start));
            entry_path_start = tup_find_next_entry_path_start(entry_path_start);
        }
    };

    f("");
    ix_EXPECT(v.empty());

    f("\n");
    ix_EXPECT(v.empty());

    f("foo\n");
    ix_EXPECT(v.empty());

    f("#+EXPORT_BEGIN foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT(v[0] == "foo.txt");

    f("#+EXPORT_BEGIN foo.txt");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT(v[0] == "foo.txt");

    f("foo\n"
      "#+EXPORT_BEGIN foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT(v[0] == "foo.txt");

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "#+EXPORT_BEGIN foobar.txt\n");
    ix_EXPECT(v.size() == 3);
    ix_EXPECT(v[0] == "foo.txt");
    ix_EXPECT(v[1] == "bar.txt");
    ix_EXPECT(v[2] == "foobar.txt");

    f("foo\n"
      "#+EXPORT_BEGIN foo.txt\n"
      "bar\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "foobar\n"
      "#+EXPORT_BEGIN foobar.txt\n");
    ix_EXPECT(v.size() == 3);
    ix_EXPECT(v[0] == "foo.txt");
    ix_EXPECT(v[1] == "bar.txt");
    ix_EXPECT(v[2] == "foobar.txt");
}

ix_TEST_CASE("textunpack: unpack_entry")
{
    ix_Vector<ix_UniquePointer<char[]>> v;
    ix_Writer writer(128);

    auto f = [&](const char *input) {
        v.clear();
        const char *entry_path_start = tup_find_first_entry_path_start(input);
        while (entry_path_start != nullptr)
        {
            const ix_StringView entry_path(entry_path_start, tup_calculate_entry_path_length(entry_path_start));
            tup_unpack_entry(entry_path, ix_strchr(entry_path_start, '\0'), &writer, nullptr);
            writer.end_string();
            v.emplace_back(writer.detach());
            entry_path_start = tup_find_next_entry_path_start(entry_path_start);
        }
    };

    f("#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(v[0].get(), "This is foo.");

    f("foo\n"
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(v[0].get(), "This is foo.");

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(v[0].get(), "");

    f("#+EXPORT_BEGIN foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(v[0].get(), "");

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(v[0].get(), "foo\n");

    f("#+EXPORT_BEGIN outer.txt\n"
      "This is outer.\n"
      "#+EXPORT_BEGIN inner.txt\n"
      "This is inner.\n"
      "#+EXPORT_END inner.txt\n"
      "This is outer.\n"
      "#+EXPORT_END outer.txt\n");
    ix_EXPECT(v.size() == 2);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "This is outer.\n"
        "This is inner.\n"
        "This is outer."
    );
    ix_EXPECT_EQSTR(v[1].get(), "This is inner.");

    f("#+EXPORT_BEGIN 1.txt\n"
      "111\n"
      "#+EXPORT_BEGIN 2.txt\n"
      "111222\n"
      "#+EXPORT_END 1.txt\n"
      "222\n"
      "#+EXPORT_END 2.txt\n");
    ix_EXPECT(v.size() == 2);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "111\n"
        "111222"
    );
    ix_EXPECT_EQSTR(
        v[1].get(),
        "111222\n"
        "222"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_FOOO\n"
      "foo\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "#+EXPORT_FOOO\n"
        "foo"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN \n"
      "foo\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "#+EXPORT_BEGIN \n"
        "foo"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_END \n"
      "foo\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "#+EXPORT_END \n"
        "foo"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      " #+EXPORT_END foo.txt\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(v[0].get(), " #+EXPORT_END foo.txt");

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN foo.txt\n"
      "This is foo.\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_END foo.txt\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 3);
    ix_EXPECT_EQSTR(v[0].get(), "This is foo.");
    ix_EXPECT_EQSTR(v[1].get(), "This is foo.");
    ix_EXPECT_EQSTR(v[2].get(), "This is foo.");

    // quotes
    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'#+EXPORT_BEGIN foo.txt\n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "#+EXPORT_BEGIN foo.txt\n"
        "bar"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'foo\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "'foo"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'#+EXPORT_END foo.txt\n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "#+EXPORT_END foo.txt\n"
        "bar"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'#+EXPORT_BEGIN \n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "'#+EXPORT_BEGIN \n"
        "bar"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'#+EXPORT_END \n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "'#+EXPORT_END \n"
        "bar"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'#+EXPORT_FOO BAR\n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "'#+EXPORT_FOO BAR\n"
        "bar"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'''''#+EXPORT_BEGIN foo.txt\n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "''''#+EXPORT_BEGIN foo.txt\n"
        "bar"
    );

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "'''''#+EXPORT_END foo.txt\n"
      "bar\n"
      "#+EXPORT_END foo.txt\n");
    ix_EXPECT(v.size() == 1);
    ix_EXPECT_EQSTR(
        v[0].get(),
        "foo\n"
        "''''#+EXPORT_END foo.txt\n"
        "bar"
    );
}

ix_TEST_CASE("textunpack: dry_run with warning")
{
    auto f = [&](const char *input, const char *expected_stderr) {
        ix_TempFileW stderr_file;

        const char *entry_path_start = tup_find_first_entry_path_start(input);
        while (entry_path_start != nullptr)
        {
            const ix_StringView entry_path(entry_path_start, tup_calculate_entry_path_length(entry_path_start));
            tup_unpack_entry(entry_path, ix_strchr(entry_path_start, '\0'), nullptr, &stderr_file.file_handle());
            entry_path_start = tup_find_next_entry_path_start(entry_path_start);
        }

        ix_EXPECT_EQSTR(stderr_file.data(), expected_stderr);
    };

    f("", "");

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n"
      "#+EXPORT_END foo.txt\n",
      "");

    f("#+EXPORT_BEGIN foo.txt\n"
      "foo\n",
      "Entry \"foo.txt\" is not terminated properly.\n");

    f("#+EXPORT_BEGIN foo.txt\n"
      "#+EXPORT_BEGIN bar.txt\n"
      "foo\n",
      "Entry \"foo.txt\" is not terminated properly.\n"
      "Entry \"bar.txt\" is not terminated properly.\n");
}
#endif
