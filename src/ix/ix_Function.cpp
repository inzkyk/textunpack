#include "ix_Function.hpp"

#if ix_DO_TEST
#include "ix_Array.hpp"
#include "ix_Vector.hpp"
#include "ix_test.hpp"

template <typename F>
using ix_Function24 = ix_FunctionN<24, F>;

ix_TEST_CASE("ix_Function")
{
    {
        const ix_Function24<int(int, int)> f([](int x, int y) { return x + y; });
        ix_EXPECT(f(1, 2) == 3);
    }

    {
        const int a = 1;
        const int b = 2;
        const ix_Function24<int(void)> f([&]() { return a + b; });
        ix_EXPECT(f() == 3);
    }

    {
        const int a = 1;
        const int b = 2;
        const ix_Function24<int(void)> f([=]() { return a + b; });
        ix_EXPECT(f() == 3);
    }

    {
        ix_Vector<uint64_t> v{1, 2};
        const ix_Function24<uint64_t(void)> f([&]() { return v[0] + v[1]; });
        ix_EXPECT(f() == 3);
        v[0] = 100;
        ix_EXPECT(f() == 102);
    }

    {
        ix_Array<uint64_t, 2> v{1, 2};
        const ix_Function24<uint64_t(void)> f([=]() { return v[0] + v[1]; });
        ix_EXPECT(f() == 3);
        v[0] = 100;
        ix_EXPECT(f() == 3);
    }

    {
        struct S
        {
            uint64_t a;
            uint64_t b;
            uint64_t c;
        };

        const S X{10, 20, 30};
        const ix_Function24<uint64_t(void)> f([=]() { return X.a + X.b + X.c; });
        ix_EXPECT(f() == 60);
    }

    {
        ix_Function24<void(void)> f([]() {});
        ix_EXPECT(!f.is_null());
        ix_EXPECT(f.is_valid());
        f();
        ix_EXPECT(!f.is_null());
        ix_EXPECT(f.is_valid());
        f.nullify();
        ix_EXPECT(f.is_null());
        ix_EXPECT(!f.is_valid());
    }
}

static int twice(int i)
{
    return 2 * i;
}

static unsigned int square(unsigned int f)
{
    return f * f;
}

ix_TEST_CASE("ix_Function: from function pointer")
{
    const ix_FunctionN<16, int(int)> f(&twice);
    ix_EXPECT(f(10) == 20);

    const ix_FunctionN<16, unsigned int(unsigned int)> g(&square);
    ix_EXPECT(g(2) == 4);
}
#endif
