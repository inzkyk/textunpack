#include "ix_combinatorics.hpp"

#if ix_DO_TEST
#include "ix_Span.hpp"
#include "ix_test.hpp"

ix_TEST_CASE("ix_is_unique")
{
    {
        uint64_t buf[] = {1};
        ix_EXPECT(ix_is_unique(buf, 0));
        ix_EXPECT(ix_is_unique(buf, 1));
    }
    {
        uint64_t buf[] = {1, 2, 3};
        ix_EXPECT(ix_is_unique(buf, 3));
    }
    {
        uint64_t buf[] = {1, 2, 1};
        ix_EXPECT(!ix_is_unique(buf, 3));
    }
}

ix_TEST_CASE("ix_shuffle")
{
    const size_t N = 100;
    uint64_t buf[N] = {};
    ix_shuffle(buf, 0);
    ix_shuffle(buf, 1);

    for (size_t i = 0; i < N; i++)
    {
        buf[i] = i;
    }
    ix_shuffle(buf, N);
    for (size_t i = 0; i < N; i++)
    {
        ix_EXPECT(buf[i] < N);
    }
    ix_EXPECT(ix_is_unique(buf, N));
}

ix_TEST_CASE("ix_is_sorted")
{
    ix_Vector<int> vec;

    vec = ix_Vector<int>{};
    ix_EXPECT(ix_is_sorted(vec.span()));

    vec = ix_Vector<int>{1};
    ix_EXPECT(ix_is_sorted(vec.span()));

    vec = ix_Vector<int>{1, 2, 3, 4, 5};
    ix_EXPECT(ix_is_sorted(vec.span()));

    vec = ix_Vector<int>{2, 1, 3, 4, 5};
    ix_EXPECT(!ix_is_sorted(vec.span()));

    vec = ix_Vector<int>{1, 2, 3, 5, 4};
    ix_EXPECT(!ix_is_sorted(vec.span()));

    vec = ix_Vector<int>{};
    ix_EXPECT(ix_is_sorted(vec.span(), ix_Greater<int>()));

    vec = ix_Vector<int>{1};
    ix_EXPECT(ix_is_sorted(vec.span(), ix_Greater<int>()));

    vec = ix_Vector<int>{5, 4, 3, 2, 1};
    ix_EXPECT(ix_is_sorted(vec.span(), ix_Greater<int>()));

    vec = ix_Vector<int>{4, 5, 3, 2, 1};
    ix_EXPECT(!ix_is_sorted(vec.span(), ix_Greater<int>()));

    vec = ix_Vector<int>{5, 4, 3, 1, 2};
    ix_EXPECT(!ix_is_sorted(vec.span(), ix_Greater<int>()));
}
#endif
