#pragma once

#include "ix.hpp"
#include "ix_DumbString.hpp"
#include "ix_test.hpp"

template <template <typename... Args> class Vector>
void ix_test_Vector()
{
    {
        const Vector<ix_DumbString> v(10);
        ix_UNUSED(v);
    }

    // push_back/operator[]
    {
        Vector<uint64_t> v;

        v.push_back(0);
        v.push_back(10);
        v.push_back(20);
        v.push_back(30);
        ix_EXPECT(v.size() == 4);
        ix_EXPECT(v.capacity() >= 4);

        ix_EXPECT(v[0] == 0);
        ix_EXPECT(v[1] == 10);
        ix_EXPECT(v[2] == 20);
        ix_EXPECT(v[3] == 30);

        v[0] += 0;
        v[1] += 100;
        v[2] += 200;
        v[3] += 300;

        ix_EXPECT(v[0] == 0);
        ix_EXPECT(v[1] == 110);
        ix_EXPECT(v[2] == 220);
        ix_EXPECT(v[3] == 330);

        ix_EXPECT(v != Vector<uint64_t>{});
        ix_EXPECT(v == Vector<uint64_t>{0, 110, 220, 330});
    }

    // With ix_DumbString
    {
        Vector<ix_DumbString> v;

        v.push_back(ix_DumbString("0"));
        v.push_back(ix_DumbString("1"));
        v.emplace_back("2");
        v.emplace_back("3");
        ix_EXPECT(v.size() == 4);
        ix_EXPECT(v.capacity() >= 4);

        ix_EXPECT(v[0] == "0");
        ix_EXPECT(v[1] == "1");
        ix_EXPECT(v[2] == "2");
        ix_EXPECT(v[3] == "3");

        v[0].repeat();
        v[1].repeat();
        v[2].repeat();
        v[3].repeat();

        ix_EXPECT(v[0] == "00");
        ix_EXPECT(v[1] == "11");
        ix_EXPECT(v[2] == "22");
        ix_EXPECT(v[3] == "33");
    }

    // Many elements
    {
        Vector<size_t> v;
        constexpr size_t N = 128;
        for (size_t i = 0; i < N; i++)
        {
            v.push_back(i);
        }

        ix_EXPECT(v.size() == N);
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(v[i] == i);
        }
    }

    // reserve()
    {
        Vector<size_t> v;
        constexpr size_t N = 128;

        v.reserve(200);
        ix_EXPECT(v.capacity() == 200);

        for (size_t i = 0; i < N; i++)
        {
            v.push_back(i);
        }

        ix_EXPECT(v.size() == N);
        ix_EXPECT(v.capacity() == 200);

        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(v[i] == i);
        }
    }

    // initializer list
    {
        const Vector<int> v{};
        ix_EXPECT(v.size() == 0);

        const Vector<int> w{0, 10, 20, 30};
        ix_EXPECT(w.size() == 4);
        ix_EXPECT(w[0] == 0);
        ix_EXPECT(w[1] == 10);
        ix_EXPECT(w[2] == 20);
        ix_EXPECT(w[3] == 30);
    }

    // range-based for
    {
        Vector<int> v{0, 10, 20, 30};
        size_t i;

        i = 0;
        for (int x : v)
        {
            ix_EXPECT(v[i] == x);
            i += 1;
        }
        ix_EXPECT(i == 4);

        i = 0;
        for (int &x : v)
        {
            x += 10;
            i += 1;
        }
        ix_EXPECT(i == 4);

        ix_EXPECT(v[0] == 10);
        ix_EXPECT(v[1] == 20);
        ix_EXPECT(v[2] == 30);
        ix_EXPECT(v[3] == 40);
    }

    // move
    {
        Vector<ix_DumbString> v0;
        v0.emplace_back("0");
        v0.emplace_back("1");
        v0.emplace_back("2");
        v0.emplace_back("3");

        Vector<ix_DumbString> v1(ix_move(v0));

        ix_EXPECT(v1[0] == "0");
        ix_EXPECT(v1[1] == "1");
        ix_EXPECT(v1[2] == "2");
        ix_EXPECT(v1[3] == "3");

        Vector<ix_DumbString> v2;
        v2.emplace_back("10");
        v2.emplace_back("10");
        v2.emplace_back("10");
        v2.emplace_back("10");

        v2 = ix_move(v1);

        ix_EXPECT(v2[0] == "0");
        ix_EXPECT(v2[1] == "1");
        ix_EXPECT(v2[2] == "2");
        ix_EXPECT(v2[3] == "3");

        ix_DISABLE_CLANG_WARNING_BEGIN
        ix_DISABLE_CLANG_WARNING("-Wself-assign-overloaded")

        v2 = ix_move(v2);
        ix_EXPECT(v2[0] == "0");
        ix_EXPECT(v2[1] == "1");
        ix_EXPECT(v2[2] == "2");
        ix_EXPECT(v2[3] == "3");
        Vector<ix_DumbString> v3(ix_move(v2));
        ix_EXPECT(v3[0] == "0");
        ix_EXPECT(v3[1] == "1");
        ix_EXPECT(v3[2] == "2");
        ix_EXPECT(v3[3] == "3");

        ix_DISABLE_CLANG_WARNING_END
    }

    // resize
    {
        Vector<ix_DumbString> v;
        v.push_back("foo");
        v.push_back("bar");
        v.resize(10);
        ix_EXPECT(v.size() == 10);
        v[5] = "1";
        v[8] = "2";
        v.resize(1);
        ix_EXPECT(v.size() == 1);
    }

    // resize (with value)
    {
        Vector<ix_DumbString> v;
        v.push_back("foo");
        v.push_back("foo");
        v.resize(10, "foo");
        ix_EXPECT(v.size() == 10);
        for (size_t i = 0; i < 10; i++)
        {
            ix_EXPECT(v[i] == "foo");
        }
        v[5] = "1";
        v[8] = "2";
        v.resize(1, "bar");
        ix_EXPECT(v.size() == 1);
    }

    // resize (default constructor)
    {
        struct Foo
        {
            int x = 10;
            int y = 20;
        };

        Vector<Foo> v;
        const size_t N = 3;
        v.resize(N);
        for (size_t i = 0; i < N; i++)
        {
            ix_EXPECT(v[i].x == 10);
            ix_EXPECT(v[i].y == 20);
        }
    }

    // equality
    {
        Vector<int> a{};
        Vector<int> b{};
        Vector<int> c{0};
        Vector<int> d{1};
        Vector<int> e{0, 1};
        ix_EXPECT(a == a);
        ix_EXPECT(b == b);
        ix_EXPECT(b == a);
        ix_EXPECT(c != d);
        ix_EXPECT(e != a);
        ix_EXPECT(e != b);
        ix_EXPECT(e != c);
        ix_EXPECT(e != d);
    }

    // nested
    {
        Vector<Vector<size_t>> v;

        for (size_t i = 0; i < 16; i++)
        {
            Vector<size_t> u;
            for (size_t j = 0; j < i; j++)
            {
                u.push_back(j);
            }
            v.emplace_back(ix_move(u));
        }
    }

    // padded
    {
        struct S
        {
            uint8_t v8;
            uint32_t v32;
        };

        Vector<S> v;

        v.resize(16);

        size_t c = 0;
        for (size_t i = 0; i < 16; i++)
        {
            v.emplace_back(S{static_cast<uint8_t>(c), static_cast<uint32_t>(c)});
        }
    }

    // set_size
    {
        Vector<int> v;
        v.set_size(100);
        ix_EXPECT(v.size() == 100);
        v.set_size(0);
        ix_EXPECT(v.size() == 0);
    }

    // swap_erase
    {
        Vector<int> v;
        v.push_back(10);
        v.push_back(20);
        v.push_back(30);
        v.push_back(40);
        v.swap_erase(1);
        ix_EXPECT(v == Vector<int>{10, 40, 30});
        v.swap_erase(0);
        ix_EXPECT(v == Vector<int>{30, 40});
        v.swap_erase(1);
        ix_EXPECT(v == Vector<int>{30});
    }

    { // back/pop_back
        Vector<int> v{1, 2, 3, 4};
        ix_EXPECT(v.back() == 4);
        v.pop_back();
        ix_EXPECT(v.size() == 3);
        ix_EXPECT(v.back() == 3);
        v.pop_back();
        v.pop_back();
        ix_EXPECT(v.size() == 1);
        ix_EXPECT(v.back() == 1);
        v.pop_back();
        ix_EXPECT(v.empty());
    }
}
