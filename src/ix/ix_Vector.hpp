#pragma once

#include "ix.hpp"
#include "ix_Vector_util_fwd.hpp"
#include "ix_assert.hpp"
#include "ix_bulk.hpp"
#include "ix_compare.hpp"
#include "ix_initializer_list.hpp"
#include "ix_memory.hpp"

template <typename T>
class ix_Span;

template <typename T>
class ix_Vector
{
    T *m_data;
    size_t m_size;
    size_t m_capacity;

    ix_VECTOR_UTIL_FRIEND_DECLARE(ix_Vector);

  public:
    ix_Vector(const ix_Vector &other) = delete;
    ix_Vector &operator=(const ix_Vector &other) = delete;

    ix_FORCE_INLINE constexpr ix_Vector()
        : m_data(nullptr),
          m_size(0),
          m_capacity(0)
    {
    }

    ix_FORCE_INLINE constexpr explicit ix_Vector(size_t initial_size)
        : m_data(ix_ALLOC_ARRAY(T, initial_size)),
          m_size(initial_size),
          m_capacity(initial_size)
    {
        ix_bulk_default_construct(m_data, initial_size);
    }

    // cppcheck-suppress noExplicitConstructor
    ix_FORCE_INLINE constexpr ix_Vector(const std::initializer_list<T> &xs)
        : m_size(xs.size()),
          m_capacity(xs.size())
    {
        m_data = ix_ALLOC_ARRAY(T, m_size);
        ix_bulk_copy_construct(m_data, xs.begin(), m_size);
    }

    ix_FORCE_INLINE ~ix_Vector()
    {
        ix_bulk_destruct(m_data, m_size);
        ix_FREE(m_data);
    }

    constexpr ix_Vector(ix_Vector &&other) noexcept
        : m_data(other.m_data),
          m_size(other.m_size),
          m_capacity(other.m_capacity)
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    constexpr ix_Vector &operator=(ix_Vector &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        ix_bulk_destruct(m_data, m_size);
        ix_FREE(m_data);

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;

        return *this;
    }

    constexpr bool operator==(const ix_Vector &other) const
    {
        if (m_size != other.m_size)
        {
            return false;
        }

        for (size_t i = 0; i < m_size; i++)
        {
            if (ix_not_equal<T>(m_data[i], other.m_data[i]))
            {
                return false;
            }
        }

        return true;
    }

    constexpr bool operator!=(const ix_Vector &other) const
    {
        return !(*this == other);
    }

    ix_FORCE_INLINE constexpr T *data()
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr const T *data() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr size_t size() const
    {
        return m_size;
    }

    ix_FORCE_INLINE constexpr size_t capacity() const
    {
        return m_capacity;
    }

    ix_FORCE_INLINE constexpr ix_Span<T> span()
    {
        return ix_Span<T>(m_data, m_size);
    }

    ix_FORCE_INLINE constexpr ix_Span<T> span(size_t i)
    {
        return ix_Span<T>(m_data, i);
    }

    ix_FORCE_INLINE constexpr bool empty() const
    {
        return (m_size == 0);
    }

    ix_FORCE_INLINE constexpr const T *begin() const
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr T *begin()
    {
        return m_data;
    }

    ix_FORCE_INLINE constexpr const T *end() const
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE constexpr T *end()
    {
        return m_data + m_size;
    }

    ix_FORCE_INLINE constexpr T &operator[](size_t i)
    {
        ix_ASSERT(i < m_size);
        return m_data[i];
    }

    ix_FORCE_INLINE constexpr const T &operator[](size_t i) const
    {
        ix_ASSERT(i < m_size);
        return m_data[i];
    }

    ix_FORCE_INLINE constexpr T &back()
    {
        ix_ASSERT(0 < m_size);
        return m_data[m_size - 1];
    }

    ix_FORCE_INLINE constexpr const T &back() const
    {
        ix_ASSERT(0 < m_size);
        return m_data[m_size - 1];
    }

    ix_FORCE_INLINE constexpr void pop_back()
    {
        ix_ASSERT(0 < m_size);
        m_data[m_size - 1].~T();
        m_size -= 1;
    }

    ix_FORCE_INLINE constexpr void clear()
    {
        ix_bulk_destruct(m_data, m_size);
        m_size = 0;
    }

    ix_FORCE_INLINE constexpr void reserve(size_t n)
    {
        if (n <= m_capacity)
        {
            return;
        }

        if constexpr (ix_is_trivially_move_constructible_v<T> && ix_is_trivially_destructible_v<T>)
        {
            m_data = ix_REALLOC_ARRAY(T, m_data, n);
            m_capacity = n;
        }
        else
        {
            T *new_data = ix_ALLOC_ARRAY(T, n);

            ix_bulk_move_construct(new_data, m_data, m_size);
            ix_bulk_destruct(m_data, m_size);

            ix_FREE(m_data);
            m_data = new_data;
            m_capacity = n;
        }
    }

    ix_FORCE_INLINE constexpr void resize(size_t n)
    {
        if (n < m_size)
        {
            ix_bulk_destruct(m_data + n, m_size - n);
            m_size = n;
            return;
        }

        if (m_capacity < n)
        {
            reserve(n);
        }

        ix_bulk_default_construct(m_data + m_size, n - m_size);
        m_size = n;
    }

    ix_FORCE_INLINE constexpr void resize(size_t n, const T &x)
    {
        if (n < m_size)
        {
            ix_bulk_destruct(m_data + n, m_size - n);
            m_size = n;
            return;
        }

        if (m_capacity < n)
        {
            reserve(n);
        }

        T *p = m_data + m_size;
        const T *p_end = m_data + n;
        while (p < p_end)
        {
            new (p) T(x);
            p += 1;
        }

        m_size = n;
    }

    ix_FORCE_INLINE constexpr void set_size(size_t size)
    {
        m_size = size;
    }

    void swap_erase(size_t index)
    {
        const size_t last_index = m_size - 1;

        if (index != last_index)
        {
            (*this)[index] = ix_move((*this)[last_index]);
        }

        pop_back();
    }

    ix_FORCE_INLINE constexpr void push_back(const T &x)
    {
        emplace_back(x);
    }

    ix_FORCE_INLINE constexpr void push_back(T &&x)
    {
        emplace_back(ix_move(x));
    }

    template <typename... Args>
    ix_FORCE_INLINE constexpr T &emplace_back(Args &&...args)
    {
        if (ix_UNLIKELY(m_size == m_capacity))
        {
            size_t new_capacity = ix_grow_array_size(m_size);
            reserve(new_capacity);
        }

        T *p = m_data + m_size;
        new (p) T(ix_forward<Args>(args)...);
        m_size += 1;

        return *p;
    }
};
