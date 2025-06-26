#pragma once

#include "ix.hpp"
#include "ix_DumbInt.hpp"
#include "ix_DumbString.hpp"
#include "ix_random.hpp"

template <template <typename...> typename HashMap, int N>
void ix_test_Map_basic()
{
    HashMap<int, int> m;
    for (int i = 0; i < N; i++)
    {
        m.emplace(i, i * i);
    }
    for (int i = 0; i < N; i++)
    {
        ix_EXPECT(m[i] == i * i);
    }
    ix_EXPECT(m.size() == N);
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_value_update()
{
    HashMap<int, int> m;
    for (int i = 0; i < N; i++)
    {
        m.emplace(i, i);
    }

    for (int i = 0; i < N; i++)
    {
        m.emplace(i, i * i);
    }

    for (int i = 0; i < N; i++)
    {
        ix_EXPECT(m[i] == i * i);
    }

    ix_EXPECT(m.size() == N);
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_normal_hash_with_large_key()
{
    HashMap<ix_DumbInt256, ix_DumbString> m;
    for (int i = 0; i < N / 2; i++)
    {
        m.emplace(i, ix_DumbString(i * i));
    }

    for (int i = N / 2; i < N; i++)
    {
        m.emplace(i, i * i);
    }

    for (int i = 0; i < N; i++)
    {
        ix_EXPECT(m[i] == ix_DumbString(i * i));
        ix_EXPECT(m.find(i) != nullptr);
    }
    ix_EXPECT(m.size() == N);
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_non_trivial_type()
{
    HashMap<ix_DumbString, ix_DumbString> m;

    m.emplace("0", "{0, 0}");
    m.emplace("1", "{1, 1}");
    m.emplace("2", "{2, 2}");
    m.emplace("3", "{3, 3}");
    m.emplace("4", "{4, 4}");
    m.emplace("5", "{5, 5}");
    ix_EXPECT(m["0"] == "{0, 0}");
    ix_EXPECT(m["1"] == "{1, 1}");
    ix_EXPECT(m["2"] == "{2, 2}");
    ix_EXPECT(m["3"] == "{3, 3}");
    ix_EXPECT(m["4"] == "{4, 4}");
    ix_EXPECT(m["5"] == "{5, 5}");

    ix_EXPECT(m.size() == 6);
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_emplace()
{
    HashMap<ix_DumbString, ix_DumbString> m;

    ix_DumbString s;
    ix_DumbString v;

    m.emplace("1", "{1, 1}");
    ix_EXPECT(m["1"] == "{1, 1}");
    ix_EXPECT(m.size() == 1);

    v = "{2, 2}";
    m.emplace("2", v);
    ix_EXPECT(m["2"] == v);
    ix_EXPECT(m.size() == 2);

    s = "3";
    m.emplace(s, "{3, 3}");
    ix_EXPECT(m[s] == "{3, 3}");
    ix_EXPECT(m.size() == 3);

    s = "4";
    v = "{4, 4}";
    m.emplace(s, v);
    ix_EXPECT(m[s] == v);
    ix_EXPECT(m.size() == 4);

    v = "{4, 4, 4, 4}";
    m.emplace(s, v);
    ix_EXPECT(m[s] == v);
    ix_EXPECT(m.size() == 4);
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_insert()
{
    HashMap<ix_DumbString, ix_DumbString> m;

    ix_DumbString s;
    ix_DumbString v;

    m.insert("1", "{1, 1}");
    ix_EXPECT(m["1"] == "{1, 1}");
    ix_EXPECT(m.size() == 1);

    v = "{2, 2}";
    m.insert("2", v);
    ix_EXPECT(m["2"] == v);
    ix_EXPECT(m.size() == 2);

    s = "3";
    m.insert(s, "{3, 3}");
    ix_EXPECT(m[s] == "{3, 3}");
    ix_EXPECT(m.size() == 3);

    s = "4";
    v = "{4, 4}";
    m.insert(s, v);
    ix_EXPECT(m[s] == v);
    ix_EXPECT(m.size() == 4);

    v = "{4, 4, 4, 4}";
    m.emplace(s, v);
    ix_EXPECT(m[s] == v);
    ix_EXPECT(m.size() == 4);
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_brace_initialization()
{
    {
        HashMap<int, int> m{
            {1, 11},
            {2, 22},
            {3, 33},
            {4, 44},
        };

        ix_EXPECT(m.size() == 4);
        ix_EXPECT(m[1] == 11);
        ix_EXPECT(m[2] == 22);
        ix_EXPECT(m[3] == 33);
        ix_EXPECT(m[4] == 44);
    }

    {
        HashMap<ix_DumbString, ix_DumbString> m{
            {"1", "{1, 1}"},
            {"2", "{2, 2}"},
            {"3", "{3, 3}"},
            {"4", "{4, 4}"},
        };

        ix_EXPECT(m.size() == 4);
        ix_EXPECT(m["1"] == "{1, 1}");
        ix_EXPECT(m["2"] == "{2, 2}");
        ix_EXPECT(m["3"] == "{3, 3}");
        ix_EXPECT(m["4"] == "{4, 4}");
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_move()
{
    // move constructor
    {
        HashMap<ix_DumbString, ix_DumbString> m0{
            {"1", "{1, 1}"},
            {"2", "{2, 2}"},
            {"3", "{3, 3}"},
            {"4", "{4, 4}"},
        };

        HashMap<ix_DumbString, ix_DumbString> m1 = ix_move(m0);
        ix_EXPECT(m1.size() == 4);
        ix_EXPECT(m1["1"] == "{1, 1}");
        ix_EXPECT(m1["2"] == "{2, 2}");
        ix_EXPECT(m1["3"] == "{3, 3}");
        ix_EXPECT(m1["4"] == "{4, 4}");
    }

    // move assignment
    {
        HashMap<ix_DumbString, ix_DumbString> m0{
            {"1", "{1, 1}"},
            {"2", "{2, 2}"},
            {"3", "{3, 3}"},
            {"4", "{4, 4}"},
        };

        HashMap<ix_DumbString, ix_DumbString> m1{
            {"foo", "{3, 1, 4}"},
        };

        m1 = ix_move(m0);

        ix_EXPECT(m1.size() == 4);
        ix_EXPECT(m1["1"] == "{1, 1}");
        ix_EXPECT(m1["2"] == "{2, 2}");
        ix_EXPECT(m1["3"] == "{3, 3}");
        ix_EXPECT(m1["4"] == "{4, 4}");
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_contains()
{
    HashMap<int, int> m;
    for (int i = 0; i < N; i++)
    {
        m.emplace(i, i * i);
    }
    for (int i = 0; i < N; i++)
    {
        ix_EXPECT(m.contains(i));
        ix_EXPECT(!m.contains(i + N));
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_erase()
{
    {
        HashMap<int, int> m;

        m.erase(0);
        ix_EXPECT(m.size() == 0);

        m.clear();
        m.emplace(1, 11);
        m.erase(1);
        ix_EXPECT(!m.contains(1));
        ix_EXPECT(m.size() == 0);

        m.clear();
        m.emplace(1, 11);
        m.emplace(2, 22);
        m.emplace(3, 33);
        m.erase(1);
        ix_EXPECT(m.size() == 2);
        ix_EXPECT(!m.contains(1));
        ix_EXPECT(m[2] == 22);
        ix_EXPECT(m[3] == 33);

        m.clear();
        m.emplace(1, 11);
        m.emplace(2, 22);
        m.emplace(3, 33);
        m.erase(2);
        ix_EXPECT(m.size() == 2);
        ix_EXPECT(!m.contains(2));
        ix_EXPECT(m[1] == 11);
        ix_EXPECT(m[3] == 33);

        m.clear();
        m.emplace(1, 11);
        m.emplace(2, 22);
        m.emplace(3, 33);
        m.erase(3);
        ix_EXPECT(m.size() == 2);
        ix_EXPECT(!m.contains(3));
        ix_EXPECT(m[1] == 11);
        ix_EXPECT(m[2] == 22);
    }

    // many erase
    {
        HashMap<int, int> m;

        for (int i = 0; i < N; i++)
        {
            m.emplace(i, i);
        }

        int buf[N];
        ix_rand_fill_unique_range<int>(buf, N, 0, N - 1);
        for (int i = 0; i < N; i++)
        {
            m.erase(buf[i]);
        }

        ix_EXPECT(m.size() == 0);
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_empty()
{
    HashMap<int, int> m;
    ix_EXPECT(m.empty());
    m.emplace(0, 10);
    ix_EXPECT(!m.empty());
    m.clear();
    ix_EXPECT(m.empty());
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_insert_by_move()
{
    { // Basic.
        HashMap<ix_DumbString, ix_DumbString> m;
        const ix_DumbString v0 = "{0, 1, 2}";
        const ix_DumbString v1 = "{0, 1, 2}";
        m.insert(v0, v1);
        ix_EXPECT(m.size() == 1);
    }

    { // Move key.
        HashMap<ix_DumbString, ix_DumbString> m;
        ix_DumbString v0 = "{0, 1, 2}";
        const ix_DumbString v1 = "{0, 1, 2}";
        m.insert(ix_move(v0), v1);
        ix_EXPECT(m.size() == 1);
    }

    { // Move value.
        HashMap<ix_DumbString, ix_DumbString> m;
        const ix_DumbString v0 = "{0, 1, 2}";
        ix_DumbString v1 = "{0, 1, 2}";
        m.insert(v0, ix_move(v1));
        ix_EXPECT(m.size() == 1);
    }

    { // Move both.
        HashMap<ix_DumbString, ix_DumbString> m;
        ix_DumbString v0 = "{0, 1, 2}";
        ix_DumbString v1 = "{0, 1, 2}";
        m.insert(ix_move(v0), ix_move(v1));
        ix_EXPECT(m.size() == 1);
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_emplace_by_move()
{
    { // Basic.
        HashMap<ix_DumbString, ix_DumbString> m;
        const ix_DumbString v0 = "{0, 1, 2}";
        const ix_DumbString v1 = "{0, 1, 2}";
        m.emplace(v0, v1);
        ix_EXPECT(m.size() == 1);
    }

    { // Move key.
        HashMap<ix_DumbString, ix_DumbString> m;
        ix_DumbString v0 = "{0, 1, 2}";
        const ix_DumbString v1 = "{0, 1, 2}";
        m.emplace(ix_move(v0), v1);
        ix_EXPECT(m.size() == 1);
    }

    { // Move value.
        HashMap<ix_DumbString, ix_DumbString> m;
        const ix_DumbString v0 = "{0, 1, 2}";
        ix_DumbString v1 = "{0, 1, 2}";
        m.emplace(v0, ix_move(v1));
        ix_EXPECT(m.size() == 1);
    }

    { // Move both.
        HashMap<ix_DumbString, ix_DumbString> m;
        ix_DumbString v0 = "{0, 1, 2}";
        ix_DumbString v1 = "{0, 1, 2}";
        m.emplace(ix_move(v0), ix_move(v1));
        ix_EXPECT(m.size() == 1);
    }

    // reserve() when threre's elements.
    {
        HashMap<int, int> m;
        m.emplace(1, 11);
        m.emplace(2, 22);
        m.emplace(3, 33);
        m.emplace(4, 44);
        m.reserve(100);
        ix_EXPECT(m.size() == 4);
        ix_EXPECT(m[1] == 11);
        ix_EXPECT(m[2] == 22);
        ix_EXPECT(m[3] == 33);
        ix_EXPECT(m[4] == 44);
        ix_EXPECT(m.capacity() >= 100);
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_get_or()
{
    // returns value
    {
        HashMap<int, int> m;
        m.emplace(1, 11);
        m.emplace(2, 22);
        m.emplace(3, 33);
        m.emplace(4, 44);
        ix_EXPECT(m.get_or(1, 100) == 11);
        ix_EXPECT(m.get_or(2, 100) == 22);
        ix_EXPECT(m.get_or(3, 100) == 33);
        ix_EXPECT(m.get_or(4, 100) == 44);
        ix_EXPECT(m.get_or(0, 100) == 100);
        ix_EXPECT(m.get_or(5, 100) == 100);
        ix_EXPECT(m.get_or(100, 100) == 100);
    }

    // returns const reference
    {
        HashMap<uint64_t, ix_DumbString> m;
        m.emplace(1, "{1}");
        m.emplace(2, "{2, 2}");
        m.emplace(3, "{3, 3, 3}");
        m.emplace(4, "{4, 4, 4, 4}");
        const ix_DumbString default_value = "{0, 1, 2, 3}";
        ix_EXPECT(m.get_or(1, default_value) == "{1}");
        ix_EXPECT(m.get_or(2, default_value) == "{2, 2}");
        ix_EXPECT(m.get_or(3, default_value) == "{3, 3, 3}");
        ix_EXPECT(m.get_or(4, default_value) == "{4, 4, 4, 4}");
        ix_EXPECT(m.get_or(0, default_value) == default_value);
        ix_EXPECT(m.get_or(5, default_value) == default_value);
    }
}

template <template <typename...> typename HashMap, int N>
void ix_test_Map_iterator()
{
    // normal iterator
    {
        HashMap<uint64_t, ix_DumbString> m;
        m.emplace(1, "{1}");
        m.emplace(2, "{2, 2}");
        m.emplace(3, "{3, 3, 3}");
        m.emplace(4, "{4, 4, 4, 4}");

        {
            auto it = m.find(10);
            ix_EXPECT(it == nullptr);
            it = m.find(1);
            ix_EXPECT(it != nullptr);
            ix_ASSERT(it != nullptr); // Suppress clang-tidy warning
            ix_EXPECT(*it == "{1}");
            ix_DumbString new_value = "{11}";
            *it = new_value;
            ix_EXPECT(m[1] == new_value);
        }

        {
            const auto *it = m.find(1);
            ix_EXPECT(it != nullptr);
            ix_ASSERT(it != nullptr); // Suppress MSVC warning
            ix_EXPECT(*it == "{11}");
        }
    }

    // const iterator
    {
        const HashMap<int, int> m{{1, 1}, {2, 22}, {3, 333}, {4, 4444}};

        auto it = m.find(10);
        ix_EXPECT(it == nullptr);

        it = m.find(3);
        ix_EXPECT(it != nullptr);
        ix_ASSERT(it != nullptr); // Suppress MSVC warning
        ix_EXPECT(*it == 333);
    }
}

template <template <typename...> typename Map, int N>
void ix_test_Map()
{
    ix_test_Map_basic<Map, N>();
    ix_test_Map_value_update<Map, N>();
    ix_test_Map_normal_hash_with_large_key<Map, N>();
    ix_test_Map_non_trivial_type<Map, N>();
    ix_test_Map_emplace<Map, N>();
    ix_test_Map_insert<Map, N>();
    ix_test_Map_brace_initialization<Map, N>();
    ix_test_Map_move<Map, N>();
    ix_test_Map_contains<Map, N>();
    ix_test_Map_erase<Map, N>();
    ix_test_Map_empty<Map, N>();
    ix_test_Map_insert_by_move<Map, N>();
    ix_test_Map_emplace_by_move<Map, N>();
    ix_test_Map_get_or<Map, N>();

    // ix_hashMapDuobleArray does not provide begin()/end().
    // ix_test_Map_iterator<Map, N>();
}
