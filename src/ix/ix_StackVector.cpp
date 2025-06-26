#include "ix_StackVector.hpp"

#if ix_DO_TEST
#include "ix_DumbString.hpp"
#include "ix_test.hpp"
#include "ix_test_Vector.hpp"

template <typename T>
using ix_StackVector2 = ix_StackVector<T, 2>;

template <typename T>
using ix_StackVector32 = ix_StackVector<T, 32>;

ix_TEST_CASE("ix_StackVector")
{
    ix_test_Vector<ix_StackVector2>();
    ix_test_Vector<ix_StackVector32>();
}

ix_TEST_CASE("ix_StackVector: default constructor")
{
    const ix_StackVector<uint64_t, 2> v{};
    ix_EXPECT(v.data() != nullptr);
    ix_EXPECT(v.empty());
    ix_EXPECT(v.capacity() == 2);
}

ix_TEST_CASE("ix_StackVector: initializer list")
{
    const ix_StackVector<int, 2> v{};
    ix_EXPECT(v.empty());
    ix_EXPECT(v.capacity() == 2);

    const ix_StackVector<int, 2> w{0, 10, 20, 30};
    ix_EXPECT(w.size() == 4);
    ix_EXPECT(w.capacity() == 4);
    ix_EXPECT(w[0] == 0);
    ix_EXPECT(w[1] == 10);
    ix_EXPECT(w[2] == 20);
    ix_EXPECT(w[3] == 30);
}

ix_TEST_CASE("ix_StackVector: move")
{
    { // large pool
        ix_StackVector<ix_DumbString, 8> v1;
        v1.push_back("10");
        v1.push_back("11");
        v1.push_back("12");
        v1.push_back("13");

        ix_StackVector<ix_DumbString, 8> v2(ix_move(v1));
        ix_EXPECT(v2[0] == "10");
        ix_EXPECT(v2[1] == "11");
        ix_EXPECT(v2[2] == "12");
        ix_EXPECT(v2[3] == "13");

        ix_StackVector<ix_DumbString, 8> v3;
        v3 = ix_move(v2);
        ix_EXPECT(v3[0] == "10");
        ix_EXPECT(v3[1] == "11");
        ix_EXPECT(v3[2] == "12");
        ix_EXPECT(v3[3] == "13");

        ix_StackVector<ix_DumbString, 8> v4;
        v4.push_back("100");
        v4.push_back("101");
        v4.push_back("102");
        v4.push_back("103");
        v4 = ix_move(v3);
    }

    { // small pool
        ix_StackVector<ix_DumbString, 2> v1;
        v1.push_back("10");
        v1.push_back("11");
        v1.push_back("12");
        v1.push_back("13");

        ix_StackVector<ix_DumbString, 2> v2(ix_move(v1));
        ix_EXPECT(v2[0] == "10");
        ix_EXPECT(v2[1] == "11");
        ix_EXPECT(v2[2] == "12");
        ix_EXPECT(v2[3] == "13");

        ix_StackVector<ix_DumbString, 2> v3;
        v3 = ix_move(v2);
        ix_EXPECT(v3[0] == "10");
        ix_EXPECT(v3[1] == "11");
        ix_EXPECT(v3[2] == "12");
        ix_EXPECT(v3[3] == "13");

        ix_StackVector<ix_DumbString, 2> v4;
        v4.push_back("100");
        v4.push_back("101");
        v4.push_back("102");
        v4.push_back("103");
        v4 = ix_move(v3);
    }

    { // self move
        ix_StackVector<ix_DumbString, 2> v;

        ix_DISABLE_CLANG_WARNING_BEGIN
        ix_DISABLE_CLANG_WARNING("-Wself-move")

        v = ix_move(v);

        ix_DISABLE_CLANG_WARNING_END
    }
}
#endif
