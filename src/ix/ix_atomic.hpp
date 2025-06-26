#pragma once

#include "ix.hpp"
#include "ix_memory.hpp"

enum ix_MemoryOrder : uint8_t
{
    ix_MEMORY_ORDER_RELAXED,
    ix_MEMORY_ORDER_CONSUME,
    ix_MEMORY_ORDER_ACQUIRE,
    ix_MEMORY_ORDER_RELEASE,
    ix_MEMORY_ORDER_ACQ_REL,
    ix_MEMORY_ORDER_SEQ_CST,
};

void ix_atomic_memory_barrier(ix_MemoryOrder order);

// RETURN x;
template <typename T>
T ix_atomic_load(const T volatile *x, ix_MemoryOrder order);

// x <- value;
template <typename T>
void ix_atomic_store(T volatile *x, T value, ix_MemoryOrder order);

// old <- *x;
// *x <- old + value;
// RETURN old;
template <typename T>
T ix_atomic_fetch_add(T volatile *x, T value, ix_MemoryOrder order);

// old <- *x;
// *x <- old - value;
// RETURN old;
template <typename T>
T ix_atomic_fetch_sub(T volatile *x, T value, ix_MemoryOrder order);

// IF (*x == *expected) THEN
//   *x <- desired;
//   RETURN true;
// ELSE
//   *expected <- *x;
//   return false;
// ENDIF
template <typename T>
bool ix_atomic_cas_weak(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure);

template <typename T>
bool ix_atomic_cas_strong(T volatile *x, T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure);

template <typename T>
class ix_Atomic
{
    T x;

  public:
    ix_Atomic() = default;

    ix_FORCE_INLINE T load(ix_MemoryOrder order) const
    {
        return ix_atomic_load(&x, order);
    }

    ix_FORCE_INLINE void store(T new_value, ix_MemoryOrder order)
    {
        ix_atomic_store(&x, new_value, order);
    }

    ix_FORCE_INLINE T fetch_add(T operand, ix_MemoryOrder order)
    {
        return ix_atomic_fetch_add(&x, operand, order);
    }

    ix_FORCE_INLINE T fetch_sub(T operand, ix_MemoryOrder order)
    {
        return ix_atomic_fetch_add(&x, operand, order);
    }

    ix_FORCE_INLINE bool cas_weak(T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
    {
        return ix_atomic_cas_weak(&x, expected, desired, success, failure);
    }

    ix_FORCE_INLINE bool cas_weak(T *expected, T desired, ix_MemoryOrder order)
    {
        return ix_atomic_cas_weak(&x, expected, desired, order, order);
    }

    ix_FORCE_INLINE bool cas_strong(T *expected, T desired, ix_MemoryOrder success, ix_MemoryOrder failure)
    {
        return ix_atomic_cas_strong(&x, expected, desired, success, failure);
    }

    ix_FORCE_INLINE bool cas_strong(T *expected, T desired, ix_MemoryOrder order)
    {
        return ix_atomic_cas_strong(&x, expected, desired, order, order);
    }
};

template <typename T>
ix_FORCE_INLINE T *ix_atomic_load(T *volatile const *x, ix_MemoryOrder order)
{
    if constexpr (sizeof(T *) == 8)
    {
        const uint64_t x_u64 = ix_atomic_load(reinterpret_cast<const uint64_t volatile *>(x), order);
        return reinterpret_cast<T *>(x_u64);
    }
    else if constexpr (sizeof(T *) == 4)
    {
        const uint32_t x_u32 = ix_atomic_load(reinterpret_cast<const uint32_t volatile *>(x), order);
        return reinterpret_cast<T *>(x_u32);
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE void ix_atomic_store(T *volatile *x, T *value, ix_MemoryOrder order)
{
    if constexpr (sizeof(T *) == 8)
    {
        uint64_t value_8;
        ix_memcpy(&value_8, &value, sizeof(T *));
        ix_atomic_store(reinterpret_cast<uint64_t volatile *>(x), value_8, order);
        return;
    }
    else if constexpr (sizeof(T *) == 4)
    {
        uint32_t value_4;
        ix_memcpy(&value_4, &value, sizeof(T *));
        ix_atomic_store(reinterpret_cast<uint32_t volatile *>(x), value_4, order);
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_weak(
    T *volatile *x,
    T **expected,
    T *desired,
    ix_MemoryOrder success,
    ix_MemoryOrder failure
)
{
    if constexpr (sizeof(T *) == 8)
    {
        return ix_atomic_cas_weak<uint64_t>(
            reinterpret_cast<uint64_t volatile *>(x),
            reinterpret_cast<uint64_t *>(expected),
            reinterpret_cast<uint64_t>(desired),
            success,
            failure
        );
    }
    else if constexpr (sizeof(T *) == 4)
    {
        return ix_atomic_cas_weak<uint32_t>(
            reinterpret_cast<uint32_t volatile *>(x),
            reinterpret_cast<uint32_t *>(expected),
            reinterpret_cast<uint32_t>(desired),
            success,
            failure
        );
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_strong(
    T *volatile *x,
    T **expected,
    T *desired,
    ix_MemoryOrder success,
    ix_MemoryOrder failure
)
{
    if constexpr (sizeof(T *) == 8)
    {
        return ix_atomic_cas_strong<uint64_t>(
            reinterpret_cast<uint64_t volatile *>(x),
            reinterpret_cast<uint64_t *>(expected),
            reinterpret_cast<uint64_t>(desired),
            success,
            failure
        );
    }
    else if constexpr (sizeof(T *) == 4)
    {
        return ix_atomic_cas_strong<uint32_t>(
            reinterpret_cast<uint32_t volatile *>(x),
            reinterpret_cast<uint32_t *>(expected),
            reinterpret_cast<uint32_t>(desired),
            success,
            failure
        );
    }
    else
    {
        ix_UNREACHABLE();
    }
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_weak(T volatile *x, T *expected, T desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_weak(x, expected, desired, order, order);
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_weak(T *volatile *x, T **expected, T *desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_weak(x, expected, desired, order, order);
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_strong(T volatile *x, T *expected, T desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_strong(x, expected, desired, order, order);
}

template <typename T>
ix_FORCE_INLINE bool ix_atomic_cas_strong(T *volatile *x, T **expected, T *desired, ix_MemoryOrder order)
{
    return ix_atomic_cas_strong(x, expected, desired, order, order);
}

// clang-format off
extern template int8_t   ix_atomic_load(const int8_t   volatile *x, ix_MemoryOrder order);
extern template int16_t  ix_atomic_load(const int16_t  volatile *x, ix_MemoryOrder order);
extern template int32_t  ix_atomic_load(const int32_t  volatile *x, ix_MemoryOrder order);
extern template int64_t  ix_atomic_load(const int64_t  volatile *x, ix_MemoryOrder order);
extern template uint8_t  ix_atomic_load(const uint8_t  volatile *x, ix_MemoryOrder order);
extern template uint16_t ix_atomic_load(const uint16_t volatile *x, ix_MemoryOrder order);
extern template uint32_t ix_atomic_load(const uint32_t volatile *x, ix_MemoryOrder order);
extern template uint64_t ix_atomic_load(const uint64_t volatile *x, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
extern template size_t   ix_atomic_load(const size_t   volatile *x, ix_MemoryOrder order);
#endif

extern template void ix_atomic_store(int8_t   volatile *x, int8_t   value, ix_MemoryOrder order);
extern template void ix_atomic_store(int16_t  volatile *x, int16_t  value, ix_MemoryOrder order);
extern template void ix_atomic_store(int32_t  volatile *x, int32_t  value, ix_MemoryOrder order);
extern template void ix_atomic_store(int64_t  volatile *x, int64_t  value, ix_MemoryOrder order);
extern template void ix_atomic_store(uint8_t  volatile *x, uint8_t  value, ix_MemoryOrder order);
extern template void ix_atomic_store(uint16_t volatile *x, uint16_t value, ix_MemoryOrder order);
extern template void ix_atomic_store(uint32_t volatile *x, uint32_t value, ix_MemoryOrder order);
extern template void ix_atomic_store(uint64_t volatile *x, uint64_t value, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
extern template void ix_atomic_store(size_t   volatile *x, size_t   value, ix_MemoryOrder order);
#endif

extern template int8_t   ix_atomic_fetch_add(int8_t   volatile *x, int8_t   value, ix_MemoryOrder order);
extern template int16_t  ix_atomic_fetch_add(int16_t  volatile *x, int16_t  value, ix_MemoryOrder order);
extern template int32_t  ix_atomic_fetch_add(int32_t  volatile *x, int32_t  value, ix_MemoryOrder order);
extern template int64_t  ix_atomic_fetch_add(int64_t  volatile *x, int64_t  value, ix_MemoryOrder order);
extern template uint8_t  ix_atomic_fetch_add(uint8_t  volatile *x, uint8_t  value, ix_MemoryOrder order);
extern template uint16_t ix_atomic_fetch_add(uint16_t volatile *x, uint16_t value, ix_MemoryOrder order);
extern template uint32_t ix_atomic_fetch_add(uint32_t volatile *x, uint32_t value, ix_MemoryOrder order);
extern template uint64_t ix_atomic_fetch_add(uint64_t volatile *x, uint64_t value, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
extern template size_t   ix_atomic_fetch_add(size_t   volatile *x, size_t   value, ix_MemoryOrder order);
#endif

extern template int8_t   ix_atomic_fetch_sub(int8_t   volatile *x, int8_t   value, ix_MemoryOrder order);
extern template int16_t  ix_atomic_fetch_sub(int16_t  volatile *x, int16_t  value, ix_MemoryOrder order);
extern template int32_t  ix_atomic_fetch_sub(int32_t  volatile *x, int32_t  value, ix_MemoryOrder order);
extern template int64_t  ix_atomic_fetch_sub(int64_t  volatile *x, int64_t  value, ix_MemoryOrder order);
extern template uint8_t  ix_atomic_fetch_sub(uint8_t  volatile *x, uint8_t  value, ix_MemoryOrder order);
extern template uint16_t ix_atomic_fetch_sub(uint16_t volatile *x, uint16_t value, ix_MemoryOrder order);
extern template uint32_t ix_atomic_fetch_sub(uint32_t volatile *x, uint32_t value, ix_MemoryOrder order);
extern template uint64_t ix_atomic_fetch_sub(uint64_t volatile *x, uint64_t value, ix_MemoryOrder order);
#if ix_PLATFORM(WASM)
extern template size_t   ix_atomic_fetch_sub(size_t   volatile *x, size_t   value, ix_MemoryOrder order);
#endif

extern template bool ix_atomic_cas_weak(int8_t   volatile *x, int8_t   *expected, int8_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(int16_t  volatile *x, int16_t  *expected, int16_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(int32_t  volatile *x, int32_t  *expected, int32_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(int64_t  volatile *x, int64_t  *expected, int64_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(uint8_t  volatile *x, uint8_t  *expected, uint8_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(uint16_t volatile *x, uint16_t *expected, uint16_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(uint32_t volatile *x, uint32_t *expected, uint32_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_weak(uint64_t volatile *x, uint64_t *expected, uint64_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#if ix_PLATFORM(WASM)
extern template bool ix_atomic_cas_weak(size_t   volatile *x, size_t   *expected, size_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#endif

extern template bool ix_atomic_cas_strong(int8_t   volatile *x, int8_t   *expected, int8_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(int16_t  volatile *x, int16_t  *expected, int16_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(int32_t  volatile *x, int32_t  *expected, int32_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(int64_t  volatile *x, int64_t  *expected, int64_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(uint8_t  volatile *x, uint8_t  *expected, uint8_t  desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(uint16_t volatile *x, uint16_t *expected, uint16_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(uint32_t volatile *x, uint32_t *expected, uint32_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
extern template bool ix_atomic_cas_strong(uint64_t volatile *x, uint64_t *expected, uint64_t desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#if ix_PLATFORM(WASM)
extern template bool ix_atomic_cas_strong(size_t   volatile *x, size_t   *expected, size_t   desired, ix_MemoryOrder success, ix_MemoryOrder failure);
#endif
// clang-format on
