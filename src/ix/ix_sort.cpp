#include "ix_sort.hpp"

#if ix_DO_TEST
#include "ix_DumbInt.hpp"
#include "ix_DumbString.hpp"
#include "ix_Span.hpp"
#include "ix_combinatorics.hpp"
#include "ix_test.hpp"

ix_TEST_CASE("ix_sort (int)")
{
    ix_Vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ix_shuffle(vec);
    ix_sort(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort(vec.span(), [](int x, int y) { return x < y; });
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort_branchless(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort_branchless(vec.span(), [](int x, int y) { return x < y; });
    ix_EXPECT(ix_is_sorted(vec.span()));
}

ix_TEST_CASE("ix_sort: ix_DumbInt")
{
    using DumbInt = ix_DumbInt<192>;
    ix_Vector<DumbInt> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ix_shuffle(vec);
    ix_sort(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort(vec.span(), [](const DumbInt &x, const DumbInt &y) { return x < y; });
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort_branchless(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort_branchless(vec.span(), [](const DumbInt &x, const DumbInt &y) { return x < y; });
    ix_EXPECT(ix_is_sorted(vec.span()));
}

ix_TEST_CASE("ix_sort: ix_DumbString")
{
    ix_Vector<ix_DumbString> vec{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    ix_shuffle(vec);
    ix_sort(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort(vec.span(), [](const ix_DumbString &x, const ix_DumbString &y) { return x < y; });
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort_branchless(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort_branchless(vec.span(), [](const ix_DumbString &x, const ix_DumbString &y) { return x < y; });
    ix_EXPECT(ix_is_sorted(vec.span()));
}

// I didn't write pdfsort, so let's test extensively.

ix_TEST_CASE("ix_sort: many small")
{
    constexpr size_t N = ix_OPT_LEVEL(DEBUG) ? 256 : 1024;
    ix_Vector<size_t> vec(N);
    for (size_t i = 0; i < N; i++)
    {
        vec[i] = i;
    }

    for (size_t i = 0; i < N; i++)
    {
        ix_shuffle(vec.data(), i);
        ix_sort(vec.span(i));
        ix_is_sorted(vec.span(i));
    }

    for (size_t i = 0; i < N; i++)
    {
        ix_shuffle(vec.data(), i);
        ix_sort(vec.span(i), ix_Greater<size_t>());
        ix_is_sorted(vec.span(i), ix_Greater<size_t>());
    }
}

ix_TEST_CASE("ix_sort: many small (ix_DumbString)")
{
    constexpr size_t N = ix_OPT_LEVEL(DEBUG) ? 128 : 512;
    ix_Vector<ix_DumbString> vec;
    vec.reserve(N);
    for (size_t i = 0; i < N; i++)
    {
        vec.emplace_back(i);
    }

    for (size_t i = 0; i < N; i++)
    {
        ix_shuffle(vec.data(), i);
        ix_sort(vec.span(i));
        ix_is_sorted(vec.span(i));
    }

    for (size_t i = 0; i < N; i++)
    {
        ix_shuffle(vec.data(), i);
        ix_sort(vec.span(i), ix_Greater<ix_DumbString>());
        ix_is_sorted(vec.span(i), ix_Greater<ix_DumbString>());
    }
}

ix_TEST_CASE("ix_sort: large")
{
    constexpr size_t N = ix_OPT_LEVEL(DEBUG) ? (233 * 217) : (1003 * 1011);
    ix_Vector<size_t> vec(N);
    for (size_t i = 0; i < N; i++)
    {
        vec[i] = i;
    }

    ix_shuffle(vec);
    ix_sort(vec.span());
    ix_EXPECT(ix_is_sorted(vec.span()));

    ix_shuffle(vec);
    ix_sort(vec.span(), ix_Greater<size_t>());
    ix_EXPECT(ix_is_sorted(vec.span(), ix_Greater<size_t>()));
}

ix_TEST_CASE("ix_sort: large (ix_DumbString)")
{
    constexpr size_t N = ix_OPT_LEVEL(DEBUG) ? (119 * 121) : (491 * 513);
    ix_Vector<ix_DumbString> vec;
    vec.reserve(N);
    for (size_t i = 0; i < N; i++)
    {
        vec.emplace_back(i);
    }

    ix_shuffle(vec);
    ix_sort(vec.span(), ix_Greater<ix_DumbString>());
    ix_EXPECT(ix_is_sorted(vec.span(), ix_Greater<ix_DumbString>()));
}

#endif
