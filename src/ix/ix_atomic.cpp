#include "ix_atomic.hpp"

#include <atomic>

static_assert(sizeof(std::atomic<int8_t>) == sizeof(int8_t));
static_assert(sizeof(std::atomic<int16_t>) == sizeof(int16_t));
static_assert(sizeof(std::atomic<int32_t>) == sizeof(int32_t));
static_assert(sizeof(std::atomic<int64_t>) == sizeof(int64_t));

static_assert(alignof(std::atomic<int8_t>) == alignof(int8_t));
static_assert(alignof(std::atomic<int16_t>) == alignof(int16_t));
static_assert(alignof(std::atomic<int32_t>) == alignof(int32_t));
static_assert(alignof(std::atomic<int64_t>) == alignof(int64_t));

static_assert(static_cast<int>(ix_MEMORY_ORDER_RELAXED) == static_cast<int>(std::memory_order_relaxed));
static_assert(static_cast<int>(ix_MEMORY_ORDER_CONSUME) == static_cast<int>(std::memory_order_consume));
static_assert(static_cast<int>(ix_MEMORY_ORDER_ACQUIRE) == static_cast<int>(std::memory_order_acquire));
static_assert(static_cast<int>(ix_MEMORY_ORDER_RELEASE) == static_cast<int>(std::memory_order_release));
static_assert(static_cast<int>(ix_MEMORY_ORDER_ACQ_REL) == static_cast<int>(std::memory_order_acq_rel));
static_assert(static_cast<int>(ix_MEMORY_ORDER_SEQ_CST) == static_cast<int>(std::memory_order_seq_cst));

void ix_atomic_memory_barrier(ix_MemoryOrder order)
{
    std::atomic_thread_fence(static_cast<std::memory_order>(order));
}

template <typename T>
T ix_atomic_load(const T volatile *x, ix_MemoryOrder order)
{
    return reinterpret_cast<const std::atomic<T> volatile *>(x)->load(static_cast<std::memory_order>(order));
}

#define implement_1(T, x, method, arg, order) \
    (reinterpret_cast<std::atomic<T> volatile *>(x)->method(arg, static_cast<std::memory_order>(order)))

template <typename T>
void ix_atomic_store(T volatile *x, T value, ix_MemoryOrder order)
{
    implement_1(T, x, store, value, order);
}

template <typename T>
T ix_atomic_fetch_add(T volatile *x, T value, ix_MemoryOrder order)
{
    return implement_1(T, x, fetch_add, value, order);
}

template <typename T>
T ix_atomic_fetch_sub(T volatile *x, T value, ix_MemoryOrder order)
{
    return implement_1(T, x, fetch_sub, value, order);
}

#define implement_2(T, x, method, arg_0, arg_1, order_0, order_1) \
    (reinterpret_cast<std::atomic<T> volatile *>(x)->method(      \
        *(arg_0),                                                 \
        arg_1,                                                    \
        static_cast<std::memory_order>(order_0),                  \
        static_cast<std::memory_order>(order_1)                   \
    ))

template <typename T>
bool ix_atomic_cas_weak(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
{
    return implement_2(T, x, compare_exchange_weak, expected, desired, success, failure);
}

template <typename T>
bool ix_atomic_cas_strong(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
{
    return implement_2(T, x, compare_exchange_strong, expected, desired, success, failure);
}

#if ix_DO_TEST
#include "ix_Thread.hpp"
#include "ix_test.hpp"

ix_TEST_CASE("ix_atomic_memory_barrier")
{
    ix_atomic_memory_barrier(ix_MEMORY_ORDER_SEQ_CST);
}

ix_TEST_CASE("ix_atomic_load")
{
    const int x = 10;
    ix_EXPECT(ix_atomic_load(&x, ix_MEMORY_ORDER_RELAXED) == 10);
}

ix_TEST_CASE("ix_atomic_store")
{
    int x = 10; // NOLINT(misc-const-correctness)
    ix_atomic_store(&x, 100, ix_MEMORY_ORDER_RELAXED);
    ix_EXPECT(x == 100);
}

ix_TEST_CASE("ix_atomic_fetch_add")
{
    int x = 10; // NOLINT(misc-const-correctness)
    ix_EXPECT(ix_atomic_fetch_add(&x, 100, ix_MEMORY_ORDER_RELAXED) == 10);
    ix_EXPECT(x == 110);
}

ix_TEST_CASE("ix_atomic_fetch_sub")
{
    int x = 10; // NOLINT(misc-const-correctness)
    ix_EXPECT(ix_atomic_fetch_sub(&x, 100, ix_MEMORY_ORDER_RELAXED) == 10);
    ix_EXPECT(x == -90);
}

ix_TEST_CASE("ix_atomic_cas_weak")
{
    int x = 10;
    int y = 10;
    bool b = ix_atomic_cas_weak(&x, &y, 20, ix_MEMORY_ORDER_RELAXED, ix_MEMORY_ORDER_RELAXED);
    ix_EXPECT(b);
    ix_EXPECT(x == 20);
    ix_EXPECT(y == 10);

    x = 10;
    y = 20;
    b = ix_atomic_cas_weak(&x, &y, 30, ix_MEMORY_ORDER_RELAXED, ix_MEMORY_ORDER_RELAXED);
    ix_EXPECT(!b);
    ix_EXPECT(x == 10);
    ix_EXPECT(y == 10);
}

ix_TEST_CASE("ix_atomic_cas_strong")
{
    int x = 10;
    int y = 10;
    bool b = ix_atomic_cas_strong(&x, &y, 20, ix_MEMORY_ORDER_RELAXED, ix_MEMORY_ORDER_RELAXED);
    ix_EXPECT(b);
    ix_EXPECT(x == 20);
    ix_EXPECT(y == 10);

    x = 10;
    y = 20;
    b = ix_atomic_cas_strong(&x, &y, 30, ix_MEMORY_ORDER_RELAXED, ix_MEMORY_ORDER_RELAXED);
    ix_EXPECT(!b);
    ix_EXPECT(x == 10);
    ix_EXPECT(y == 10);
}

ix_TEST_CASE("ix_atomic_fetch_add: with multiple threads")
{
    int x = 0;

    static constexpr int16_t N = 64;
    ix_Thread threads[N];
    for (int16_t i = 0; i < N; i++)
    {
        threads[i].start([&]() {
            for (int16_t j = 0; j < N; j++)
            {
                ix_atomic_fetch_add(&x, 1, ix_MEMORY_ORDER_RELAXED);
                ix_yield_this_thread();
            }
        });
    }

    for (int16_t i = 0; i < N; i++)
    {
        threads[i].join();
    }

    ix_EXPECT(x == N * N);
    ix_EXPECT(ix_atomic_fetch_add(&x, 1, ix_MEMORY_ORDER_RELAXED) == N * N);
    ix_EXPECT(x == N * N + 1);
}

ix_TEST_CASE("ix_atomic_fetch_sub: multiple threads")
{
    int x = 0;

    static constexpr int16_t N = 64;
    ix_Thread threads[N];
    for (int16_t i = 0; i < N; i++)
    {
        threads[i].start([&]() {
            for (int16_t j = 0; j < N; j++)
            {
                ix_atomic_fetch_sub(&x, 1, ix_MEMORY_ORDER_RELAXED);
                ix_yield_this_thread();
            }
        });
    }

    for (int16_t i = 0; i < N; i++)
    {
        threads[i].join();
    }

    ix_EXPECT(x == -N * N);
    ix_EXPECT(ix_atomic_fetch_sub(&x, 1, ix_MEMORY_ORDER_RELAXED) == -N * N);
    ix_EXPECT(x == -N * N - 1);
}
#endif

// clang-format off
template int8_t   ix_atomic_load(const int8_t   volatile *x, ix_MemoryOrder order);
template int16_t  ix_atomic_load(const int16_t  volatile *x, ix_MemoryOrder order);
template int32_t  ix_atomic_load(const int32_t  volatile *x, ix_MemoryOrder order);
template int64_t  ix_atomic_load(const int64_t  volatile *x, ix_MemoryOrder order);
template uint8_t  ix_atomic_load(const uint8_t  volatile *x, ix_MemoryOrder order);
template uint16_t ix_atomic_load(const uint16_t volatile *x, ix_MemoryOrder order);
template uint32_t ix_atomic_load(const uint32_t volatile *x, ix_MemoryOrder order);
template uint64_t ix_atomic_load(const uint64_t volatile *x, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
template size_t   ix_atomic_load(const size_t   volatile *x, ix_MemoryOrder order);
#endif

template void ix_atomic_store(int8_t   volatile *x, int8_t   value, ix_MemoryOrder order);
template void ix_atomic_store(int16_t  volatile *x, int16_t  value, ix_MemoryOrder order);
template void ix_atomic_store(int32_t  volatile *x, int32_t  value, ix_MemoryOrder order);
template void ix_atomic_store(int64_t  volatile *x, int64_t  value, ix_MemoryOrder order);
template void ix_atomic_store(uint8_t  volatile *x, uint8_t  value, ix_MemoryOrder order);
template void ix_atomic_store(uint16_t volatile *x, uint16_t value, ix_MemoryOrder order);
template void ix_atomic_store(uint32_t volatile *x, uint32_t value, ix_MemoryOrder order);
template void ix_atomic_store(uint64_t volatile *x, uint64_t value, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
template void ix_atomic_store(size_t   volatile *x, size_t   value, ix_MemoryOrder order);
#endif

template int8_t   ix_atomic_fetch_add(int8_t   volatile *x, int8_t   value, ix_MemoryOrder order);
template int16_t  ix_atomic_fetch_add(int16_t  volatile *x, int16_t  value, ix_MemoryOrder order);
template int32_t  ix_atomic_fetch_add(int32_t  volatile *x, int32_t  value, ix_MemoryOrder order);
template int64_t  ix_atomic_fetch_add(int64_t  volatile *x, int64_t  value, ix_MemoryOrder order);
template uint8_t  ix_atomic_fetch_add(uint8_t  volatile *x, uint8_t  value, ix_MemoryOrder order);
template uint16_t ix_atomic_fetch_add(uint16_t volatile *x, uint16_t value, ix_MemoryOrder order);
template uint32_t ix_atomic_fetch_add(uint32_t volatile *x, uint32_t value, ix_MemoryOrder order);
template uint64_t ix_atomic_fetch_add(uint64_t volatile *x, uint64_t value, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
template size_t   ix_atomic_fetch_add(size_t   volatile *x, size_t   value, ix_MemoryOrder order);
#endif

template int8_t   ix_atomic_fetch_sub(int8_t   volatile *x, int8_t   value, ix_MemoryOrder order);
template int16_t  ix_atomic_fetch_sub(int16_t  volatile *x, int16_t  value, ix_MemoryOrder order);
template int32_t  ix_atomic_fetch_sub(int32_t  volatile *x, int32_t  value, ix_MemoryOrder order);
template int64_t  ix_atomic_fetch_sub(int64_t  volatile *x, int64_t  value, ix_MemoryOrder order);
template uint8_t  ix_atomic_fetch_sub(uint8_t  volatile *x, uint8_t  value, ix_MemoryOrder order);
template uint16_t ix_atomic_fetch_sub(uint16_t volatile *x, uint16_t value, ix_MemoryOrder order);
template uint32_t ix_atomic_fetch_sub(uint32_t volatile *x, uint32_t value, ix_MemoryOrder order);
template uint64_t ix_atomic_fetch_sub(uint64_t volatile *x, uint64_t value, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
template size_t   ix_atomic_fetch_sub(size_t   volatile *x, size_t   value, ix_MemoryOrder order);
#endif

template bool ix_atomic_cas_weak(int8_t   volatile *x, int8_t   *expected, int8_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(int16_t  volatile *x, int16_t  *expected, int16_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(int32_t  volatile *x, int32_t  *expected, int32_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(int64_t  volatile *x, int64_t  *expected, int64_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(uint8_t  volatile *x, uint8_t  *expected, uint8_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(uint16_t volatile *x, uint16_t *expected, uint16_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(uint32_t volatile *x, uint32_t *expected, uint32_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_weak(uint64_t volatile *x, uint64_t *expected, uint64_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#if ix_PLATFORM(WASM)
template bool ix_atomic_cas_weak(size_t   volatile *x, size_t   *expected, size_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#endif

template bool ix_atomic_cas_strong(int8_t   volatile *x, int8_t   *expected, int8_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(int16_t  volatile *x, int16_t  *expected, int16_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(int32_t  volatile *x, int32_t  *expected, int32_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(int64_t  volatile *x, int64_t  *expected, int64_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(uint8_t  volatile *x, uint8_t  *expected, uint8_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(uint16_t volatile *x, uint16_t *expected, uint16_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(uint32_t volatile *x, uint32_t *expected, uint32_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
template bool ix_atomic_cas_strong(uint64_t volatile *x, uint64_t *expected, uint64_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#if ix_PLATFORM(WASM)
template bool ix_atomic_cas_strong(size_t   volatile *x, size_t   *expected, size_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#endif
// clang-format on
