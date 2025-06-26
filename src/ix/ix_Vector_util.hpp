#pragma once

#include "ix.hpp"
#include "ix_assert.hpp"
#include "ix_bulk.hpp"
#include "ix_memory.hpp"
#include "ix_min_max.hpp"
#include "ix_type_traits.hpp"
#include "ix_utility.hpp"

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, T &&x)
{
    if (vec.m_size + 1 > vec.m_capacity)
    {
        const auto pos_index = pos - vec.m_data;
        size_t new_capacity = ix_grow_array_size(vec.m_capacity);
        vec.reserve(new_capacity);
        pos = vec.m_data + pos_index;
    }

    T *new_element = vec.m_data + vec.m_size;

    if (new_element == pos)
    {
        new (new_element) T(ix_move(x));
        vec.m_size += 1;
        return;
    }

    T *last_element = new_element - 1;
    new (new_element) T(ix_move(*last_element));

    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        size_t n = static_cast<size_t>(last_element - pos);
        ix_memmove(pos + 1, pos, sizeof(T) * n);
    }
    else
    {
        T *p = last_element;
        while (pos < p)
        {
            *p = ix_move(*(p - 1));
            p -= 1;
        }
    }

    *pos = ix_move(x);

    vec.m_size += 1;
}

template <typename Vec, typename T>
ix_FORCE_INLINE void ix_Vector_insert(Vec &vec, T *pos, const T &x)
{
    ix_Vector_insert(vec, pos, ix_move(T(x)));
}

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, const T *start, const T *end)
{
    size_t n = static_cast<size_t>(end - start);
    size_t required_capacity = vec.m_size + n;
    if (required_capacity >= vec.m_capacity)
    {
        const auto pos_index = pos - vec.m_data;
        size_t new_capacity = ix_max(ix_grow_array_size(vec.m_capacity), required_capacity);
        vec.reserve(new_capacity);
        pos = vec.m_data + pos_index;
    }

    T *new_element_start = vec.m_data + vec.m_size;
    if (new_element_start == pos)
    {
        ix_bulk_copy_construct(new_element_start, start, n);
        vec.m_size += n;
        return;
    }

    const size_t m = static_cast<size_t>(new_element_start - pos);
    if (m >= n)
    {
        //             pos                  new_element_start
        //              |                          |
        //              |<----------- m ---------->|
        // |------------|--------------------------|
        //              ^                          |
        //              |                          |
        //       |=============|                   |
        //       |<---- n ---->|                   |
        //                                         |
        //                                         |<---- n ---->|
        // |------------|=============|--------------------------|
        //              |<---- n ---->|<----------- m ---------->|

        ix_bulk_copy_construct(new_element_start, new_element_start - n, n);

        if constexpr (ix_is_trivially_copy_assignable_v<T>)
        {
            ix_memmove(pos + n, pos, sizeof(T) * (m - n));
        }
        else
        {
            T *p = new_element_start - 1;
            T *p_last = pos + n;
            while (p_last <= p)
            {
                *p = *(p - n);
                p -= 1;
            }
        }

        ix_bulk_copy_assign(pos, start, n);
    }
    else
    {
        //             pos           new_element_start
        //              |                   |
        //              |<------- m ------->|
        // |------------|-------------------|
        //              ^                   |
        //              |                   |
        //   |==========================|   |
        //   |<----------- n ---------->|   |
        //                                  |
        //                                  |<----------- n ---------->|
        // |------------|==========================|-------------------|
        //              |<----------- n ---------->|<------- m ------->|

        const size_t num_from_external = n - m;
        ix_bulk_copy_construct(new_element_start, start + m, num_from_external);
        ix_bulk_copy_construct(new_element_start + num_from_external, pos, m);
        ix_bulk_copy_assign(pos, start, m);
    }

    vec.m_size += n;
}

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos)
{
    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        const size_t n = vec.m_size - static_cast<size_t>(pos - vec.m_data) - 1;
        ix_memmove(pos, pos + 1, sizeof(T) * n);
        (vec.m_data + vec.m_size - 1)->~T();
    }
    else
    {
        T *p = pos;
        const T *p_end = vec.m_data + vec.m_size;
        while (p < p_end - 1)
        {
            *p = ix_move(*(p + 1));
            p += 1;
        }
        p->~T();
    }

    vec.m_size -= 1;
}

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos, size_t n)
{
    ix_ASSERT(vec.m_data <= pos);
    ix_ASSERT(n <= vec.m_size);

    if (n == 0)
    {
        return;
    }

    if constexpr (ix_is_trivially_move_assignable_v<T>)
    {
        const size_t num_elements_to_move = vec.m_size - static_cast<size_t>(pos - vec.m_data) - n;
        ix_memmove(pos, pos + n, sizeof(T) * num_elements_to_move);
        ix_bulk_destruct(vec.m_data + vec.m_size - 1 - n, n);
    }
    else
    {
        T *p = pos;
        const T *p_end = vec.m_data + vec.m_size;
        while (p < p_end - n)
        {
            *p = ix_move(*(p + n));
            p += 1;
        }
        p->~T();
    }

    vec.m_size -= n;
}
