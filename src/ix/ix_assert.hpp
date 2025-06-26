#pragma once

#include "ix.hpp"

#if ix_COMPILER(MSVC)
#define ix_ASSERT_FATAL(expr, msg) \
    do                             \
    {                              \
        if (ix_UNLIKELY(!(expr)))  \
        {                          \
            __debugbreak();        \
            ix_abort();            \
        }                          \
    } while (0)
#else
#define ix_ASSERT_FATAL(expr, msg)                       \
    do                                                   \
    {                                                    \
        if (ix_UNLIKELY(!(expr)))                        \
        {                                                \
            ix_abort_with_info(__FILE__, __LINE__, msg); \
        }                                                \
    } while (0)
#endif

#if ix_OPT_LEVEL(DEBUG)
#define ix_ASSERT(expr) ix_ASSERT_FATAL((expr), "Assertion failed!")
#else
#define ix_ASSERT(expr) (void)(expr)
#endif

// ix_ASSERT_FATAL without message (for benchmarks)
#define ix_ABORT_IF_NOT(expr)     \
    do                            \
    {                             \
        if (ix_UNLIKELY(!(expr))) \
        {                         \
            ix_abort();           \
        }                         \
    } while (0)
