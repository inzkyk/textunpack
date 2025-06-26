#pragma once

#include "ix.hpp"
#include "ix_HashSet.hpp"
#include "ix_random.hpp"

template <typename T>
class ix_Span;

template <typename T>
void ix_shuffle(ix_Span<T> span);

template <typename T>
bool ix_is_unique(const T *arr, size_t length);

template <typename T>
bool ix_is_sorted(ix_Span<T> span);

// cf. https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
template <typename T>
void ix_shuffle(T *arr, size_t length)
{
    if (length == 0)
    {
        return;
    }
    for (size_t i = length - 1; i != 0; i--)
    {
        const size_t j = ix_rand<size_t>() % i;
        ix_swap(arr[i], arr[j]);
    }
}

template <typename Container>
ix_FORCE_INLINE void ix_shuffle(Container &container)
{
    ix_shuffle(container.data(), container.size());
}

template <typename T>
bool ix_is_unique(const T *arr, size_t length)
{
    if (length == 0)
    {
        return true;
    }

    ix_HashSet<T> values;
    values.reserve(length);
    for (size_t i = 0; i < length; i++)
    {
        if (values.contains(arr[i]))
        {
            return false;
        }
        values.insert(arr[i]);
    }
    return true;
}

template <typename T, typename Less>
bool ix_is_sorted(ix_Span<T> span, Less less)
{
    const size_t length = span.size();
    if (length <= 1)
    {
        return true;
    }

    for (size_t i = 1; i < length; i++)
    {
        if (less(span[i - 1], span[i]))
        {
            continue;
        }
        return false;
    }

    return true;
}

template <typename T>
ix_FORCE_INLINE bool ix_is_sorted(ix_Span<T> span)
{
    return ix_is_sorted(span, ix_Less<T>());
}
