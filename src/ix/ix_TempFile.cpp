#include "ix_TempFile.hpp"
#include "ix_Buffer.hpp"
#include "ix_Option.hpp"
#include "ix_file.hpp"
#include "ix_memory.hpp"
#include "ix_random.hpp"
#include "ix_string.hpp"
#include "ix_util_win.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#endif

// #include <fcntl.h>

#if ix_PLATFORM(WIN)
#include "ix_Windows.hpp"

#include <fileapi.h>
#endif

constexpr const size_t ix_TEMP_FILENAME_RANDOM_PART_LENGTH = 16;

const char *ix_temp_dirname()
{
#if ix_PLATFORM(WIN)
    static char path_utf8[ix_MAX_PATH + 1] = {};

    const bool already_calcuated = path_utf8[0] != '\0';
    if (ix_LIKELY(already_calcuated))
    {
        return path_utf8;
    }

    wchar_t path_wchar[ix_MAX_PATH + 1];

    const DWORD ret = GetTempPath(ix_LENGTH_OF(path_wchar), path_wchar);
#if ix_MEASURE_COVERAGE
    ix_UNUSED(ret);
#else
    if (ret == 0)
    {
        ix_log_debug("GetTempPath() failed.");
        return nullptr;
    }
#endif

    const ix_Option<size_t> length = wchar_path_to_utf8(path_wchar, ret, path_utf8);
    length.assert_ok();

    return path_utf8;
#else
    return "/tmp/";
#endif
}

size_t ix_temp_dirname_length()
{
#if ix_PLATFORM(WIN)
    static const size_t length = ix_strlen(ix_temp_dirname());
    return length;
#else
    return ix_strlen("/tmp/");
#endif
}

#if ix_DO_TEST
ix_TEST_CASE("ix_temp_file_dirname")
{
    ix_EXPECT_EQSTR(ix_temp_dirname(), ix_temp_dirname());
    ix_EXPECT_EQSTR(ix_temp_dirname(), ix_temp_dirname());
    ix_EXPECT_EQSTR(ix_temp_dirname(), ix_temp_dirname());
    ix_EXPECT_EQSTR(ix_temp_dirname(), ix_temp_dirname());
    ix_EXPECT_EQSTR(ix_temp_dirname(), ix_temp_dirname());
}

ix_TEST_CASE("ix_temp_file_dirname_length")
{
    ix_EXPECT(ix_temp_dirname_length() > 0);
    ix_EXPECT(ix_temp_dirname_length() == ix_temp_dirname_length());
    ix_EXPECT(ix_temp_dirname()[ix_temp_dirname_length()] == '\0');
    ix_EXPECT(ix_temp_dirname()[ix_temp_dirname_length()] == '\0');
    ix_EXPECT(ix_temp_dirname()[ix_temp_dirname_length()] == '\0');
}
#endif

const char *ix_temp_filename(char *output_buffer, size_t output_buffer_length, const char *prefix)
{
    if (output_buffer_length < ix_temp_filename_length(prefix) + 1)
    {
        return nullptr;
    }

    static const size_t temp_dirname_length = ix_temp_dirname_length();
    const char *temp_file_dirname = ix_temp_dirname();
    // temp_dirname_length = ix_strlen(temp_file_dirname);
    ix_memcpy(output_buffer, temp_file_dirname, temp_dirname_length);

    const size_t prefix_length = ix_strlen(prefix);

    const size_t temp_filename_length = temp_dirname_length + prefix_length + ix_TEMP_FILENAME_RANDOM_PART_LENGTH;
#if ix_MEASURE_COVERAGE
    ix_UNUSED(temp_filename_length);
#else
    if (temp_filename_length >= ix_MAX_PATH)
    {
        ix_log_debug("ix_temp_filename: too long prefix.");
        return nullptr;
    }
#endif

    ix_memcpy(output_buffer + temp_dirname_length, prefix, prefix_length);
    ix_rand_fill_alphanumeric(
        output_buffer + temp_dirname_length + prefix_length,
        ix_TEMP_FILENAME_RANDOM_PART_LENGTH
    );

    output_buffer[temp_filename_length] = '\0';
    return output_buffer;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_temp_filename")
{
    char buf_a[128];
    char buf_b[128];

    for (size_t i = 0; i < 10; i++)
    {
        const char *a = ix_temp_filename(buf_a, ix_LENGTH_OF(buf_a));
        const char *b = ix_temp_filename(buf_b, ix_LENGTH_OF(buf_b));
        ix_EXPECT_EQ(a[ix_temp_filename_length()], '\0');
        ix_EXPECT_EQ(b[ix_temp_filename_length()], '\0');
        ix_EXPECT(ix_strcmp(a, b) != 0);
    }

    for (size_t i = 0; i < 10; i++)
    {
        const char *a = ix_temp_filename(buf_a, ix_LENGTH_OF(buf_a), "foo_");
        const char *b = ix_temp_filename(buf_b, ix_LENGTH_OF(buf_b), "foo_");
        ix_EXPECT_EQ(a[ix_temp_filename_length("foo_")], '\0');
        ix_EXPECT_EQ(b[ix_temp_filename_length("foo_")], '\0');
        ix_EXPECT(ix_strcmp(a, b) != 0);
    }

    for (size_t i = 0; i < 10; i++)
    {
        const char *a = ix_temp_filename(buf_a, ix_LENGTH_OF(buf_a), "一時ファイル_");
        const char *b = ix_temp_filename(buf_b, ix_LENGTH_OF(buf_b), "一時ファイル_");
        ix_EXPECT_EQ(a[ix_temp_filename_length("一時ファイル_")], '\0');
        ix_EXPECT_EQ(b[ix_temp_filename_length("一時ファイル_")], '\0');
        ix_EXPECT(ix_strcmp(a, b) != 0);
    }
}
#endif

size_t ix_temp_filename_length(const char *prefix)
{
    const size_t prefix_length = ix_strlen(prefix);
    return ix_temp_dirname_length() + prefix_length + ix_TEMP_FILENAME_RANDOM_PART_LENGTH;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_temp_filename_length")
{
    char buf[64];

    ix_EXPECT(ix_temp_filename_length() > 0);
    ix_EXPECT(ix_temp_filename_length() == ix_temp_filename_length());
    ix_EXPECT(ix_temp_filename(buf, ix_LENGTH_OF(buf))[ix_temp_filename_length()] == '\0');
    ix_EXPECT(ix_temp_filename(buf, ix_LENGTH_OF(buf))[ix_temp_filename_length()] == '\0');

    const char *prefix = "temporary_";
    ix_EXPECT(ix_temp_filename_length(prefix) > 0);
    ix_EXPECT(ix_temp_filename_length(prefix) == ix_temp_filename_length(prefix));
    ix_EXPECT(ix_temp_filename(buf, ix_LENGTH_OF(buf), prefix)[ix_temp_filename_length(prefix)] == '\0');
    ix_EXPECT(ix_temp_filename(buf, ix_LENGTH_OF(buf), prefix)[ix_temp_filename_length(prefix)] == '\0');
}
#endif

ix_TempFile::~ix_TempFile()
{
    const ix_Result result = ix_remove_file(m_filename);
    if (result.is_error())
    {
        ix_log_warning("ix_TempFile: Failed to remove \"%s\"", m_filename);
    }
    ix_FREE(m_filename);
}

ix_TempFile::ix_TempFile(const char *str, size_t str_length)
{
    const size_t filename_length = ix_temp_filename_length();
    m_filename = ix_MALLOC(char *, filename_length + 1);
    ix_temp_filename(m_filename, filename_length + 1);

    const ix_FileHandle file = ix_open_file(m_filename, ix_WRITE_ONLY).unwrap();
    ix_ASSERT(file.is_valid());
    file.write(str, str_length);
}

ix_TempFile::ix_TempFile()
    : ix_TempFile(nullptr, 0)
{
}

ix_TempFile::ix_TempFile(const char *str)
    : ix_TempFile(str, ix_strlen(str))
{
}

const char *ix_TempFile::filename() const
{
    return m_filename;
}

ix_TempFileR::~ix_TempFileR()
{
    if (m_filename == nullptr)
    {
        ix_ASSERT(!m_file.is_valid());
        return;
    }

    m_file.close();
    const ix_Result result = ix_remove_file(m_filename);
    if (result.is_error())
    {
        ix_log_warning("ix_TempFileR: Failed to remove \"%s\"", m_filename);
    }
    ix_FREE(m_filename);
}

ix_TempFileR::ix_TempFileR(const char *str)
    : ix_TempFileR(str, (str == nullptr) ? 0 : ix_strlen(str))
{
}

ix_TempFileR::ix_TempFileR(const char *str, size_t str_length)
    : m_filename(nullptr)
{
    if (str == nullptr)
    {
        return;
    }

    const size_t filename_length = ix_temp_filename_length();
    m_filename = ix_MALLOC(char *, filename_length + 1);
    ix_temp_filename(m_filename, filename_length + 1);

    m_file = ix_open_file(m_filename, ix_WRITE_ONLY).unwrap();
    m_file.write(str, str_length);
    m_file.close();

    m_file = ix_open_file(m_filename, ix_READ_ONLY).unwrap();
    ix_ASSERT(m_file.is_valid());
}

const ix_FileHandle &ix_TempFileR::file_handle() const
{
    return m_file;
}

const char *ix_TempFileR::filename() const
{
    return m_filename;
}

ix_TempFileW::~ix_TempFileW()
{
    if (m_file.is_valid())
    {
        m_file.close();
    }

    const ix_Result result = ix_remove_file(m_filename);
    if (result.is_error())
    {
        ix_log_warning("ix_TempFileW: Failed to remove \"%s\"", m_filename);
    }
    ix_FREE(m_filename);
}

ix_TempFileW::ix_TempFileW()
    : m_filename(nullptr),
      m_data(nullptr)
{
    const size_t filename_length = ix_temp_filename_length();
    m_filename = ix_MALLOC(char *, filename_length + 1);
    ix_temp_filename(m_filename, filename_length + 1);

    m_file = ix_open_file(m_filename, ix_WRITE_ONLY).unwrap();
    ix_ASSERT(m_file.is_valid());
}

const ix_FileHandle &ix_TempFileW::file_handle() const
{
    return m_file;
}

const char *ix_TempFileW::filename() const
{
    return m_filename;
}

void ix_TempFileW::close()
{
    if (m_file.is_valid())
    {
        m_file.close();
    }
}

char *ix_TempFileW::data()
{
    if (m_data.get() == nullptr)
    {
        close();
        ix_Buffer buffer;
        buffer.load_file(m_filename).unwrap(); // TODO: remove unwrap(?)
        m_data = buffer.detach();
    }
    return m_data.get();
}

void ix_TempFileW::reset()
{
    ix_ASSERT(m_filename != nullptr);

    // Destruct
    close();
    const ix_Result result = ix_remove_file(m_filename);
    if (result.is_error())
    {
        ix_log_warning("ix_TempFileW: Failed to remove \"%s\"", m_filename);
    }

    // Construct
    m_data.swap(nullptr);

    const size_t filename_length = ix_temp_filename_length();
    ix_temp_filename(m_filename, filename_length + 1);

    m_file = ix_open_file(m_filename, ix_WRITE_ONLY).unwrap();
    ix_ASSERT(m_file.is_valid());
}

#if ix_DO_TEST

ix_TEST_CASE("ix_TempFile")
{
    {
        const ix_TempFile temp;
        ix_EXPECT(temp.filename() != nullptr);
        ix_EXPECT_EQSTR(ix_Buffer::from_file(temp.filename()).data(), "");
    }

    {
        const char *msg = "hello world";
        const ix_TempFile temp(msg);
        ix_EXPECT(temp.filename() != nullptr);
        ix_EXPECT_EQSTR(ix_Buffer::from_file(temp.filename()).data(), msg);
    }

    {
        const char *msg = "hello world";
        const ix_TempFile temp(msg, 5);
        ix_EXPECT(temp.filename() != nullptr);
        ix_EXPECT_EQSTR(ix_Buffer::from_file(temp.filename()).data(), "hello");
    }
}

ix_TEST_CASE("ix_TempFileR")
{
    const char *s = "hello world!";
    ix_UniquePointer<char[]> content(nullptr);

    // Constructor with a string.
    content = ix_TempFileR(s).file_handle().read_all(nullptr).unwrap();
    ix_EXPECT_EQSTR(content.get(), s);

    // Constructor with a pointer and a length.
    content = ix_TempFileR(s, 5).file_handle().read_all(nullptr).unwrap();
    ix_EXPECT_EQSTR(content.get(), "hello");
}

ix_TEST_CASE("ix_TempFileW")
{
    { // Do nothing.
        const ix_TempFileW w;
        ix_EXPECT(w.file_handle().is_valid());
        ix_EXPECT(w.filename() != nullptr);
    }

    { // Write and don't read.
        const ix_TempFileW w;
        const char *s = "hello world!";
        w.file_handle().write_string(s);
    }

    { // Write and data().
        ix_TempFileW w;
        const char *s = "hello world!";
        w.file_handle().write_string(s);
        ix_EXPECT_EQSTR(w.data(), s);
    }

    { // Write, close(), and load with filename().
        ix_TempFileW w;
        const char *s = "hello world!";
        w.file_handle().write_string(s);
        w.close();
        ix_UniquePointer<char[]> content = ix_load_file(w.filename()).unwrap();
        ix_EXPECT_EQSTR(content.get(), s);
    }

    { // Call data() multiple times.
        ix_TempFileW w;
        const char *s = "hello world!";
        w.file_handle().write_string(s);
        ix_EXPECT_EQSTR(w.data(), s);
        ix_EXPECT_EQSTR(w.data(), s);
        ix_EXPECT_EQSTR(w.data(), s);
    }

    { // reset()
        ix_TempFileW w;
        const char *s = "hello world!";
        w.file_handle().write_string(s);
        ix_EXPECT_EQSTR(w.data(), s);

        w.reset();
        s = "bye world!";
        w.file_handle().write_string(s);
        ix_EXPECT_EQSTR(w.data(), s);
    }
}
#endif
