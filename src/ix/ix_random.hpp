#pragma once

#include "ix.hpp"

class ix_Xoshiro256PlusPlus;

namespace ix_global
{
extern ix_Xoshiro256PlusPlus rng;
} // namespace ix_global

// From https://prng.di.unimi.it/xoshiro256plusplus.c
// (by David Blackman and Sebastiano Vigna, public domain)
class ix_Xoshiro256PlusPlus
{
    uint64_t m_state[4];

  public:
    constexpr ix_Xoshiro256PlusPlus(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
        : m_state{a, b, c, d}
    {
    }

    explicit ix_Xoshiro256PlusPlus(uint64_t x);
    void set_random_seed();
    void set_seed(uint64_t seed);

    template <typename T>
    T rand();

    template <typename T>
    T rand_non_negative();

    template <typename T>
    T rand_range(T min, T max);

    void fill_alphanumeric(char *buf, size_t length);

    template <typename T>
    void fill_unique(T *buf, size_t length);

    template <typename T>
    void fill_unique_range(T *buf, size_t length, T min, T max);
};

void ix_rand_set_seed(uint64_t seed);
void ix_rand_set_random_seed();

template <typename T>
ix_FORCE_INLINE T ix_rand()
{
    return ix_global::rng.rand<T>();
}

template <typename T>
ix_FORCE_INLINE T ix_rand_non_negative()
{
    return ix_global::rng.rand_non_negative<T>();
}
template <typename T>
ix_FORCE_INLINE T ix_rand_range(T min, T max)
{
    return ix_global::rng.rand_range(min, max);
}

ix_FORCE_INLINE void ix_rand_fill_alphanumeric(char *buf, size_t length)
{
    ix_global::rng.fill_alphanumeric(buf, length);
}

template <typename T>
ix_FORCE_INLINE void ix_rand_fill_unique(T *buf, size_t length)
{
    return ix_global::rng.fill_unique(buf, length);
}

template <typename T>
void ix_rand_fill_unique_range(T *buf, size_t length, T min, T max)
{
    return ix_global::rng.fill_unique_range(buf, length, min, max);
}

#if ix_PLATFORM(WASM)
template <>
ix_FORCE_INLINE size_t ix_Xoshiro256PlusPlus::rand()
{
    return rand<uint32_t>();
}

template <>
ix_FORCE_INLINE size_t ix_Xoshiro256PlusPlus::rand_non_negative()
{
    return rand<uint32_t>();
}

template <>
ix_FORCE_INLINE size_t ix_Xoshiro256PlusPlus::rand_range(size_t min, size_t max)
{
    return rand_range<uint32_t>(min, max);
}
#endif

// clang-format off
extern template int8_t   ix_Xoshiro256PlusPlus::rand<int8_t>();
extern template int16_t  ix_Xoshiro256PlusPlus::rand<int16_t>();
extern template int32_t  ix_Xoshiro256PlusPlus::rand<int32_t>();
extern template int64_t  ix_Xoshiro256PlusPlus::rand<int64_t>();
extern template uint8_t  ix_Xoshiro256PlusPlus::rand<uint8_t>();
extern template uint16_t ix_Xoshiro256PlusPlus::rand<uint16_t>();
extern template uint32_t ix_Xoshiro256PlusPlus::rand<uint32_t>();
extern template uint64_t ix_Xoshiro256PlusPlus::rand<uint64_t>();
extern template float    ix_Xoshiro256PlusPlus::rand<float>();
extern template double   ix_Xoshiro256PlusPlus::rand<double>();

extern template int8_t   ix_Xoshiro256PlusPlus::rand_non_negative<int8_t>();
extern template int16_t  ix_Xoshiro256PlusPlus::rand_non_negative<int16_t>();
extern template int32_t  ix_Xoshiro256PlusPlus::rand_non_negative<int32_t>();
extern template int64_t  ix_Xoshiro256PlusPlus::rand_non_negative<int64_t>();
extern template uint8_t  ix_Xoshiro256PlusPlus::rand_non_negative<uint8_t>();  // = rand()
extern template uint16_t ix_Xoshiro256PlusPlus::rand_non_negative<uint16_t>(); // = rand()
extern template uint32_t ix_Xoshiro256PlusPlus::rand_non_negative<uint32_t>(); // = rand()
extern template uint64_t ix_Xoshiro256PlusPlus::rand_non_negative<uint64_t>(); // = rand()

extern template int8_t   ix_Xoshiro256PlusPlus::rand_range<int8_t>(int8_t, int8_t);
extern template int16_t  ix_Xoshiro256PlusPlus::rand_range<int16_t>(int16_t, int16_t);
extern template int32_t  ix_Xoshiro256PlusPlus::rand_range<int32_t>(int32_t, int32_t);
extern template int64_t  ix_Xoshiro256PlusPlus::rand_range<int64_t>(int64_t, int64_t);
extern template uint8_t  ix_Xoshiro256PlusPlus::rand_range<uint8_t>(uint8_t, uint8_t);
extern template uint16_t ix_Xoshiro256PlusPlus::rand_range<uint16_t>(uint16_t, uint16_t);
extern template uint32_t ix_Xoshiro256PlusPlus::rand_range<uint32_t>(uint32_t, uint32_t);
extern template uint64_t ix_Xoshiro256PlusPlus::rand_range<uint64_t>(uint64_t, uint64_t);
extern template float    ix_Xoshiro256PlusPlus::rand_range<float>(float, float);
extern template double   ix_Xoshiro256PlusPlus::rand_range<double>(double, double);

extern template void ix_Xoshiro256PlusPlus::fill_unique<int8_t>(int8_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<int16_t>(int16_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<int32_t>(int32_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<int64_t>(int64_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<uint8_t>(uint8_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<uint16_t>(uint16_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<uint32_t>(uint32_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<uint64_t>(uint64_t *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<float>(float *, size_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique<double>(double *, size_t);

extern template void ix_Xoshiro256PlusPlus::fill_unique_range<int8_t>(int8_t *, size_t, int8_t, int8_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<int16_t>(int16_t *, size_t, int16_t, int16_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<int32_t>(int32_t *, size_t, int32_t, int32_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<int64_t>(int64_t *, size_t, int64_t, int64_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<uint8_t>(uint8_t *, size_t, uint8_t, uint8_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<uint16_t>(uint16_t *, size_t, uint16_t, uint16_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<uint32_t>(uint32_t *, size_t, uint32_t, uint32_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<uint64_t>(uint64_t *, size_t, uint64_t, uint64_t);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<float>(float *, size_t, float, float);
extern template void ix_Xoshiro256PlusPlus::fill_unique_range<double>(double *, size_t, double, double);
// clang-format on
