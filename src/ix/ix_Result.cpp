#include "ix_Result.hpp"
#include "ix_Option.hpp"
#include "ix_StringView.hpp"
#include "ix_Writer.hpp"
#include "ix_assert.hpp"

ix_StringView ix_result_kind_to_string(ix_ResultKind kind)
{
#define IMPLEMENT_CASE(e) \
    case e:               \
        return ix_StringView(ix_PP_TO_STRING_LITERAL(e))

    switch (kind)
    {
        IMPLEMENT_CASE(ix_OK);
        IMPLEMENT_CASE(ix_ERROR_SOMETHING_WENT_WRONG);
        IMPLEMENT_CASE(ix_ERROR_ALREADY_INITIALIZED);
        IMPLEMENT_CASE(ix_ERROR_BUFFER_TOO_SMALL);
        IMPLEMENT_CASE(ix_ERROR_DIR_ACCESS_DENIED);
        IMPLEMENT_CASE(ix_ERROR_DIR_ALREADY_EXISTS);
        IMPLEMENT_CASE(ix_ERROR_DIR_NOT_FOUND);
        IMPLEMENT_CASE(ix_ERROR_FILE_ACCESS_DENIED);
        IMPLEMENT_CASE(ix_ERROR_FILE_ALREADY_EXISTS);
        IMPLEMENT_CASE(ix_ERROR_FILE_NOT_FOUND);
        IMPLEMENT_CASE(ix_ERROR_FILE_READ_FAILED);
        IMPLEMENT_CASE(ix_ERROR_FILE_WRITE_FAILED);
        IMPLEMENT_CASE(ix_ERROR_INVALID_INPUT);
        IMPLEMENT_CASE(ix_ERROR_INVALID_PARAMETER);
        IMPLEMENT_CASE(ix_ERROR_INVALID_PATH);
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF16);
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF8);
        IMPLEMENT_CASE(ix_ERROR_NOT_FOUND);
        IMPLEMENT_CASE(ix_ERROR_NOT_INITIALIZED);
        IMPLEMENT_CASE(ix_ERROR_PATH_TOO_LONG);
        IMPLEMENT_CASE(ix_ERROR_PROCESS_INVOCATION_FAILED);
        ix_CASE_EXHAUSTED();
    }

    ix_UNREACHABLE();

#undef IMPLEMENT_CASE
}

ix_StringView ix_result_kind_to_description(ix_ResultKind kind)
{
#define IMPLEMENT_CASE(e, str) \
    case e:                    \
        return ix_StringView(str)

    switch (kind)
    {
        IMPLEMENT_CASE(ix_OK, "OK");
        IMPLEMENT_CASE(ix_ERROR_SOMETHING_WENT_WRONG, "Something went wrong");
        IMPLEMENT_CASE(ix_ERROR_ALREADY_INITIALIZED, "Not initialized");
        IMPLEMENT_CASE(ix_ERROR_BUFFER_TOO_SMALL, "Buffer too small");
        IMPLEMENT_CASE(ix_ERROR_DIR_ACCESS_DENIED, "Directory access denied");
        IMPLEMENT_CASE(ix_ERROR_DIR_ALREADY_EXISTS, "Directory already exists");
        IMPLEMENT_CASE(ix_ERROR_DIR_NOT_FOUND, "Directory not found");
        IMPLEMENT_CASE(ix_ERROR_FILE_ACCESS_DENIED, "File access denied");
        IMPLEMENT_CASE(ix_ERROR_FILE_ALREADY_EXISTS, "File already exists");
        IMPLEMENT_CASE(ix_ERROR_FILE_NOT_FOUND, "File not found");
        IMPLEMENT_CASE(ix_ERROR_FILE_READ_FAILED, "File read failed");
        IMPLEMENT_CASE(ix_ERROR_FILE_WRITE_FAILED, "File write failed");
        IMPLEMENT_CASE(ix_ERROR_INVALID_INPUT, "Invlid input");
        IMPLEMENT_CASE(ix_ERROR_INVALID_PARAMETER, "Invalid parameter");
        IMPLEMENT_CASE(ix_ERROR_INVALID_PATH, "Invalid path");
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF16, "Invalid UTF-16 string");
        IMPLEMENT_CASE(ix_ERROR_INVALID_UTF8, "Invalid UTF-8 string");
        IMPLEMENT_CASE(ix_ERROR_NOT_FOUND, "Not found");
        IMPLEMENT_CASE(ix_ERROR_NOT_INITIALIZED, "Not initialized");
        IMPLEMENT_CASE(ix_ERROR_PATH_TOO_LONG, "Path too long");
        IMPLEMENT_CASE(ix_ERROR_PROCESS_INVOCATION_FAILED, "Failed to invoke a process");
        ix_CASE_EXHAUSTED();
    }

    ix_UNREACHABLE();

#undef IMPLEMENT_CASE
}

ix_Result::ix_Result(ix_ResultKind kind)
    : m_kind(kind)
{
}

ix_ResultKind ix_Result::kind() const
{
    return m_kind;
}

bool ix_Result::is_ok() const
{
    return (m_kind == ix_OK);
}

bool ix_Result::is_error() const
{
    return (m_kind != ix_OK);
}

void ix_Result::assert_ok() const
{
    ix_ASSERT(m_kind == ix_OK);
}

ix_StringView ix_Result::string() const
{
    return ix_result_kind_to_string(m_kind);
}

ix_StringView ix_Result::description() const
{
    return ix_result_kind_to_description(m_kind);
}

size_t ix_Print<ix_Result>::print(ix_Writer &writer, ix_Result result)
{
    const ix_StringView sv = result.string();
    const size_t length = sv.length();
    writer.write(sv.data(), length);
    return length;
}

size_t ix_Print<ix_ResultKind>::print(ix_Writer &writer, ix_ResultKind kind)
{
    const ix_StringView sv = ix_result_kind_to_string(kind);
    const size_t length = sv.length();
    writer.write(sv.data(), length);
    return length;
}

#if ix_DO_TEST
#include "ix_test.hpp"

ix_TEST_CASE("ix_Result")
{
    ix_Result o = ix_ERROR_SOMETHING_WENT_WRONG;
    ix_EXPECT(o.is_error());

    o = ix_OK;
    ix_EXPECT(o.is_ok());
}
#endif
