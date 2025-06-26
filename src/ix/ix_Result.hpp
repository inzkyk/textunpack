#pragma once

#include "ix.hpp"

class ix_StringView;
class ix_Writer;

template <typename T>
struct ix_Print;

enum [[nodiscard]] ix_ResultKind : uint8_t;

ix_StringView ix_result_kind_to_string(ix_ResultKind kind);
ix_StringView ix_result_kind_to_description(ix_ResultKind kind);

enum ix_ResultKind : uint8_t
{
    ix_OK,
    ix_ERROR_SOMETHING_WENT_WRONG,

    ix_ERROR_ALREADY_INITIALIZED,
    ix_ERROR_BUFFER_TOO_SMALL,
    ix_ERROR_DIR_ACCESS_DENIED,
    ix_ERROR_DIR_ALREADY_EXISTS,
    ix_ERROR_DIR_NOT_FOUND,
    ix_ERROR_FILE_ACCESS_DENIED,
    ix_ERROR_FILE_ALREADY_EXISTS,
    ix_ERROR_FILE_NOT_FOUND,
    ix_ERROR_FILE_READ_FAILED,
    ix_ERROR_FILE_WRITE_FAILED,
    ix_ERROR_INVALID_INPUT,
    ix_ERROR_INVALID_PARAMETER,
    ix_ERROR_INVALID_PATH,
    ix_ERROR_INVALID_UTF16,
    ix_ERROR_INVALID_UTF8,
    ix_ERROR_NOT_FOUND,
    ix_ERROR_NOT_INITIALIZED,
    ix_ERROR_PATH_TOO_LONG,
    ix_ERROR_PROCESS_INVOCATION_FAILED,
};

class [[nodiscard]] ix_Result
{
    ix_ResultKind m_kind;

  public:
    ix_Result() = default;
    ix_Result(ix_ResultKind kind); // cppcheck-suppress noExplicitConstructor; NOLINT
    ix_ResultKind kind() const;
    ix_StringView string() const;
    ix_StringView description() const;
    bool is_ok() const;
    bool is_error() const;
    void assert_ok() const;
};

template <>
struct ix_Print<ix_Result>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_Result result);
};

template <>
struct ix_Print<ix_ResultKind>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, ix_ResultKind kind);
};
