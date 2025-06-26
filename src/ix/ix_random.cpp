#include "ix_random.hpp"
#include "ix_HashSet.hpp"
#include "ix_assert.hpp"
#include "ix_combinatorics.hpp"
#include "ix_environment.hpp"
#include "ix_hash.hpp"
#include "ix_limits.hpp"
#include "ix_type_traits.hpp"
#include "ix_wrap.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#endif

#include <time.h>

namespace ix_global
{
ix_Xoshiro256PlusPlus rng(ix_hash64(91), ix_hash64(96), ix_hash64(60), ix_hash64(70));
} // namespace ix_global

ix_Xoshiro256PlusPlus::ix_Xoshiro256PlusPlus(uint64_t x)
    : m_state()
{
    set_seed(x);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_Xoshiro256PlusPlus: constructor")
{
    ix_Xoshiro256PlusPlus rng(0, 1, 2, 3);
    ix_EXPECT(rng.rand<int>() != 0);
}
#endif

void ix_Xoshiro256PlusPlus::set_random_seed()
{
    const uint64_t t =
        0xDeadBeefDeadBeefULL ^ static_cast<uint64_t>(time(nullptr)) ^ ix_process_id() ^ ix_thread_id();
    set_seed(t);
}

void ix_Xoshiro256PlusPlus::set_seed(uint64_t seed)
{
    // linear congruential generator
    constexpr uint64_t m = 1ULL << 31;
    constexpr uint64_t a = 1664525;
    constexpr uint64_t c = 1013904223;
    uint64_t x = seed;
    for (uint64_t i = 0; i < ix_LENGTH_OF(m_state); i++)
    {
        x = (a * x + c) % m;
        const uint64_t s1 = x;
        x = (a * x + c) % m;
        const uint64_t s2 = x;

        m_state[i] = (s1 << 32UL) | s2;
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand_set_seed")
{
    ix_rand_set_seed(42);
    ix_EXPECT_EQ(ix_rand<uint64_t>(), 6968232877874539080ULL);
    ix_EXPECT_EQ(ix_rand<uint64_t>(), 7567020837360480200ULL);
    ix_EXPECT_EQ(ix_rand<uint64_t>(), 1885804526265309061ULL);
    ix_EXPECT_EQ(ix_rand<uint64_t>(), 5413171059772814680ULL);
    ix_EXPECT_EQ(ix_rand<uint64_t>(), 14121810022092433096ULL);
    ix_rand_set_random_seed();
}
#endif

void ix_rand_set_seed(uint64_t seed)
{
    ix_global::rng.set_seed(seed);
}

void ix_rand_set_random_seed()
{
    ix_global::rng.set_random_seed();
}

// From https://prng.di.unimi.it/xoshiro256starstar.c
// (by David Blackman and Sebastiano Vigna, public domain)
static uint64_t rotl(const uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

template <typename T>
T ix_Xoshiro256PlusPlus::rand()
{
    if constexpr (ix_is_same_v<T, uint64_t>)
    {
        const uint64_t result = rotl(m_state[0] + m_state[3], 23) + m_state[0];

        const uint64_t t = m_state[1] << 17;

        m_state[2] ^= m_state[0];
        m_state[3] ^= m_state[1];
        m_state[1] ^= m_state[2];
        m_state[0] ^= m_state[3];

        m_state[2] ^= t;

        m_state[3] = rotl(m_state[3], 45);

        return result;
    }
    else if constexpr (ix_is_same_v<T, float>)
    {
        // cf. https://stackoverflow.com/questions/52147419
        const uint32_t v = rand<uint32_t>();
        constexpr uint32_t mask1 = 0x3F800000UL;
        constexpr uint32_t mask2 = 0x3FFFFFFFUL;
        const uint32_t masked = (v | mask1) & mask2;
        float one_to_two;
        ix_memcpy(&one_to_two, &masked, sizeof(float));
        return one_to_two - 1.0F;
    }
    else if constexpr (ix_is_same_v<T, double>)
    {
        // cf. https://stackoverflow.com/questions/52147419
        const uint64_t v = rand<uint64_t>();
        constexpr uint64_t mask1 = 0x3FF0000000000000ULL;
        constexpr uint64_t mask2 = 0x3FFFFFFFFFFFFFFFULL;
        const uint64_t masked = (v | mask1) & mask2;
        double one_to_two;
        ix_memcpy(&one_to_two, &masked, sizeof(double));
        return one_to_two - 1.0;
    }
    else
    {
        T x;
        uint64_t v = rand<uint64_t>();
        ix_memcpy(&x, &v, sizeof(T));
        return x;
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand")
{
    ix_rand_set_random_seed();
    for (uint64_t i = 0; i < 25; i++)
    {
        ix_rand<int8_t>();
        ix_rand<int16_t>();
        ix_rand<int32_t>();
        ix_rand<int64_t>();
        ix_rand<uint8_t>();
        ix_rand<uint16_t>();
        ix_rand<uint32_t>();
        ix_rand<uint64_t>();

        ix_EXPECT(0.0F <= ix_rand<float>());
        ix_EXPECT(ix_rand<float>() < 1.0F);

        ix_EXPECT(0.0 <= ix_rand<double>());
        ix_EXPECT(ix_rand<double>() < 1.0);
    }
}
#endif

template <typename T>
T ix_Xoshiro256PlusPlus::rand_non_negative()
{
    if constexpr (ix_numeric_limits<T>::is_signed)
    {
        while (true)
        {
            T x = rand<T>();
            x = (x < 0) ? -x : x;
            if (ix_LIKELY(x >= 0))
            {
                return x;
            }
            ix_ASSERT(x == ix_numeric_limits<T>::min());
        }
    }
    else
    {
        return rand<T>();
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand_non_negative")
{
    for (size_t i = 0; i < 25; i++)
    {
        ix_EXPECT(0 <= ix_rand_non_negative<int8_t>());
        ix_EXPECT(0 <= ix_rand_non_negative<int16_t>());
        ix_EXPECT(0 <= ix_rand_non_negative<int32_t>());
        ix_EXPECT(0 <= ix_rand_non_negative<int64_t>());
    }

    for (size_t i = 0; i < 5000; i++)
    {
        ix_EXPECT(0 <= ix_rand_non_negative<int8_t>());
    }
}
#endif

template <typename T>
T ix_Xoshiro256PlusPlus::rand_range(T min, T max)
{
    ix_ASSERT(min <= max);
    if constexpr (ix_numeric_limits<T>::is_integer && !ix_numeric_limits<T>::is_signed)
    {
        const T x = rand_non_negative<T>();
        const bool whole_range = (min == ix_numeric_limits<T>::min()) && (max == ix_numeric_limits<T>::max());
        if (whole_range)
        {
            return x;
        }
        const T range = static_cast<T>(max - min) + 1;
        return (x % range) + min;
    }
    else if constexpr (ix_numeric_limits<T>::is_integer)
    {
        const bool range_representable = ix_can_sub<T>(max, min) && ix_can_add<T>(max - min, 1);
        if (ix_LIKELY(range_representable))
        {
            const T range = static_cast<T>(max - min) + 1;
            return static_cast<T>(rand_non_negative<T>() % range) + min;
        }

        // `max - min + 1` is not representable in T.
        // This means that there is at least 1/2 chance to get the right value by `rand<T>`.
        T x = rand<T>();
        while (x < min || max < x)
        {
            x = rand<T>();
        }
        return x;
    }
    else
    {
        const T range = max - min;
        const T x = rand<T>() * range;
        return x + min;
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand_range")
{
    for (uint64_t i = 0; i < 25; i++)
    {
        ix_EXPECT(-2 <= ix_rand_range<int8_t>(-2, 2));
        ix_EXPECT(-2 <= ix_rand_range<int16_t>(-2, 2));
        ix_EXPECT(-2 <= ix_rand_range<int32_t>(-2, 2));
        ix_EXPECT(-2 <= ix_rand_range<int64_t>(-2, 2));
        ix_EXPECT(ix_rand_range<int8_t>(-2, 2) <= 2);
        ix_EXPECT(ix_rand_range<int16_t>(-2, 2) <= 2);
        ix_EXPECT(ix_rand_range<int32_t>(-2, 2) <= 2);
        ix_EXPECT(ix_rand_range<int64_t>(-2, 2) <= 2);

        ix_EXPECT(2 <= ix_rand_range<uint8_t>(2, 4));
        ix_EXPECT(2 <= ix_rand_range<uint16_t>(2, 4));
        ix_EXPECT(2 <= ix_rand_range<uint32_t>(2, 4));
        ix_EXPECT(2 <= ix_rand_range<uint64_t>(2, 4));
        ix_EXPECT(ix_rand_range<uint8_t>(2, 4) <= 4);
        ix_EXPECT(ix_rand_range<uint16_t>(2, 4) <= 4);
        ix_EXPECT(ix_rand_range<uint32_t>(2, 4) <= 4);
        ix_EXPECT(ix_rand_range<uint64_t>(2, 4) <= 4);

        ix_EXPECT(-2.0F <= ix_rand_range<float>(-2.0F, 2.0F));
        ix_EXPECT(ix_rand_range<float>(-2.0F, 2.0F) < 2.0F);

        ix_EXPECT(-2.0 <= ix_rand_range<double>(-2.0, 2.0));
        ix_EXPECT(ix_rand_range<double>(-2.0, 2.0) < 2.0);
    }

    ix_EXPECT(ix_rand_range<int8_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<int16_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<int32_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<int64_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<uint8_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<uint16_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<uint32_t>(2, 2) == 2);
    ix_EXPECT(ix_rand_range<uint64_t>(2, 2) == 2);
    ix_EXPECT_EQF(ix_rand_range<float>(2.0F, 2.0F), 2.0F);
    ix_EXPECT_EQD(ix_rand_range<double>(2.0, 2.0), 2.0);

    ix_EXPECT(ix_rand_range<int8_t>(0, ix_numeric_limits<int8_t>::max()) >= 0);
    ix_EXPECT(ix_rand_range<int16_t>(0, ix_numeric_limits<int16_t>::max()) >= 0);
    ix_EXPECT(ix_rand_range<int32_t>(0, ix_numeric_limits<int32_t>::max()) >= 0);
    ix_EXPECT(ix_rand_range<int64_t>(0, ix_numeric_limits<int64_t>::max()) >= 0);

    ix_rand_range<int8_t>(ix_numeric_limits<int8_t>::min(), ix_numeric_limits<int8_t>::max());
    ix_rand_range<int16_t>(ix_numeric_limits<int16_t>::min(), ix_numeric_limits<int16_t>::max());
    ix_rand_range<int32_t>(ix_numeric_limits<int32_t>::min(), ix_numeric_limits<int32_t>::max());
    ix_rand_range<int64_t>(ix_numeric_limits<int64_t>::min(), ix_numeric_limits<int64_t>::max());
}
#endif

void ix_Xoshiro256PlusPlus::fill_alphanumeric(char *buf, size_t length)
{
    constexpr const char alphanumric[] = "abndefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length; i++)
    {
        const uint8_t idx = rand<uint8_t>() % (ix_LENGTH_OF(alphanumric) - 1);
        buf[i] = alphanumric[idx];
    }
    buf[length] = '\0';
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand_fill_alphanumeric")
{
    char buf[10 + 1];
    ix_rand_fill_alphanumeric(buf, 10);
    ix_EXPECT(ix_strlen(buf) == 10);

    ix_rand_fill_alphanumeric(buf, 5);
    ix_EXPECT(ix_strlen(buf) == 5);

    ix_rand_fill_alphanumeric(buf, 0);
    ix_EXPECT(ix_strlen(buf) == 0);
}
#endif

template <typename T>
void ix_Xoshiro256PlusPlus::fill_unique(T *buf, size_t length)
{
    ix_HashSet<T> values;
    values.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        T candidate = rand<T>();
        while (ix_UNLIKELY(values.contains(candidate)))
        {
            candidate = rand<T>();
        }
        values.insert(candidate);
        buf[i] = candidate;
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand_fill_unique")
{
    {
        constexpr size_t N = 100;
        uint64_t buf[N];
        ix_rand_fill_unique(buf, N);
        ix_EXPECT(ix_is_unique(buf, N));
    }

    {
        constexpr size_t N = 256;
        uint8_t buf[N];
        ix_rand_fill_unique(buf, N);
        ix_is_unique(buf, N);
        ix_EXPECT(ix_is_unique(buf, N));
    }
}
#endif

template <typename T>
void ix_Xoshiro256PlusPlus::fill_unique_range(T *buf, size_t length, T min, T max)
{
    ix_ASSERT(min < max);
    if constexpr (ix_numeric_limits<T>::is_integer)
    {
        const size_t num_options = static_cast<size_t>(max) - static_cast<size_t>(min) + size_t{1};
        ix_ASSERT(length <= num_options);

        const bool use_direct_pick = (num_options <= 2 * length);
        if (use_direct_pick)
        {
            ix_Vector<T> options;
            options.reserve(num_options);
            T x = min;
            for (size_t i = 0; i < num_options; i++)
            {
                options.emplace_back(x);
                x += 1;
            }

            for (size_t i = 0; i < length; i++)
            {
                const size_t index = rand<size_t>() % (num_options - i);
                buf[i] = options[index];
                options.swap_erase(index);
            }
            return;
        }
    }

    ix_HashSet<T> values;
    values.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        T candidate = rand_range(min, max);
        while (ix_UNLIKELY(values.contains(candidate)))
        {
            candidate = rand_range(min, max);
        }
        values.insert(candidate);
        buf[i] = candidate;
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_rand_fill_unique_range")
{
    for (size_t _ = 0; _ < 10; _++)
    {
        constexpr size_t N = 100;
        constexpr uint64_t min = 200;
        constexpr uint64_t max = 20000;
        uint64_t buf[N];
        ix_rand_fill_unique_range<uint64_t>(buf, N, min, max);
        ix_EXPECT(ix_is_unique(buf, N));
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(min <= buf[i]);
            ix_EXPECT(buf[i] <= max);
        }
    }

    for (size_t _ = 0; _ < 10; _++)
    {
        constexpr size_t N = 127;
        constexpr uint8_t min = 10;
        constexpr uint8_t max = 255;
        uint8_t buf[N];
        ix_rand_fill_unique_range<uint8_t>(buf, N, min, max);
        ix_EXPECT(ix_is_unique(buf, N));
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(min <= buf[i]);
            ix_EXPECT(buf[i] <= max);
        }
    }

    for (size_t _ = 0; _ < 10; _++)
    {
        constexpr size_t N = 10;
        constexpr uint8_t min = 11;
        constexpr uint8_t max = 20;
        uint8_t buf[N];
        ix_rand_fill_unique_range<uint8_t>(buf, N, min, max);
        ix_EXPECT(ix_is_unique(buf, N));
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(min <= buf[i]);
            ix_EXPECT(buf[i] <= max);
        }
    }

    {
        constexpr size_t N = 100;
        constexpr uint64_t min = 11;
        constexpr uint64_t max = 110;
        uint64_t buf[N];
        ix_rand_fill_unique_range<uint64_t>(buf, N, min, max);
        ix_EXPECT(ix_is_unique(buf, N));
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(min <= buf[i]);
            ix_EXPECT(buf[i] <= max);
        }
    }

    for (size_t _ = 0; _ < 10; _++)
    {
        constexpr size_t N = 100;
        constexpr float min = 10.0F;
        constexpr float max = 100.0F;
        float buf[N];
        ix_rand_fill_unique_range<float>(buf, N, min, max);
        ix_EXPECT(ix_is_unique(buf, N));
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(min <= buf[i]);
            ix_EXPECT(buf[i] <= max);
        }
    }
}
#endif

// clang-format off
template int8_t   ix_Xoshiro256PlusPlus::rand<int8_t>();
template int16_t  ix_Xoshiro256PlusPlus::rand<int16_t>();
template int32_t  ix_Xoshiro256PlusPlus::rand<int32_t>();
template int64_t  ix_Xoshiro256PlusPlus::rand<int64_t>();
template uint8_t  ix_Xoshiro256PlusPlus::rand<uint8_t>();
template uint16_t ix_Xoshiro256PlusPlus::rand<uint16_t>();
template uint32_t ix_Xoshiro256PlusPlus::rand<uint32_t>();
template uint64_t ix_Xoshiro256PlusPlus::rand<uint64_t>();
template float    ix_Xoshiro256PlusPlus::rand<float>();
template double   ix_Xoshiro256PlusPlus::rand<double>();

template int8_t   ix_Xoshiro256PlusPlus::rand_non_negative<int8_t>();
template int16_t  ix_Xoshiro256PlusPlus::rand_non_negative<int16_t>();
template int32_t  ix_Xoshiro256PlusPlus::rand_non_negative<int32_t>();
template int64_t  ix_Xoshiro256PlusPlus::rand_non_negative<int64_t>();
template uint8_t  ix_Xoshiro256PlusPlus::rand_non_negative<uint8_t>();
template uint16_t ix_Xoshiro256PlusPlus::rand_non_negative<uint16_t>();
template uint32_t ix_Xoshiro256PlusPlus::rand_non_negative<uint32_t>();
template uint64_t ix_Xoshiro256PlusPlus::rand_non_negative<uint64_t>();

template int8_t   ix_Xoshiro256PlusPlus::rand_range<int8_t>(int8_t, int8_t);
template int16_t  ix_Xoshiro256PlusPlus::rand_range<int16_t>(int16_t, int16_t);
template int32_t  ix_Xoshiro256PlusPlus::rand_range<int32_t>(int32_t, int32_t);
template int64_t  ix_Xoshiro256PlusPlus::rand_range<int64_t>(int64_t, int64_t);
template uint8_t  ix_Xoshiro256PlusPlus::rand_range<uint8_t>(uint8_t, uint8_t);
template uint16_t ix_Xoshiro256PlusPlus::rand_range<uint16_t>(uint16_t, uint16_t);
template uint32_t ix_Xoshiro256PlusPlus::rand_range<uint32_t>(uint32_t, uint32_t);
template uint64_t ix_Xoshiro256PlusPlus::rand_range<uint64_t>(uint64_t, uint64_t);
template float    ix_Xoshiro256PlusPlus::rand_range<float>(float, float);
template double   ix_Xoshiro256PlusPlus::rand_range<double>(double, double);

template void ix_Xoshiro256PlusPlus::fill_unique<int8_t>(int8_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<int16_t>(int16_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<int32_t>(int32_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<int64_t>(int64_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<uint8_t>(uint8_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<uint16_t>(uint16_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<uint32_t>(uint32_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<uint64_t>(uint64_t *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<float>(float *, size_t);
template void ix_Xoshiro256PlusPlus::fill_unique<double>(double *, size_t);

template void ix_Xoshiro256PlusPlus::fill_unique_range<int8_t>(int8_t *, size_t, int8_t, int8_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<int16_t>(int16_t *, size_t, int16_t, int16_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<int32_t>(int32_t *, size_t, int32_t, int32_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<int64_t>(int64_t *, size_t, int64_t, int64_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<uint8_t>(uint8_t *, size_t, uint8_t, uint8_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<uint16_t>(uint16_t *, size_t, uint16_t, uint16_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<uint32_t>(uint32_t *, size_t, uint32_t, uint32_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<uint64_t>(uint64_t *, size_t, uint64_t, uint64_t);
template void ix_Xoshiro256PlusPlus::fill_unique_range<float>(float *, size_t, float, float);
template void ix_Xoshiro256PlusPlus::fill_unique_range<double>(double *, size_t, double, double);
// clang-format on
