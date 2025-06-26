#pragma once

#include "ix.hpp"

#if ix_PLATFORM(WIN)

template <typename T>
class ix_Vector;

ix_Option<size_t> ix_win_utf8_to_wchar(
    const char *utf8_buf,
    size_t utf8_buf_len,
    wchar_t *wchar_buf,
    size_t wchar_buf_len
);

ix_Option<size_t> ix_win_wchar_to_utf8(
    const wchar_t *wchar_buf,
    size_t wchar_buf_len,
    char *utf8_buf,
    size_t utf8_buf_len
);

ix_Result ix_win_utf8_to_wchar_dynamic(const char *utf8_buf, size_t utf8_buf_len, ix_Vector<wchar_t> &vector);
ix_Result ix_win_wchar_to_utf8_dynamic(const wchar_t *wchar_buf, size_t wchar_buf_len, ix_Vector<char> &vector);
ix_Option<size_t> utf8_path_to_wchar(const char *path_utf8, wchar_t path_wchar[ix_MAX_PATH + 1]);
ix_Option<size_t> wchar_path_to_utf8(const wchar_t *path_wchar, size_t path_length, char path_utf8[ix_MAX_PATH + 1]);

ix_Option<ix_UniquePointer<const char *[]>> ix_win_wargs_to_args(
    int argc,
    const wchar_t *const *wargv,
    ix_Vector<char> &buf
);

const char *ix_win_msg_to_string(unsigned int uMsg);

#if ix_MEASURE_COVERAGE
void ix_win_abort_with_info(const char *file, int line, const char *msg);
#else
[[noreturn]] void ix_win_abort_with_info(const char *file, int line, const char *msg);
#endif

#if ix_OPT_LEVEL(DEBUG)
#define ix_WIN_ASSERT(expr)                                                            \
    do                                                                                 \
    {                                                                                  \
        if (ix_UNLIKELY(!(expr)))                                                      \
        {                                                                              \
            ix_win_abort_with_info(__FILE__, __LINE__, ix_PP_TO_STRING_LITERAL(expr)); \
        }                                                                              \
    } while (0)
#define ix_WIN_ASSERT_NONZERO(expr) ix_WIN_ASSERT((expr) != 0)
#define ix_WIN_CHECK_HR(hr) ix_WIN_ASSERT((hr) == S_OK)
#else
#define ix_WIN_ASSERT(expr) ix_UNUSED(expr)
#define ix_WIN_ASSERT_NONZERO(expr) ix_UNUSED(expr)
#define ix_WIN_CHECK_HR(hr) ix_UNUSED(hr)
#endif

#endif
