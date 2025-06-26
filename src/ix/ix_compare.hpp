#pragma once

#include "ix.hpp"
#include "ix_string.hpp"

ix_DISABLE_GCC_WARNING_BEGIN
ix_DISABLE_GCC_WARNING("-Wfloat-equal")
ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wfloat-equal")

template <typename T>
struct ix_Equal
{
    ix_FORCE_INLINE static constexpr bool equal(const T &x, const T &y)
    {
        return (x == y);
    }
};

template <typename T>
ix_FORCE_INLINE constexpr bool ix_equal(const T &x, const T &y)
{
    return (x == y);
}

template <typename T>
ix_FORCE_INLINE constexpr bool ix_not_equal(const T &x, const T &y)
{
    return (x != y);
}

template <typename T>
ix_FORCE_INLINE constexpr bool ix_is_zero(const T &x)
{
    return (x == T{0});
}

ix_DISABLE_GCC_WARNING_END
ix_DISABLE_CLANG_WARNING_END

struct ix_StringEqual
{
    ix_FORCE_INLINE static constexpr bool equal(const char *x, const char *y)
    {
        return (ix_strcmp(x, y) == 0);
    }
};

template <typename T>
struct ix_Compare
{
    ix_FORCE_INLINE static constexpr int8_t compare(const T x, const T y)
    {
        if (x < y)
        {
            return -1;
        }
        if (x > y)
        {
            return 1;
        }
        return 0;
    }
};

template <>
struct ix_Compare<const char *>
{
    ix_FORCE_INLINE static constexpr int8_t compare(const char *x, const char *y)
    {
        return static_cast<int8_t>(ix_strcmp(x, y));
    }
};

template <typename T>
struct ix_Less
{
    ix_FORCE_INLINE constexpr bool operator()(const T &x, const T &y)
    {
        return x < y;
    }
};

template <typename T>
struct ix_Greater
{
    ix_FORCE_INLINE constexpr bool operator()(const T &x, const T &y)
    {
        return x > y;
    }
};
