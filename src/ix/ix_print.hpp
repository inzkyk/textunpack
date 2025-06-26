#pragma once

#include "ix.hpp"

class ix_Writer;

template <typename T>
struct ix_Print
{
    static constexpr bool is_implemented = false;
};

#define ix_DECLARE_PRINT_BY_VALUE(type)                 \
    template <>                                         \
    struct ix_Print<type>                               \
    {                                                   \
        static constexpr bool is_implemented = true;    \
        static size_t print(ix_Writer &writer, type x); \
    }

ix_DECLARE_PRINT_BY_VALUE(char);
ix_DECLARE_PRINT_BY_VALUE(int8_t);
ix_DECLARE_PRINT_BY_VALUE(int16_t);
ix_DECLARE_PRINT_BY_VALUE(int32_t);
ix_DECLARE_PRINT_BY_VALUE(int64_t);
ix_DECLARE_PRINT_BY_VALUE(uint8_t);
ix_DECLARE_PRINT_BY_VALUE(uint16_t);
ix_DECLARE_PRINT_BY_VALUE(uint32_t);
ix_DECLARE_PRINT_BY_VALUE(uint64_t);
ix_DECLARE_PRINT_BY_VALUE(float);
ix_DECLARE_PRINT_BY_VALUE(double);
ix_DECLARE_PRINT_BY_VALUE(void *);
ix_DECLARE_PRINT_BY_VALUE(const void *);
ix_DECLARE_PRINT_BY_VALUE(decltype(nullptr));

#undef ix_DECLARE_PRINT_BY_VALUE

template <typename T>
struct ix_Print<T *>
{
    static constexpr bool is_implemented = true;

    ix_FORCE_INLINE static size_t print(ix_Writer &writer, T *x)
    {
        return ix_Print<const void *>::print(writer, reinterpret_cast<void *>(x));
    }
};

template <typename T>
struct ix_Print<const T *>
{
    static constexpr bool is_implemented = true;

    ix_FORCE_INLINE static size_t print(ix_Writer &writer, const T *x)
    {
        return ix_Print<const void *>::print(writer, reinterpret_cast<const void *>(x));
    }
};
