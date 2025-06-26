#include "ix_Vector_util.hpp"

#if ix_DO_TEST
#include "ix_DumbString.hpp"
#include "ix_StackVector.hpp"
#include "ix_test.hpp"

template <template <typename... Args> class Vector>
static void test_Vector_insert()
{
    // 0 element, int
    {
        Vector<int> v{};
        ix_Vector_insert(v, v.begin(), 100);
        ix_EXPECT(v == Vector<int>{100});
    }

    // 1 element, int
    {
        Vector<int> v;

        v = {10};
        ix_Vector_insert(v, v.begin(), 100);
        ix_EXPECT(v == Vector<int>{100, 10});

        v = {10};
        ix_Vector_insert(v, v.end(), 100);
        ix_EXPECT(v == Vector<int>{10, 100});
    }

    // 2 element, int
    {
        Vector<int> v;

        v = {10, 100};
        ix_Vector_insert(v, v.begin(), 50);
        ix_EXPECT(v == Vector<int>{50, 10, 100});

        v = {10, 100};
        ix_Vector_insert(v, v.begin() + 1, 50);
        ix_EXPECT(v == Vector<int>{10, 50, 100});

        v = {10, 100};
        ix_Vector_insert(v, v.begin() + 2, 50);
        ix_EXPECT(v == Vector<int>{10, 100, 50});
    }

    // 0 element, ix_DumbString
    {
        Vector<ix_DumbString> v{};
        ix_Vector_insert(v, v.begin(), ix_DumbString("100"));
        ix_EXPECT(v == Vector<ix_DumbString>{"100"});
    }

    // 1 element, ix_DumbString
    {
        Vector<ix_DumbString> v;

        v = {"10"};
        ix_Vector_insert(v, v.begin(), ix_DumbString("100"));
        ix_EXPECT(v == Vector<ix_DumbString>{"100", "10"});

        v = {"10"};
        ix_Vector_insert(v, v.end(), ix_DumbString("100"));
        ix_EXPECT(v == Vector<ix_DumbString>{"10", "100"});
    }

    // 2 element, ix_DumbString
    {
        Vector<ix_DumbString> v;

        v = {"10", "100"};
        ix_Vector_insert(v, v.begin(), ix_DumbString("50"));
        ix_EXPECT(v == Vector<ix_DumbString>{"50", "10", "100"});

        v = {"10", "100"};
        ix_Vector_insert(v, v.begin() + 1, ix_DumbString("50"));
        ix_EXPECT(v == Vector<ix_DumbString>{"10", "50", "100"});

        v = {"10", "100"};
        ix_Vector_insert(v, v.begin() + 2, ix_DumbString("50"));
        ix_EXPECT(v == Vector<ix_DumbString>{"10", "100", "50"});
    }

    // range, int
    {
        Vector<int> v;
        Vector<int> w;

        v = {};
        w = {};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{});

        v = {};
        w = {0, 1, 2};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{0, 1, 2});

        v = {0, 1, 2};
        w = {100};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{100, 0, 1, 2});

        v = {0, 1, 2};
        w = {100};
        ix_Vector_insert(v, v.begin() + 1, w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{0, 100, 1, 2});

        v = {0, 1, 2};
        w = {100};
        ix_Vector_insert(v, v.begin() + 2, w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{0, 1, 100, 2});

        v = {0, 1, 2};
        w = {100};
        ix_Vector_insert(v, v.begin() + 3, w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{0, 1, 2, 100});

        v = {100};
        w = {0, 1, 2};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{0, 1, 2, 100});

        v = {100};
        w = {0, 1, 2};
        ix_Vector_insert(v, v.begin() + 1, w.begin(), w.end());
        ix_EXPECT(v == Vector<int>{100, 0, 1, 2});
    }

    // range, ix_DumbString
    {
        Vector<ix_DumbString> v;
        Vector<ix_DumbString> w;

        v = {};
        w = {};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{});

        v = {};
        w = {"0", "1", "2"};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"0", "1", "2"});

        v = {"0", "1", "2"};
        w = {"100"};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"100", "0", "1", "2"});

        v = {"0", "1", "2"};
        w = {"100"};
        ix_Vector_insert(v, v.begin() + 1, w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"0", "100", "1", "2"});

        v = {"0", "1", "2"};
        w = {"100"};
        ix_Vector_insert(v, v.begin() + 2, w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"0", "1", "100", "2"});

        v = {"0", "1", "2"};
        w = {"100"};
        ix_Vector_insert(v, v.begin() + 3, w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"0", "1", "2", "100"});

        v = {"100"};
        w = {"0", "1", "2"};
        ix_Vector_insert(v, v.begin(), w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"0", "1", "2", "100"});

        v = {"100"};
        w = {"0", "1", "2"};
        ix_Vector_insert(v, v.begin() + 1, w.begin(), w.end());
        ix_EXPECT(v == Vector<ix_DumbString>{"100", "0", "1", "2"});
    }
}

template <template <typename... Args> class Vector>
static void test_Vector_erase()
{
    // int
    {
        Vector<int> v;

        v = {10};
        ix_Vector_erase(v, v.begin());
        ix_EXPECT(v == Vector<int>{});

        v = {10, 100};
        ix_Vector_erase(v, v.begin());
        ix_EXPECT(v == Vector<int>{100});

        v = {10, 100};
        ix_Vector_erase(v, v.begin() + 1);
        ix_EXPECT(v == Vector<int>{10});

        v = {10, 50, 100};
        ix_Vector_erase(v, v.begin());
        ix_EXPECT(v == Vector<int>{50, 100});

        v = {10, 50, 100};
        ix_Vector_erase(v, v.begin() + 1);
        ix_EXPECT(v == Vector<int>{10, 100});

        v = {10, 50, 100};
        ix_Vector_erase(v, v.begin() + 1);
        ix_EXPECT(v == Vector<int>{10, 100});
    }

    // ix_DumbString
    {
        Vector<ix_DumbString> v;

        v.push_back("1");
        ix_Vector_erase(v, v.begin());
        ix_EXPECT(v.size() == 0);

        v.clear();
        v.push_back("1");
        v.push_back("2");
        ix_Vector_erase(v, v.begin());
        ix_EXPECT(v.size() == 1);
        ix_EXPECT(v[0] == "2");

        v.clear();
        v.push_back("1");
        v.push_back("2");
        ix_Vector_erase(v, v.begin() + 1);
        ix_EXPECT(v.size() == 1);
        ix_EXPECT(v[0] == "1");

        v.clear();
        v.push_back("1");
        v.push_back("3");
        v.push_back("2");
        ix_Vector_erase(v, v.begin());
        ix_EXPECT(v.size() == 2);
        ix_EXPECT(v[0] == "3");
        ix_EXPECT(v[1] == "2");

        v.clear();
        v.push_back("1");
        v.push_back("3");
        v.push_back("2");
        ix_Vector_erase(v, v.begin() + 1);
        ix_EXPECT(v.size() == 2);
        ix_EXPECT(v[0] == "1");
        ix_EXPECT(v[1] == "2");

        v.clear();
        v.push_back("1");
        v.push_back("3");
        v.push_back("2");
        ix_Vector_erase(v, v.begin() + 2);
        ix_EXPECT(v.size() == 2);
        ix_EXPECT(v[0] == "1");
        ix_EXPECT(v[1] == "3");
    }

    { // erase(p, n)
        Vector<int> v{1, 2, 3, 4};

        ix_Vector_erase(v, &v[1], 0);
        ix_EXPECT(v == Vector<int>{1, 2, 3, 4});

        ix_Vector_erase(v, &v[1], 2);
        ix_EXPECT(v == Vector<int>{1, 4});

        ix_Vector_erase(v, &v[1], 1);
        ix_EXPECT(v == Vector<int>{1});

        ix_Vector_erase(v, &v[0], 1);
        ix_EXPECT(v == Vector<int>{});
    }
}

template <typename T>
using ix_StackVector2 = ix_StackVector<T, 2>;

template <typename T>
using ix_StackVector32 = ix_StackVector<T, 32>;

ix_TEST_CASE("ix_Vector_insert")
{
    test_Vector_insert<ix_Vector>();
    test_Vector_insert<ix_StackVector2>();
    test_Vector_insert<ix_StackVector32>();

    test_Vector_erase<ix_Vector>();
    test_Vector_erase<ix_StackVector2>();
    test_Vector_erase<ix_StackVector32>();
}
#endif
