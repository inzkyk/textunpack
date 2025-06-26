#include "ix_file.hpp"
#include "ix_HollowValue.hpp"
#include "ix_Option.hpp"
#include "ix_UniquePointer.hpp"
#include "ix_assert.hpp"
#include "ix_filepath.hpp"
#include "ix_memory.hpp"
#include "ix_min_max.hpp"
#include "ix_printf.hpp"
#include "ix_string.hpp"

#include "ix_util_win.hpp"
#include "ix_utility.hpp"

#if ix_DO_TEST
#include "ix_TempFile.hpp"
#include "ix_random.hpp"
#include "ix_test.hpp"
#endif

#include <stdio.h>
#include <stdlib.h>

#if ix_PLATFORM(WIN)

#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_NLS
#include "ix_Windows.hpp"

#include <WinBase.h>
#include <fcntl.h>
#include <io.h>

#if !defined(STDIN_FILENO)
#define STDIN_FILENO 0
#endif
#if !defined(STDOUT_FILENO)
#define STDOUT_FILENO 1
#endif
#if !defined(STDERR_FILENO)
#define STDERR_FILENO 2
#endif

#else

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

#if ix_PLATFORM(WASM)
#include <emscripten.h>
#endif

#if ix_DO_TEST
#if ix_PLATFORM(WASM)
#define CWD "/cwd/"
#else
#define CWD
#endif

static void mount_cwd()
{
#if ix_PLATFORM(WASM)
    // clang-format off
    EM_ASM(
        if (!FS.analyzePath('/cwd').exists) {
            FS.mkdir('/cwd');
        }
        FS.mount(NODEFS, {root : '.'}, '/cwd');,
        ""
    );
    // clang-format on
#endif
}

static void unmount_cwd()
{
#if ix_PLATFORM(WASM)
    // clang-format off
    EM_ASM(
        FS.unmount('/cwd');,
        ""
    );
    // clang-format on
#endif
}
#endif

static ix_HollowValue<ix_FileHandle> g_filehandle_stdin;
static ix_HollowValue<ix_FileHandle> g_filehandle_stdout;
static ix_HollowValue<ix_FileHandle> g_filehandle_stderr;

ix_Result ix_initialize_stdio()
{
#if ix_PLATFORM(WIN)
    const int stdin_oldmode = setmode(STDIN_FILENO, O_BINARY);

    if (stdin_oldmode == -1)
    {
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

    const int stdout_oldmode = setmode(STDOUT_FILENO, O_BINARY);

    if (stdout_oldmode == -1)
    {
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

    const int stderr_oldmode = setmode(STDERR_FILENO, O_BINARY);

    if (stderr_oldmode == -1)
    {
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#endif

    g_filehandle_stdin.construct();
    g_filehandle_stdout.construct();
    g_filehandle_stderr.construct();

#if ix_PLATFORM(WIN)
    g_filehandle_stdin.get().v.handle = GetStdHandle(STD_INPUT_HANDLE);
    g_filehandle_stdout.get().v.handle = GetStdHandle(STD_OUTPUT_HANDLE);
    g_filehandle_stderr.get().v.handle = GetStdHandle(STD_ERROR_HANDLE);
#else
    g_filehandle_stdin.get().v.fd = STDIN_FILENO;
    g_filehandle_stdout.get().v.fd = STDOUT_FILENO;
    g_filehandle_stderr.get().v.fd = STDERR_FILENO;
#endif

    return ix_OK;
}

ix_Result ix_remove_file(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const BOOL ret = DeleteFile(path_wchar);
    if (ret != 0)
    {
        return ix_OK;
    }

    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        return ix_ERROR_FILE_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#else
    const int ret = unlink(path);
    if (ret == 0)
    {
        return ix_OK;
    }
    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_FILE_NOT_FOUND;
    case EACCES:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}

bool ix_is_file(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return false;
    }

    const DWORD ret = GetFileAttributes(path_wchar);
    return (ret != INVALID_FILE_ATTRIBUTES) && ((ret & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
    struct stat sb;
    int ret = stat(path, &sb);
    return (ret == 0) && ((sb.st_mode & S_IFMT) == S_IFREG);
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_is_file")
{
    char buf[64];
    const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
    ix_EXPECT(!ix_is_file(filename));
    const char *message = "hello world";
    ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
    ix_EXPECT(ix_is_file(filename));
    ix_EXPECT(ix_remove_file(filename).is_ok());
    ix_EXPECT(!ix_is_file(filename));

    const char *dirname = ix_temp_dirname();
    ix_EXPECT(!ix_is_file(dirname));
}
#endif

bool ix_is_directory(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return false;
    }

    const DWORD ret = GetFileAttributes(path_wchar);
    return (ret != INVALID_FILE_ATTRIBUTES) && ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
    struct stat sb;
    int ret = stat(path, &sb);
    return (ret == 0) && ((sb.st_mode & S_IFMT) == S_IFDIR);
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_is_directory")
{
    mount_cwd();

    char buf[64];
    const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
    ix_EXPECT(!ix_is_directory(filename));
    const char *message = "hello world";
    ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
    ix_EXPECT(!ix_is_directory(filename));
    ix_EXPECT(ix_remove_file(filename).is_ok());
    ix_EXPECT(!ix_is_directory(filename));

    const char *dirname = ix_temp_dirname();
    ix_EXPECT(ix_is_directory(dirname));

    ix_EXPECT(!ix_is_directory(""));
    ix_EXPECT(ix_is_directory("."));
    ix_EXPECT(ix_is_directory("./"));
    ix_EXPECT(ix_is_directory(".."));
    ix_EXPECT(ix_is_directory("../"));

    ix_EXPECT(ix_create_directory("foo").is_ok());
    ix_EXPECT(ix_is_directory(CWD "foo"));
    ix_EXPECT(ix_is_directory(CWD "./foo"));
    ix_EXPECT(ix_is_directory(CWD "foo/../foo/../foo"));
    ix_EXPECT(ix_is_directory(CWD "foo/../foo/../foo/../foo/"));

#if ix_PLATFORM(WIN)
    ix_EXPECT(ix_is_directory(".\\"));
    ix_EXPECT(ix_is_directory(".\\"));
    ix_EXPECT(ix_is_directory(".\\foo"));
    ix_EXPECT(ix_is_directory(".\\foo\\"));
    ix_EXPECT(ix_is_directory("./foo\\../.\\foo\\../.\\foo"));
    ix_EXPECT(ix_is_directory(".\\foo\\../.\\foo\\.././.\\foo\\../././.\\foo"));
#endif

    ix_EXPECT(ix_remove_directory("foo").is_ok());
    unmount_cwd();
}
#endif

ix_Result ix_write_to_file(const char *filename, const char *data, size_t data_length)
{
    const ix_Option<ix_FileHandle> maybe_file = ix_open_file(filename, ix_WRITE_ONLY);
    if (maybe_file.is_error())
    {
        return maybe_file.result_kind();
    }
    const ix_FileHandle &file = maybe_file.unwrap();
    file.write(data, data_length);
    return ix_OK;
}

ix_Result ix_write_string_to_file(const char *filename, const char *str)
{
    return ix_write_to_file(filename, str, ix_strlen(str));
}

ix_Option<ix_UniquePointer<char[]>> ix_load_file(const char *filename, size_t *data_length_out, size_t padding_length)
{
    ix_ASSERT(filename != nullptr);

    ix_Option<ix_FileHandle> file_opt = ix_open_file(filename, ix_READ_ONLY);
    if (file_opt.is_error())
    {
        return file_opt.result_kind();
    }
    const ix_FileHandle &file = file_opt.unwrap();
    ix_ASSERT(file.is_valid());

    return file.read_all(data_length_out, padding_length);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_load_file")
{
    // nonexistent file
    {
        const ix_Option<ix_UniquePointer<char[]>> p = ix_load_file("foo", nullptr);
        ix_EXPECT_EQ(p.result_kind(), ix_ERROR_FILE_NOT_FOUND);
    }

    // small file
    {
        char buf[64];
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        const char *message = "hello world!";
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
        size_t length = 0;
        ix_UniquePointer<char[]> str = ix_load_file(filename, &length).unwrap();
        ix_EXPECT(str.get() != nullptr);
        ix_EXPECT_EQSTR(str.get(), message);
        ix_EXPECT(length == 12);
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }
}
#endif

ix_Result ix_remove_directory(const char *path)
{
#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const BOOL ret = RemoveDirectory(path_wchar);
    if (ret != 0)
    {
        return ix_OK;
    }
    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        return ix_ERROR_DIR_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#else
    const int ret = rmdir(path);
    if (ret == 0)
    {
        return ix_OK;
    }
    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_DIR_NOT_FOUND;
    case EACCES:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}

ix_Result ix_create_directory(const char *path)
{
    if (*path == '\0')
    {
        return ix_ERROR_DIR_ALREADY_EXISTS;
    }

#if ix_PLATFORM(WIN)
    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const BOOL ret = CreateDirectory(path_wchar, nullptr);
    if (ret != 0)
    {
        return ix_OK;
    }

    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_ALREADY_EXISTS:
        return ix_ERROR_DIR_ALREADY_EXISTS;
    case ERROR_PATH_NOT_FOUND:
        return ix_ERROR_DIR_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#else
    const int ret = mkdir(path, 0777);
    if (ret == 0)
    {
        return ix_OK;
    }

    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_DIR_NOT_FOUND;
    case EACCES:
        return ix_ERROR_DIR_ACCESS_DENIED;
    case EEXIST:
        return ix_ERROR_DIR_ALREADY_EXISTS;
#if ix_PLATFORM(WASM)
    case EINVAL:
        return ix_is_directory(path) ? ix_ERROR_DIR_ALREADY_EXISTS : ix_ERROR_SOMETHING_WENT_WRONG;
#endif
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_create_directory")
{
    mount_cwd();

    ix_EXPECT_EQ(ix_create_directory("").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory(".").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("./").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("..").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("../").kind(), ix_ERROR_DIR_ALREADY_EXISTS);

    ix_EXPECT(ix_create_directory("foo").is_ok());
    ix_EXPECT_EQ(ix_create_directory("foo").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_create_directory("foo/").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_create_directory("./foo/").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_create_directory("foo").is_ok());
    ix_EXPECT(ix_create_directory("foo/bar").is_ok());
    ix_EXPECT(ix_create_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo/bar"));
    ix_EXPECT(ix_is_directory("foo/bar/foobar"));
    ix_EXPECT_EQ(ix_create_directory("foo").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo/bar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo/bar/foobar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT(ix_remove_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_create_directory("./././././././././foo").is_ok());
    ix_EXPECT(ix_create_directory("./foo/.././foo/././../././foo/././bar").is_ok());
    ix_EXPECT(ix_create_directory("./foo/.././foo/./bar/.././bar/../../foo/././bar/../../foo/bar/foobar").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo/bar"));
    ix_EXPECT(ix_is_directory("foo/bar/foobar"));
    ix_EXPECT_EQ(ix_create_directory("foo").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo/bar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo/bar/foobar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT(ix_remove_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    char buf[64];
    const char *path = ix_temp_filename(buf, ix_LENGTH_OF(buf));
    ix_EXPECT(ix_create_directory(path).is_ok());
    ix_EXPECT_EQ(ix_create_directory(path).kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT(ix_is_directory(path));
    ix_EXPECT(ix_remove_directory(path).is_ok());

#if ix_PLATFORM(WIN)
    ix_EXPECT_EQ(ix_create_directory(".\\").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("..\\").kind(), ix_ERROR_DIR_ALREADY_EXISTS);

    ix_EXPECT(ix_create_directory("foo").is_ok());
    ix_EXPECT(ix_create_directory("foo\\bar").is_ok());
    ix_EXPECT(ix_create_directory("foo\\bar\\foobar").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo\\bar"));
    ix_EXPECT(ix_is_directory("foo\\bar\\foobar"));
    ix_EXPECT_EQ(ix_create_directory("foo").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo\\bar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo\\bar\\foobar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT(ix_remove_directory("foo\\bar\\foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo\\bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_create_directory(".\\./.\\.\\./.\\.\\./.\\foo").is_ok());
    ix_EXPECT(ix_create_directory(".\\foo\\../.\\foo\\./.\\..\\.\\./foo\\.\\.\\bar").is_ok());
    ix_EXPECT(ix_create_directory("./foo\\..\\foo\\./bar/..\\..\\foo\\./bar.\\..\\../foo\\bar\\foobar").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo\\bar"));
    ix_EXPECT(ix_is_directory("foo\\bar\\foobar"));
    ix_EXPECT_EQ(ix_create_directory("foo").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo\\bar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT_EQ(ix_create_directory("foo\\bar\\foobar").kind(), ix_ERROR_DIR_ALREADY_EXISTS);
    ix_EXPECT(ix_remove_directory("foo\\bar\\foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo\\bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());
#endif

    unmount_cwd();
}
#endif

#if ix_PLATFORM(WIN)
ix_FORCE_INLINE static bool is_path_separator(wchar_t c)
{
    return (c == L'\\') || (c == '/');
}

static wchar_t *next_path_separator(wchar_t *p)
{
    wchar_t c = *p;
    while (!is_path_separator(c) && (c != L'\0'))
    {
        p += 1;
        c = *p;
    }
    return p;
}
#endif

ix_Result ix_ensure_directories(const char *path)
{
#if ix_PLATFORM(WIN)
    if (*path == '\0')
    {
        return ix_OK;
    }

    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    const DWORD ret = GetFileAttributes(path_wchar);
    const bool exists = (ret != INVALID_FILE_ATTRIBUTES) && ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0);
    if (exists)
    {
        const bool dir = ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0);
        return dir ? ix_OK : ix_ERROR_FILE_ALREADY_EXISTS;
    }

    wchar_t *path_wchar_end = next_path_separator(path_wchar + 1);
    while (*path_wchar_end != L'\0')
    {
        const wchar_t temp = *path_wchar_end;
        *path_wchar_end = L'\0';
        const BOOL ret_bool = CreateDirectory(path_wchar, nullptr);
        *path_wchar_end = temp;
        const bool fail = (ret_bool == 0);
        if (fail)
        {
            const DWORD err = GetLastError();
            switch (err)
            {
            case ERROR_ALREADY_EXISTS:
                break;
            case ERROR_PATH_NOT_FOUND:
                return ix_ERROR_DIR_NOT_FOUND;
            case ERROR_ACCESS_DENIED:
                return ix_ERROR_DIR_ACCESS_DENIED;
            default:
                return ix_ERROR_SOMETHING_WENT_WRONG;
            }
        }
        path_wchar_end = next_path_separator(path_wchar_end + 1);
    }

    const bool ends_with_separator = is_path_separator(*(path_wchar_end - 1));
    if (ends_with_separator)
    {
        return ix_OK;
    }

    const BOOL ret_bool = CreateDirectory(path_wchar, nullptr);
    const bool success = (ret_bool != 0);
    if (success)
    {
        return ix_OK;
    }

    const DWORD err = GetLastError();
    switch (err)
    {
    case ERROR_PATH_NOT_FOUND:
        return ix_ERROR_DIR_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#else
    if (*path == '\0')
    {
        return ix_OK;
    }

    if (ix_is_directory(path))
    {
        return ix_OK;
    }

    char buf[ix_MAX_PATH + 1];
    strncpy(buf, path, sizeof(buf));
    const bool path_too_long = (buf[sizeof(buf) - 1] != '\0');
    if (path_too_long)
    {
        return ix_ERROR_PATH_TOO_LONG;
    }

    char *p = ix_next_path_separator(buf + 1);
    while (*p != '\0')
    {
        const char temp = *p;
        *p = '\0';
        const int ret = mkdir(buf, 0777);
        const bool fail = (ret != 0);
        if (fail)
        {
            bool exists = (errno == EEXIST);
#if ix_PLATFORM(WASM)
            // At least on WASM, `mkdir(".")` does not set errno to EEXIST.
            exists |= ix_is_directory(buf);
#endif
            if (!exists)
            {
                switch (errno)
                {
                case ENOENT:
                    return ix_ERROR_DIR_NOT_FOUND;
                case EACCES:
                    return ix_ERROR_DIR_ACCESS_DENIED;
                default:
                    return ix_ERROR_SOMETHING_WENT_WRONG;
                }
            }
        }
        *p = temp;
        p = ix_next_path_separator(p + 1);
    }

    const bool ends_with_separator = ix_is_path_separator(*(p - 1));
    if (ends_with_separator)
    {
        return ix_OK;
    }

    const int ret = mkdir(path, 0777);
    const bool success = (ret == 0);
    if (success)
    {
        return ix_OK;
    }

    ix_ASSERT(errno != EEXIST);

    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_DIR_NOT_FOUND;
    case EACCES:
        return ix_ERROR_DIR_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_ensure_directories")
{
    mount_cwd();

    ix_EXPECT(ix_ensure_directories("").is_ok());
    ix_EXPECT(ix_ensure_directories(".").is_ok());
    ix_EXPECT(ix_ensure_directories("./").is_ok());
    ix_EXPECT(ix_ensure_directories("..").is_ok());
    ix_EXPECT(ix_ensure_directories("../").is_ok());

    ix_EXPECT(ix_ensure_directories("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_ensure_directories("foo").is_ok());
    ix_EXPECT(ix_ensure_directories("foo/bar").is_ok());
    ix_EXPECT(ix_ensure_directories("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo/bar"));
    ix_EXPECT(ix_is_directory("foo/bar/foobar"));
    ix_EXPECT(ix_remove_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_ensure_directories("foo/bar/foobar/").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo/bar"));
    ix_EXPECT(ix_is_directory("foo/bar/foobar"));
    ix_EXPECT(ix_remove_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());

    ix_EXPECT(ix_ensure_directories("./foo/bar/foobar/").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo/bar"));
    ix_EXPECT(ix_is_directory("foo/bar/foobar"));
    ix_EXPECT(ix_remove_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());

#if ix_PLATFORM(WASM)
    ix_EXPECT(ix_ensure_directories("./../cwd/././foo/./../foo/././bar/./../../foo/bar/././foobar/").is_ok());
    ix_EXPECT(ix_ensure_directories("./../cwd/././foo/./../foo/././bar/./../../foo/bar/././foobar/").is_ok());
#else
    ix_EXPECT(ix_ensure_directories("././foo/./../foo/././bar/./../../foo/bar/././foobar/").is_ok());
    ix_EXPECT(ix_ensure_directories("./././foo/./../foo/././bar/./../../foo/bar/././foobar/").is_ok());
#endif

    ix_EXPECT(ix_is_directory(CWD "foo"));
    ix_EXPECT(ix_is_directory(CWD "foo/bar"));
    ix_EXPECT(ix_is_directory(CWD "foo/bar/foobar"));
    ix_EXPECT(ix_remove_directory(CWD "foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory(CWD "foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory(CWD "foo").is_ok());

    char buf[64];
    const char *path = ix_temp_filename(buf, ix_LENGTH_OF(buf));
    ix_EXPECT(ix_ensure_directories(path).is_ok());
    ix_EXPECT(ix_is_directory(path));
    ix_EXPECT(ix_remove_directory(path).is_ok());

#if ix_PLATFORM(WIN)
    ix_EXPECT(ix_ensure_directories(".\\").is_ok());
    ix_EXPECT(ix_ensure_directories("..\\").is_ok());

    ix_EXPECT(ix_ensure_directories(R"(.\\././foo\./../foo/.\./bar/./..\..\foo/bar/.\./foobar\)").is_ok());
    ix_EXPECT(ix_ensure_directories(R"(.\\././foo\./../foo/.\./bar/./..\..\foo/bar/.\./foobar\)").is_ok());
    ix_EXPECT(ix_is_directory("foo"));
    ix_EXPECT(ix_is_directory("foo/bar"));
    ix_EXPECT(ix_is_directory("foo/bar/foobar"));
    ix_EXPECT(ix_remove_directory("foo/bar/foobar").is_ok());
    ix_EXPECT(ix_remove_directory("foo/bar").is_ok());
    ix_EXPECT(ix_remove_directory("foo").is_ok());
#endif
    ix_EXPECT_EQ(
        ix_ensure_directories(
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
            "01234567890123456789012345678901234567890123456789"
        )
            .kind(),
        ix_ERROR_PATH_TOO_LONG
    );

    unmount_cwd();
}
#endif

ix_Option<ix_FileHandle> ix_create_directories_and_file(const char *path)
{
    ix_Option<ix_FileHandle> maybe_file = ix_open_file(path, ix_WRITE_ONLY);
    if (maybe_file.is_ok())
    {
        return maybe_file;
    }

    const size_t dirname_length = ix_dirname_length(path);
    const bool path_too_long = (dirname_length >= ix_MAX_PATH);
    if (path_too_long)
    {
        return ix_FileHandle();
    }

    char buf[ix_MAX_PATH + 1];
    ix_memcpy(buf, path, dirname_length);
    buf[dirname_length] = '\0';
    const ix_Result result = ix_ensure_directories(buf);
    if (result.is_error())
    {
        return ix_FileHandle();
    }

    return ix_open_file(path, ix_WRITE_ONLY);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_create_directories_and_file")
{
    ix_EXPECT(ix_create_directories_and_file("").is_error());

    auto f = [](const char *path) {
        ix_EXPECT(!ix_is_file(path));

        ix_Option<ix_FileHandle> maybe_file = ix_create_directories_and_file(path);
        ix_EXPECT(maybe_file.is_ok());
        maybe_file.unwrap().close();

        ix_EXPECT(ix_is_file(path));
    };

    f("foo.txt");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());

    f("foo/foo/foo.txt");
    ix_EXPECT(ix_remove_file("foo/foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_directory("foo/foo/").is_ok());
    ix_EXPECT(ix_remove_directory("foo/").is_ok());

    char buf[64];
    const char *temp_path;
    temp_path = ix_temp_filename(buf, ix_LENGTH_OF(buf));
    f(temp_path);
    ix_EXPECT(ix_remove_file(temp_path).is_ok());

    auto g = [&](const char *path) {
        ix_EXPECT(!ix_is_file(path));

        const size_t path_length = ix_strlen(path);
        ix_memcpy(buf, path, path_length);
        buf[path_length] = '\0';

        ix_Option<ix_FileHandle> maybe_file = ix_create_directories_and_file(path);
        ix_EXPECT(maybe_file.is_ok());
        maybe_file.unwrap().close();

        ix_EXPECT(ix_is_file(path));
    };

    g("foo.txt");
    ix_EXPECT(ix_remove_file("foo.txt").is_ok());

    g("foo/foo/foo.txt");
    ix_EXPECT(ix_remove_file("foo/foo/foo.txt").is_ok());
    ix_EXPECT(ix_remove_directory("foo/foo/").is_ok());
    ix_EXPECT(ix_remove_directory("foo/").is_ok());

    temp_path = ix_temp_filename(buf, ix_LENGTH_OF(buf));
    g(temp_path);
    ix_EXPECT(ix_remove_file(temp_path).is_ok());
}
#endif

ix_FileHandle ix_FileHandle::null(ix_FileOpenOption option)
{
#if ix_PLATFORM(WIN)
    return ix_open_file("NUL", option).unwrap();
#else
    return ix_open_file("/dev/null", option).unwrap();
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: null")
{
    const ix_FileHandle null_read = ix_FileHandle::null(ix_READ_ONLY);
    size_t bytes_read = 0;
    ix_EXPECT(null_read.read_all(&bytes_read).is_ok());
    ix_EXPECT(bytes_read == 0);

    const ix_FileHandle null_write = ix_FileHandle::null(ix_WRITE_ONLY);
    const char *message = "hello null!";
    null_write.write_string(message);
}
#endif

ix_FileHandle::ix_FileHandle()
{
    invalidate();
}

ix_Option<ix_FileHandle> ix_open_file(const char *path, ix_FileOpenOption option)
{
#if ix_PLATFORM(WIN)

    wchar_t path_wchar[ix_MAX_PATH + 1];
    const ix_Option<size_t> length_opt = utf8_path_to_wchar(path, path_wchar);

    if (length_opt.is_error())
    {
        return length_opt.result_kind();
    }

    HANDLE handle = nullptr;

    switch (option)
    {
    case ix_READ_ONLY: {
        handle = CreateFile(
            path_wchar,
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        break;
    }

    case ix_WRITE_ONLY: {
        handle = CreateFile(path_wchar, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        break;
    }
        ix_CASE_EXHAUSTED();
    }

    if (handle != INVALID_HANDLE_VALUE)
    {
        ix_FileHandle file_handle;
        file_handle.v.handle = handle;
        return file_handle;
    }

    const DWORD err = GetLastError();
    ix_ASSERT(err != 0);
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
        return ix_ERROR_FILE_NOT_FOUND;
    case ERROR_ACCESS_DENIED:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#else

    int fd;

    switch (option)
    {
    case ix_READ_ONLY: {
        fd = open(path, O_RDONLY);
        break;
    }
    case ix_WRITE_ONLY: {
        fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        break;
    }
        ix_CASE_EXHAUSTED();
    }

    if (fd != -1)
    {
        ix_FileHandle file_handle;
        file_handle.v.fd = fd;
        return file_handle;
    }

    switch (errno)
    {
    case ENOENT:
        return ix_ERROR_FILE_NOT_FOUND;
    case EACCES:
        return ix_ERROR_FILE_ACCESS_DENIED;
    default:
        return ix_ERROR_SOMETHING_WENT_WRONG;
    }

#endif
}

ix_FileHandle::ix_FileHandle(ix_FileHandle &&other)
    : v(other.v)
{
    other.invalidate();
}

ix_FileHandle &ix_FileHandle::operator=(ix_FileHandle &&other)
{
    if (is_valid())
    {
        close();
    }

    v = other.v;
    other.invalidate();

    return *this;
}

ix_FileHandle::~ix_FileHandle()
{
    if (is_valid())
    {
        close();
    }
}

void ix_FileHandle::invalidate()
{
#if ix_PLATFORM(WIN)
    v.handle = INVALID_HANDLE_VALUE;
#else
    v.fd = -1;
#endif
}

void ix_FileHandle::close()
{
#if ix_PLATFORM(WIN)
    const BOOL ret = CloseHandle(v.handle);
    ix_ASSERT_FATAL(ret != 0, "CloseHandle() returned 0");
#else
    int ret = ::close(v.fd);
    ix_ASSERT_FATAL(ret == 0, "close() did not return 0");
#endif
    invalidate();
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: close")
{
    mount_cwd();
    ix_EXPECT(ix_write_string_to_file("foo.txt", "foo").is_ok());

    ix_Option<ix_FileHandle> maybe_file = ix_open_file("foo.txt", ix_READ_ONLY);
    ix_EXPECT(maybe_file.is_ok());
    ix_FileHandle &file = maybe_file.unwrap();
    file.close();
    ix_EXPECT(!file.is_valid());

    ix_EXPECT(ix_remove_file("foo.txt").is_ok());
    unmount_cwd();
}
#endif

bool ix_FileHandle::is_valid() const
{
#if ix_PLATFORM(WIN)
    return (v.handle != INVALID_HANDLE_VALUE);
#else
    return (v.fd != -1);
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: is_valid")
{
    mount_cwd();
    ix_TempFileR temp("foo");

    const ix_FileHandle file;
    ix_EXPECT(!file.is_valid());
    ix_EXPECT(ix_open_file(CWD temp.filename(), ix_READ_ONLY).unwrap().is_valid());
    ix_EXPECT(ix_open_file(CWD temp.filename(), ix_READ_ONLY).unwrap().is_valid());
    ix_EXPECT(ix_open_file(CWD "foo", ix_READ_ONLY).is_error());

    unmount_cwd();
}
#endif

HANDLE ix_FileHandle::get_native_handle() const
{
    return v.handle;
}

int ix_FileHandle::get_native_fd() const
{
    return v.fd;
}

void ix_FileHandle::write(const void *data, size_t data_length) const
{
#if ix_PLATFORM(WIN)
    while (data_length > 0)
    {
        DWORD bytes_written;
        // For some reason, this makes the write much faster (see benchmark_io).
        const DWORD write_amount = ix_min(static_cast<DWORD>(data_length), 1024UL * 1024UL);
        const BOOL ret = WriteFile(v.handle, data, write_amount, &bytes_written, nullptr);
        const bool fail = (ret == 0);
        if (fail)
        {
            return;
        }
        data = static_cast<const uint8_t *>(data) + bytes_written;
        data_length -= bytes_written;
    }
#else
    while (data_length > 0)
    {
        const ssize_t bytes_written = ::write(v.fd, data, static_cast<unsigned int>(data_length));
        const bool fail = (bytes_written == -1);
        if (fail)
        {
            return;
        }
        data = static_cast<const uint8_t *>(data) + bytes_written;
        const size_t bytes_written_unsigned = static_cast<size_t>(bytes_written);
        data_length -= bytes_written_unsigned;
    }
#endif
}

void ix_FileHandle::write_string(const char *str) const
{
    write(str, ix_strlen(str));
}

void ix_FileHandle::write_char(char c) const
{
    write(&c, 1);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: write_char")
{
    ix_TempFileW w;
    w.file_handle().write_char('a');
    w.file_handle().write_char('b');
    w.file_handle().write_char('c');
    ix_EXPECT_EQSTR(w.data(), "abc");
}
#endif

ix_PRINTF_FORMAT(2, 0) size_t ix_FileHandle::write_stringfv(ix_FORMAT_ARG const char *format, va_list args) const
{
    va_list args_copy;
    va_copy(args_copy, args); // Copy must occur here.

    const int required_length_s = ix_vsnprintf(nullptr, 0, format, args) + 1;
    ix_ASSERT(required_length_s > 0);
    const size_t required_length = static_cast<size_t>(required_length_s);

    char buffer[ix_OPT_LEVEL(DEBUG) ? 32 : 1024];
    char *formatted = buffer;
    if (required_length > ix_LENGTH_OF(buffer))
    {
        formatted = ix_MALLOC(char *, required_length);
    }

    const int formatted_length_signed = ix_vsnprintf(formatted, required_length, format, args_copy);
    va_end(args_copy);
    ix_ASSERT(formatted_length_signed >= 0);

    const size_t formatted_length = static_cast<size_t>(formatted_length_signed);
    write(formatted, formatted_length);

    const bool data_allocated = (formatted != buffer);
    if (data_allocated)
    {
        ix_FREE(formatted);
    }

    return formatted_length;
}

ix_PRINTF_FORMAT(2, 3) size_t ix_FileHandle::write_stringf(ix_FORMAT_ARG const char *format, ...) const
{
    va_list args;
    va_start(args, format);
    const size_t ret = write_stringfv(format, args);
    va_end(args);
    return ret;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: write_stringf")
{
    char buf[64];

    // empty
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        ix_FileHandle file = ix_open_file(filename, ix_WRITE_ONLY).unwrap();

        ix_DISABLE_GCC_WARNING_BEGIN
        ix_DISABLE_GCC_WARNING("-Wformat-zero-length")
        ix_EXPECT_EQ(file.write_stringf(""), 0);
        ix_DISABLE_GCC_WARNING_END
        file.close();
        ix_UniquePointer<char[]> output = ix_load_file(filename).unwrap();
        ix_EXPECT_EQSTR(output.get(), "");
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // literal
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        ix_FileHandle file = ix_open_file(filename, ix_WRITE_ONLY).unwrap();
        ix_EXPECT_EQ(file.write_stringf("hello world!"), 12);
        file.close();
        ix_UniquePointer<char[]> output = ix_load_file(filename).unwrap();
        ix_EXPECT_EQSTR(output.get(), "hello world!");
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // format string
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        ix_FileHandle file = ix_open_file(filename, ix_WRITE_ONLY).unwrap();
        ix_EXPECT_EQ(file.write_stringf("%s %s!", "hello", "world"), 12);
        file.close();
        ix_UniquePointer<char[]> output = ix_load_file(filename).unwrap();
        ix_EXPECT_EQSTR(output.get(), "hello world!");
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // long format string
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        ix_FileHandle file = ix_open_file(filename, ix_WRITE_ONLY).unwrap();
        const int x = 1 << 15;
        ix_EXPECT_EQ(file.write_stringf("%d%d%d%d%d%d%d%d", x, x, x, x, x, x, x, x), 40);
        file.close();
        ix_UniquePointer<char[]> output = ix_load_file(filename).unwrap();
        ix_EXPECT_EQSTR(output.get(), "3276832768327683276832768327683276832768");
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }
}
#endif

size_t ix_FileHandle::read(void *buffer, size_t buffer_length) const
{
#if ix_PLATFORM(WIN)
    size_t total_bytes_read = 0;
    while (buffer_length > 0)
    {
        DWORD bytes_read;
        const BOOL ret = ReadFile(v.handle, buffer, static_cast<DWORD>(buffer_length), &bytes_read, nullptr);
        const bool fail = (ret == 0);
        if (fail)
        {
            const DWORD err = GetLastError();
            return (err == ERROR_BROKEN_PIPE) ? total_bytes_read : ix_SIZE_MAX;
        }
        const bool eof = (bytes_read == 0);
        if (eof)
        {
            break;
        }
        buffer = static_cast<char *>(buffer) + bytes_read;
        const size_t bytes_read_unsigned = static_cast<size_t>(bytes_read);
        buffer_length -= bytes_read_unsigned;
        total_bytes_read += bytes_read_unsigned;
    }

    return total_bytes_read;
#else
    if (v.fd == -1)
    {
        return ix_SIZE_MAX;
    }

    size_t total_bytes_read = 0;
    while (buffer_length > 0)
    {
        const ssize_t bytes_read = ::read(v.fd, buffer, buffer_length);
        const bool fail = (bytes_read == -1);
        if (fail)
        {
            return ix_SIZE_MAX;
        }
        const bool eof = (bytes_read == 0);
        if (eof)
        {
            break;
        }
        buffer = static_cast<char *>(buffer) + bytes_read;
        const size_t bytes_read_unsigned = static_cast<size_t>(bytes_read);
        buffer_length -= bytes_read_unsigned;
        total_bytes_read += bytes_read_unsigned;
    }

    return total_bytes_read;
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: read/write")
{
    char path_buf[128];
    const char *path = ix_temp_filename(path_buf, ix_LENGTH_OF(path_buf));
    const char *message = "hello world!";

    ix_FileHandle file = ix_open_file(path, ix_WRITE_ONLY).unwrap();
    ix_EXPECT(file.is_valid());
    file.write_string(message);
    file.close();

    ix_EXPECT(ix_is_file(path));
    file = ix_open_file(path, ix_READ_ONLY).unwrap();
    ix_EXPECT(file.is_valid());

    char buf[128];
    const size_t bytes_read = file.read(buf, sizeof(buf));
    file.close();
    ix_EXPECT(bytes_read == ix_strlen(message));
    buf[bytes_read] = '\0';
    ix_EXPECT_EQSTR(buf, message);
    ix_EXPECT(ix_remove_file(path).is_ok());
}

ix_TEST_CASE("ix_FileHandle: read/write with a mutibyte filename")
{
    char buf[128];
    const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf), "一時ファイル_");
    const char *message = "こんにちは、世界！";
    ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
    ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
    ix_EXPECT(file.is_valid());
    ix_UniquePointer<char[]> str = file.read_all(nullptr).unwrap();
    ix_EXPECT_EQSTR(str.get(), message);
    file.close();
    ix_EXPECT(ix_remove_file(filename).is_ok());
}
#endif

ix_Option<ix_UniquePointer<char[]>> ix_FileHandle::read_all(
    size_t *data_length_out,
    size_t padding_length,
    size_t first_read_size
) const
{
    char *data = nullptr;
    size_t data_size;

    const size_t file_size = size();
    const bool file_size_valid = (file_size != ix_SIZE_MAX);
    if (file_size_valid)
    {
        data = ix_MALLOC(char *, file_size + padding_length);
        data_size = file_size;
        const size_t bytes_written = read(data, file_size);
        if (bytes_written != file_size)
        {
            ix_FREE(data);
            return ix_UniquePointer<char[]>(nullptr);
        }
    }
    else
    {
        data_size = 0;
        size_t read_size = first_read_size;
        ix_ASSERT(first_read_size > 0);
        while (true)
        {
            data = ix_REALLOC(char *, data, data_size + read_size + padding_length);

            const size_t bytes_read = read(data + data_size, read_size);
            const bool failed = (bytes_read == ix_SIZE_MAX);
            if (failed)
            {
                ix_FREE(data);
                return ix_UniquePointer<char[]>(nullptr);
            }

            data_size += bytes_read;

            const bool consumed = (bytes_read == 0);
            if (consumed)
            {
                break;
            }

            if (bytes_read == read_size)
            {
                read_size *= 2;
            }
        }
    }

    for (size_t i = 0; i < padding_length; i++)
    {
        data[data_size + i] = '\0';
    }

    if (data_length_out != nullptr)
    {
        *data_length_out = data_size;
    }

    return ix_UniquePointer<char[]>(ix_move(data));
}

#if ix_DO_TEST
ix_TEST_CASE("ix_read_all")
{
    char buf[64];

    // without size
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        const char *message = "hello world!";
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
        ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
        ix_UniquePointer<char[]> str = file.read_all(nullptr).unwrap();
        file.close();
        ix_EXPECT(str.get() != nullptr);
        ix_EXPECT_EQSTR(str.get(), "hello world!");
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // with size
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        const char *message = "hello world!";
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
        ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
        size_t size = 0;
        const ix_UniquePointer<char[]> str = file.read_all(&size).unwrap();
        file.close();
        ix_EXPECT(str.get() != nullptr);
        ix_EXPECT_EQSTR(str.get(), "hello world!");
        ix_EXPECT(str[size] == '\0');
        ix_EXPECT(size == 12);
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // mid-sized file
    {
        char message[2048 + 1];
        ix_rand_fill_alphanumeric(message, ix_LENGTH_OF(message) - 1);

        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());

        ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
        const ix_UniquePointer<char[]> p = file.read_all(nullptr).unwrap();
        file.close();

        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // with padding
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        const char *message = "1234";
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
        ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
        size_t data_length = 0;
        ix_UniquePointer<char[]> data = file.read_all(&data_length, 4).unwrap();
        ix_EXPECT_EQ(data_length, 4);
        ix_EXPECT_EQ(data[4], '\0');
        ix_EXPECT_EQ(data[5], '\0');
        ix_EXPECT_EQ(data[6], '\0');
        ix_EXPECT_EQ(data[7], '\0');
        file.close();
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }
}
#endif

size_t ix_FileHandle::size() const
{
#if ix_PLATFORM(WIN)
    LARGE_INTEGER file_size;
    const BOOL ret = GetFileSizeEx(v.handle, &file_size);
    if (ret == 0)
    {
        return ix_SIZE_MAX;
    }
    return static_cast<size_t>(file_size.QuadPart);
#else
    const off_t curr = lseek(v.fd, 0, SEEK_CUR);
    if (curr == off_t{-1})
    {
        return ix_SIZE_MAX;
    }

    const off_t file_size = lseek(v.fd, 0, SEEK_END);
    if (file_size == off_t{-1})
    {
        return ix_SIZE_MAX;
    }

    const off_t ret = lseek(v.fd, curr, SEEK_SET);
    if (ret == off_t{-1})
    {
        return ix_SIZE_MAX;
    }

    return static_cast<size_t>(file_size);
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: size")
{
    char buf[64];

    // zero
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        const char *message = "";
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
        ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
        ix_EXPECT(file.is_valid());
        ix_EXPECT(file.size() == 0);
        file.close();
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }

    // 11 bytes
    {
        const char *filename = ix_temp_filename(buf, ix_LENGTH_OF(buf));
        const char *message = "hello world";
        ix_EXPECT(ix_write_string_to_file(filename, message).is_ok());
        ix_FileHandle file = ix_open_file(filename, ix_READ_ONLY).unwrap();
        ix_EXPECT(file.size() == 11);
        file.close();
        ix_EXPECT(ix_remove_file(filename).is_ok());
    }
}
#endif

void ix_FileHandle::substitute(ix_FileHandle &&another_file)
{
    if (is_valid())
    {
        close();
    }
    v.handle = another_file.v.handle;
    another_file.invalidate();
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: substitute")
{
    char buf[2][ix_MAX_PATH + 1];
    ix_temp_filename(buf[0], ix_LENGTH_OF(buf[0]));
    ix_temp_filename(buf[1], ix_LENGTH_OF(buf[1]));
    ix_FileHandle files[2];
    files[0] = ix_open_file(buf[0], ix_WRITE_ONLY).unwrap();
    files[0].write_string("0");
    files[1] = ix_open_file(buf[1], ix_WRITE_ONLY).unwrap();
    files[1].write_string("1");

    files[0].substitute(ix_move(files[1]));
    ix_EXPECT(files[0].is_valid());
    ix_EXPECT(!files[1].is_valid());
    files[0].write_string("0");
    files[0].close();

    ix_EXPECT_EQSTR(ix_load_file(buf[0]).unwrap().get(), "0");
    ix_EXPECT_EQSTR(ix_load_file(buf[1]).unwrap().get(), "10");

    ix_EXPECT(ix_remove_file(buf[0]).is_ok());
    ix_EXPECT(ix_remove_file(buf[1]).is_ok());
}
#endif

ix_FileHandle ix_FileHandle::copy() const
{
#if ix_PLATFORM(WIN)
    ix_FileHandle copy;
    HANDLE process = GetCurrentProcess();
    const BOOL ret = DuplicateHandle(
        process,        // Source process
        v.handle,       // Source handle
        process,        // Target process
        &copy.v.handle, // New handle
        0,              // Desired access (0 means same access)
        FALSE,          // Not inheritable
        DUPLICATE_SAME_ACCESS
    );
    ix_ASSERT_FATAL(ret, "File handle duplication failed");
    return copy;
#else
    ix_FileHandle copy;
    copy.v.fd = dup(v.fd);
    ix_ASSERT_FATAL(copy.v.fd != -1, "File handle duplication failed");
    return copy;
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: copy")
{
    char buf[2][ix_MAX_PATH + 1];
    ix_temp_filename(buf[0], ix_LENGTH_OF(buf[0]));
    ix_temp_filename(buf[1], ix_LENGTH_OF(buf[1]));
    ix_FileHandle files[2];
    files[0] = ix_open_file(buf[0], ix_WRITE_ONLY).unwrap();
    files[0].write_string("0");
    files[1] = ix_open_file(buf[1], ix_WRITE_ONLY).unwrap();
    files[1].write_string("1");

    files[0].substitute(files[1].copy());
    ix_EXPECT(files[0].is_valid());
    ix_EXPECT(files[1].is_valid());
    files[0].write_string("0");
    files[1].write_string("1");
    files[0].close();
    files[1].close();

    ix_EXPECT_EQSTR(ix_load_file(buf[0]).unwrap().get(), "0");
    ix_EXPECT_EQSTR(ix_load_file(buf[1]).unwrap().get(), "101");

    ix_EXPECT(ix_remove_file(buf[0]).is_ok());
    ix_EXPECT(ix_remove_file(buf[1]).is_ok());
}
#endif

const ix_FileHandle &ix_FileHandle::of_stdin()
{
    ix_ASSERT_FATAL(g_filehandle_stdin.get().is_valid(), "stdio system is not initialized!");
    return g_filehandle_stdin.get();
}

const ix_FileHandle &ix_FileHandle::of_stdout()
{
    ix_ASSERT_FATAL(g_filehandle_stdout.get().is_valid(), "stdio system is not initialized!");
    return g_filehandle_stdout.get();
}

const ix_FileHandle &ix_FileHandle::of_stderr()
{
    ix_ASSERT_FATAL(g_filehandle_stderr.get().is_valid(), "stdio system is not initialized!");
    return g_filehandle_stderr.get();
}

#if ix_DO_TEST
ix_TEST_CASE("ix_FileHandle: stdio")
{
    ix_EXPECT(ix_FileHandle::of_stdin().is_valid());
    ix_EXPECT(ix_FileHandle::of_stdout().is_valid());
    ix_EXPECT(ix_FileHandle::of_stderr().is_valid());
}
#endif
