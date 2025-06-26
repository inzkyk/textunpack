#pragma once

#include "ix.hpp"
#include "ix_Pair.hpp"
#include "ix_Span.hpp"
#include "ix_compare.hpp"
#include "ix_initializer_list.hpp"
#include "ix_min_max.hpp"
#include "ix_utility.hpp"

// Based on pdqsort.h (https://github.com/orlp/pdqsort, zlib license).

namespace ix_detail::sort
{
// Partitions below this size are sorted using insertion sort.
constexpr ptrdiff_t insertion_sort_threshold = 24;

// Partitions above this size use Tukey's ninther to select the pivot.
constexpr ptrdiff_t ninther_threshold = 128;

// When we detect an already sorted partition, attempt an insertion sort that allows this
// amount of element moves before giving up.
constexpr size_t partial_insertion_sort_limit = 8;

// Must be multiple of 8 due to loop unrolling, and < 256 to fit in unsigned char.
constexpr unsigned char block_size = 64;

// Cacheline size, assumes power of two.
constexpr uintptr_t cacheline_size = 64;

// Returns floor(log2(n)), assumes n > 0.
template <class T>
inline int log2(T n)
{
    int log = 0;
    while (n >>= 1)
    {
        log += 1;
    }
    return log;
}

template <typename T, typename Less>
inline void sift_down(T *first, ptrdiff_t n, ptrdiff_t start, Less less)
{
    ptrdiff_t root = start;
    T tmp = ix_move(first[root]);
    while (true)
    {
        ptrdiff_t child = (2 * root) + 1;
        const bool no_child = (child >= n);
        if (no_child)
        {
            break;
        }

        const ptrdiff_t right = child + 1;
        if (right < n && less(first[child], first[right]))
        {
            child = right;
        }

        if (!less(tmp, first[child]))
        {
            break;
        }

        first[root] = ix_move(first[child]);
        root = child;
    }
    first[root] = ix_move(tmp); // Place original root element into its final position
}

template <typename T, typename Less>
inline void make_heap(T *first, T *last, Less less)
{
    const ptrdiff_t n = last - first;
    if (n < 2)
    {
        return;
    }

    ptrdiff_t i = (n / 2) - 1;
    while (i >= 0)
    {
        sift_down(first, n, i, less);
        i -= 1;
    }
}

template <typename T, typename Less>
inline void sort_heap(T *first, T *last, Less less)
{
    const ptrdiff_t n = last - first;
    ptrdiff_t i = last - first;
    while (i > 1)
    {
        ix_swap(first[0], first[i - 1]);
        i -= 1;
        sift_down(first, n, i, less);
    }
}

// Sorts [begin, end) using insertion sort with the given comparison function.
template <class T, class Less>
inline void insertion_sort(T *begin, T *end, Less less)
{
    if (begin == end)
    {
        return;
    }

    for (T *cur = begin + 1; cur != end; ++cur)
    {
        T *sift = cur;
        T *sift_1 = cur - 1;

        // Less first so we can avoid 2 moves for an element already positioned correctly.
        if (less(*sift, *sift_1))
        {
            T tmp = ix_move(*sift);

            do
            {
                *sift-- = ix_move(*sift_1);
            } while (sift != begin && less(tmp, *--sift_1));

            *sift = ix_move(tmp);
        }
    }
}

// Sorts [begin, end) using insertion sort with the given comparison function. Assumes
// *(begin - 1) is an element smaller than or equal to any element in [begin, end).
template <class T, class Less>
inline void unguarded_insertion_sort(T *begin, T *end, Less less)
{
    if (begin == end)
    {
        return;
    }

    for (T *cur = begin + 1; cur != end; ++cur)
    {
        T *sift = cur;
        T *sift_1 = cur - 1;

        // Less first so we can avoid 2 moves for an element already positioned correctly.
        if (less(*sift, *sift_1))
        {
            T tmp = ix_move(*sift);

            do
            {
                *sift-- = ix_move(*sift_1);
            } while (less(tmp, *--sift_1));

            *sift = ix_move(tmp);
        }
    }
}

// Attempts to use insertion sort on [begin, end). Will return false if more than
// partial_insertion_sort_limit elements were moved, and abort sorting. Otherwise it will
// successfully sort and return true.
template <class T, class Less>
inline bool partial_insertion_sort(T *begin, T *end, Less less)
{
    if (begin == end)
    {
        return true;
    }

    size_t limit = 0;
    for (T *cur = begin + 1; cur != end; ++cur)
    {
        T *sift = cur;
        T *sift_1 = cur - 1;

        // Less first so we can avoid 2 moves for an element already positioned correctly.
        if (less(*sift, *sift_1))
        {
            T tmp = ix_move(*sift);

            do
            {
                *sift-- = ix_move(*sift_1);
            } while (sift != begin && less(tmp, *--sift_1));

            *sift = ix_move(tmp);
            limit += static_cast<size_t>(cur - sift);
        }

        if (limit > partial_insertion_sort_limit)
        {
            return false;
        }
    }

    return true;
}

template <class T, class Less>
inline void sort2(T *a, T *b, Less less)
{
    if (less(*b, *a))
    {
        ix_iter_swap(a, b);
    }
}

// Sorts the elements *a, *b and *c using comparison function less.
template <class T, class Less>
inline void sort3(T *a, T *b, T *c, Less less)
{
    sort2(a, b, less);
    sort2(b, c, less);
    sort2(a, b, less);
}

template <class T>
inline T *align_cacheline(T *p)
{
    uintptr_t ip = reinterpret_cast<uintptr_t>(p);
    ip = (ip + cacheline_size - uintptr_t{1}) & static_cast<uintptr_t>(-intptr_t{cacheline_size});
    return reinterpret_cast<T *>(ip);
}

template <class T>
inline void swap_offsets(
    T *first,
    T *last,
    unsigned char *offsets_l,
    unsigned char *offsets_r,
    size_t num,
    bool use_swaps
)
{
    if (use_swaps)
    {
        // This case is needed for the descending distribution, where we need
        // to have proper swapping for pdqsort to remain O(n).
        for (size_t i = 0; i < num; ++i)
        {
            ix_iter_swap(first + offsets_l[i], last - offsets_r[i]);
        }
    }
    else if (num > 0)
    {
        T *l = first + offsets_l[0];
        T *r = last - offsets_r[0];
        T tmp(ix_move(*l));
        *l = ix_move(*r);
        for (size_t i = 1; i < num; ++i)
        {
            l = first + offsets_l[i];
            *r = ix_move(*l);
            r = last - offsets_r[i];
            *l = ix_move(*r);
        }
        *r = ix_move(tmp);
    }
}

// Partitions [begin, end) around pivot *begin using comparison function less. Elements equal
// to the pivot are put in the right-hand partition. Returns the position of the pivot after
// partitioning and whether the passed sequence already was correctly partitioned. Assumes the
// pivot is a median of at least 3 elements and that [begin, end) is at least
// insertion_sort_threshold long. Uses branchless partitioning.
template <class T, class Less>
inline ix_Pair<T *, bool> partition_right_branchless(T *begin, T *end, Less less)
{
    // Move pivot into local for speed.
    T pivot(ix_move(*begin));
    T *first = begin;
    T *last = end;

    // Find the first element greater than or equal than the pivot (the median of 3 guarantees
    // this exists).
    while (less(*++first, pivot))
    {
    }

    // Find the first element strictly smaller than the pivot. We have to guard this search if
    // there was no element before *first.
    if (first - 1 == begin)
    {
        while (first < last && !less(*--last, pivot)) // NOLINT
        {
        }
    }
    else
    {
        while (!less(*--last, pivot))
        {
        }
    }

    // If the first pair of elements that should be swapped to partition are the same element,
    // the passed in sequence already was correctly partitioned.
    bool already_partitioned = first >= last;
    if (!already_partitioned)
    {
        ix_iter_swap(first, last);
        ++first;

        // The following branchless partitioning is derived from "BlockQuicksort: How Branch
        // Mispredictions don’t affect Quicksort" by Stefan Edelkamp and Armin Weiss, but
        // heavily micro-optimized.
        unsigned char offsets_l_storage[block_size + cacheline_size];
        unsigned char offsets_r_storage[block_size + cacheline_size];
        unsigned char *offsets_l = align_cacheline(offsets_l_storage);
        unsigned char *offsets_r = align_cacheline(offsets_r_storage);

        T *offsets_l_base = first;
        T *offsets_r_base = last;
        size_t num_l;
        size_t num_r;
        size_t start_l;
        size_t start_r;
        num_l = num_r = start_l = start_r = 0;

        while (first < last)
        {
            // Fill up offset blocks with elements that are on the wrong side.
            // First we determine how much elements are considered for each offset block.
            size_t num_unknown = static_cast<size_t>(last - first);
            size_t left_split = num_l == 0 ? (num_r == 0 ? num_unknown / 2 : num_unknown) : 0;
            size_t right_split = num_r == 0 ? (num_unknown - left_split) : 0;

            // Fill the offset blocks.
            if (left_split >= block_size)
            {
                for (unsigned char i = 0; i < block_size;)
                {
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                }
            }
            else
            {
                for (unsigned char i = 0; i < left_split;)
                {
                    offsets_l[num_l] = i++;
                    num_l += !less(*first, pivot);
                    ++first;
                }
            }

            if (right_split >= block_size)
            {
                for (unsigned char i = 0; i < block_size;)
                {
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                }
            }
            else
            {
                for (unsigned char i = 0; i < right_split;)
                {
                    offsets_r[num_r] = ++i;
                    num_r += less(*--last, pivot);
                }
            }

            // Swap elements and update block sizes and first/last boundaries.
            size_t num = ix_min(num_l, num_r);
            swap_offsets(
                offsets_l_base,
                offsets_r_base,
                offsets_l + start_l,
                offsets_r + start_r,
                num,
                num_l == num_r
            );
            num_l -= num;
            num_r -= num;
            start_l += num;
            start_r += num;

            if (num_l == 0)
            {
                start_l = 0;
                offsets_l_base = first;
            }

            if (num_r == 0)
            {
                start_r = 0;
                offsets_r_base = last;
            }
        }

        // We have now fully identified [first, last)'s proper position. Swap the last elements.
        if (num_l)
        {
            offsets_l += start_l;
            while (num_l--)
            {
                ix_iter_swap(offsets_l_base + offsets_l[num_l], --last);
            }
            first = last;
        }
        if (num_r)
        {
            offsets_r += start_r;
            while (num_r--)
            {
                ix_iter_swap(offsets_r_base - offsets_r[num_r], first), ++first;
            }
            last = first;
        }
    }

    // Put the pivot in the right place.
    T *pivot_pos = first - 1;
    *begin = ix_move(*pivot_pos);
    *pivot_pos = ix_move(pivot);

    return {pivot_pos, already_partitioned};
}

// Partitions [begin, end) around pivot *begin using comparison function less. Elements equal
// to the pivot are put in the right-hand partition. Returns the position of the pivot after
// partitioning and whether the passed sequence already was correctly partitioned. Assumes the
// pivot is a median of at least 3 elements and that [begin, end) is at least
// insertion_sort_threshold long.
template <class T, class Less>
inline ix_Pair<T *, bool> partition_right(T *begin, T *end, Less less)
{
    // Move pivot into local for speed.
    T pivot(ix_move(*begin));

    T *first = begin;
    T *last = end;

    // Find the first element greater than or equal than the pivot (the median of 3 guarantees
    // this exists).
    while (less(*++first, pivot))
    {
    }

    // Find the first element strictly smaller than the pivot. We have to guard this search if
    // there was no element before *first.
    if (first - 1 == begin)
    {
        while (first < last && !less(*--last, pivot)) // NOLINT
        {
        }
    }
    else
    {
        while (!less(*--last, pivot))
        {
        }
    }

    // If the first pair of elements that should be swapped to partition are the same element,
    // the passed in sequence already was correctly partitioned.
    bool already_partitioned = first >= last;

    // Keep swapping pairs of elements that are on the wrong side of the pivot. Previously
    // swapped pairs guard the searches, which is why the first iteration is special-cased
    // above.
    while (first < last)
    {
        ix_iter_swap(first, last);
        while (less(*++first, pivot))
        {
        }
        while (!less(*--last, pivot))
        {
        }
    }

    // Put the pivot in the right place.
    T *pivot_pos = first - 1;
    *begin = ix_move(*pivot_pos);
    *pivot_pos = ix_move(pivot);

    return {pivot_pos, already_partitioned};
}

// Similar function to the one above, except elements equal to the pivot are put to the left of
// the pivot and it doesn't check or return if the passed sequence already was partitioned.
// Since this is rarely used (the many equal case), and in that case pdqsort already has O(n)
// performance, no block quicksort is applied here for simplicity.
template <class T, class Less>
inline T *partition_left(T *begin, T *end, Less less)
{
    T pivot(ix_move(*begin));
    T *first = begin;
    T *last = end;

    while (less(pivot, *--last))
    {
    }

    if (last + 1 == end)
    {
        while (first < last && !less(pivot, *++first)) // NOLINT
        {
        }
    }
    else
    {
        while (!less(pivot, *++first))
        {
        }
    }

    while (first < last)
    {
        ix_iter_swap(first, last);
        while (less(pivot, *--last))
        {
        }
        while (!less(pivot, *++first))
        {
        }
    }

    T *pivot_pos = last;
    *begin = ix_move(*pivot_pos);
    *pivot_pos = ix_move(pivot);

    return pivot_pos;
}

template <class T, class Less, bool Branchless>
inline void pdqsort_loop(T *begin, T *end, Less less, int bad_allowed, bool leftmost = true)
{
    // Use a while loop for tail recursion elimination.
    while (true)
    {
        ptrdiff_t size = end - begin;

        // Insertion sort is faster for small arrays.
        if (size < insertion_sort_threshold)
        {
            if (leftmost)
            {
                insertion_sort(begin, end, less);
            }
            else
            {
                unguarded_insertion_sort(begin, end, less);
            }
            return;
        }

        // Choose pivot as median of 3 or pseudomedian of 9.
        ptrdiff_t s2 = size / 2;
        if (size > ninther_threshold)
        {
            sort3(begin, begin + s2, end - 1, less);
            sort3(begin + 1, begin + (s2 - 1), end - 2, less);
            sort3(begin + 2, begin + (s2 + 1), end - 3, less);
            sort3(begin + (s2 - 1), begin + s2, begin + (s2 + 1), less);
            ix_iter_swap(begin, begin + s2);
        }
        else
        {
            sort3(begin + s2, begin, end - 1, less);
        }

        // If *(begin - 1) is the end of the right partition of a previous partition operation
        // there is no element in [begin, end) that is smaller than *(begin - 1). Then if our
        // pivot compares equal to *(begin - 1) we change strategy, putting equal elements in
        // the left partition, greater elements in the right partition. We do not have to
        // recurse on the left partition, since it's sorted (all equal).
        if (!leftmost && !less(*(begin - 1), *begin))
        {
            begin = partition_left(begin, end, less) + 1;
            continue;
        }

        // Partition and get results.
        ix_Pair<T *, bool> part_result =
            Branchless ? partition_right_branchless(begin, end, less) : partition_right(begin, end, less);
        T *pivot_pos = part_result.first;
        bool already_partitioned = part_result.second;

        // Check for a highly unbalanced partition.
        ptrdiff_t l_size = pivot_pos - begin;
        ptrdiff_t r_size = end - (pivot_pos + 1);
        bool highly_unbalanced = l_size < size / 8 || r_size < size / 8;

        // If we got a highly unbalanced partition we shuffle elements to break many patterns.
        if (highly_unbalanced)
        {
            // If we had too many bad partitions, switch to heapsort to guarantee O(n log n).
            if (--bad_allowed == 0)
            {
                make_heap(begin, end, less);
                sort_heap(begin, end, less);
                return;
            }

            if (l_size >= insertion_sort_threshold)
            {
                ix_iter_swap(begin, begin + (l_size / 4));
                ix_iter_swap(pivot_pos - 1, pivot_pos - (l_size / 4));

                if (l_size > ninther_threshold)
                {
                    ix_iter_swap(begin + 1, begin + (l_size / 4 + 1));
                    ix_iter_swap(begin + 2, begin + (l_size / 4 + 2));
                    ix_iter_swap(pivot_pos - 2, pivot_pos - (l_size / 4 + 1));
                    ix_iter_swap(pivot_pos - 3, pivot_pos - (l_size / 4 + 2));
                }
            }

            if (r_size >= insertion_sort_threshold)
            {
                ix_iter_swap(pivot_pos + 1, pivot_pos + (1 + r_size / 4));
                ix_iter_swap(end - 1, end - (r_size / 4));

                if (r_size > ninther_threshold)
                {
                    ix_iter_swap(pivot_pos + 2, pivot_pos + (2 + r_size / 4));
                    ix_iter_swap(pivot_pos + 3, pivot_pos + (3 + r_size / 4));
                    ix_iter_swap(end - 2, end - (1 + r_size / 4));
                    ix_iter_swap(end - 3, end - (2 + r_size / 4));
                }
            }
        }
        else
        {
            // If we were decently balanced and we tried to sort an already partitioned
            // sequence try to use insertion sort.
            if (already_partitioned && partial_insertion_sort(begin, pivot_pos, less) &&
                partial_insertion_sort(pivot_pos + 1, end, less))
            {
                return;
            }
        }

        // Sort the left partition first using recursion and do tail recursion elimination for
        // the right-hand partition.
        pdqsort_loop<T, Less, Branchless>(begin, pivot_pos, less, bad_allowed, leftmost);
        begin = pivot_pos + 1;
        leftmost = false;
    }
}
} // namespace ix_detail::sort

template <class T, class Less>
inline void ix_sort(ix_Span<T> span, Less less)
{
    if (span.size() <= 1)
    {
        return;
    }

    T *begin = span.begin();
    T *end = span.end();

    constexpr bool branchless = ix_is_integral_v<T> && //
                                (ix_is_same_v<ix_Less<T>, Less> || ix_is_same_v<ix_Greater<T>, Less>);
    ix_detail::sort::pdqsort_loop<T, Less, branchless>(begin, end, less, ix_detail::sort::log2(end - begin));
}

template <class T>
ix_FORCE_INLINE void ix_sort(ix_Span<T> span)
{
    ix_sort(span, ix_Less<T>());
}

template <class T, class Less>
inline void ix_sort_branchless(ix_Span<T> span, Less less)
{
    if (span.size() <= 1)
    {
        return;
    }

    T *begin = span.begin();
    T *end = span.end();

    ix_detail::sort::pdqsort_loop<T, Less, true>(begin, end, less, ix_detail::sort::log2(end - begin));
}

template <class T>
ix_FORCE_INLINE void ix_sort_branchless(ix_Span<T> span)
{
    ix_sort_branchless(span, ix_Less<T>());
}
