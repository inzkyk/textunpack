#include "ix_HashSet.hpp"

#if ix_DO_TEST
#include "ix_DumbInt.hpp"
#include "ix_DumbString.hpp"
#include "ix_Writer.hpp"
#include "ix_test.hpp"

template <>
struct ix_Print<ix_HashSet<ix_DumbString>>
{
    static constexpr bool is_implemented = true;
    static size_t print(ix_Writer &writer, const ix_HashSet<ix_DumbString> &set)
    {
        writer.write_string("[");
        size_t length = 1;
        size_t num = 0;
        for (const ix_DumbString &s : set)
        {
            if (num != 0)
            {
                writer.write_string(", ");
                length += 2;
            }
            const size_t s_length = ix_strlen(s.data());
            writer.write(s.data(), s_length);
            length += s_length;
            num += 1;
        }
        writer.write_string("]");
        length += 1;
        return length;
    }
};

struct DumbHash
{
    ix_FORCE_INLINE static constexpr size_t hash(ix_DumbInt256)
    {
        return 0;
    }
};

struct EqHash256
{
    ix_FORCE_INLINE static size_t hash(const ix_DumbInt256 &x)
    {
        return static_cast<size_t>(x.v[0]);
    }
};

ix_TEST_CASE("ix_HashSet: uint64_t")
{
    ix_HashSet<uint64_t> s;
    ix_EXPECT(s.empty());
    ix_EXPECT(!s.contains(9));
    s.insert(1);
    ix_EXPECT(s.size() == 1);
    ix_EXPECT(!s.contains(9));
    ix_EXPECT(s.contains(1));
    s.erase(9);
    ix_EXPECT(s.size() == 1);
    ix_EXPECT(!s.contains(9));
    ix_EXPECT(s.contains(1));
    s.erase(1);
    ix_EXPECT(s.empty());
    ix_EXPECT(!s.contains(9));
    ix_EXPECT(!s.contains(1));

    constexpr uint64_t N = 256;
    s.clear();
    for (uint64_t i = 0; i < N; i++)
    {
        s.insert(i);
    }

    for (uint64_t i = 0; i < N; i++)
    {
        ix_EXPECT(s.contains(i));
    }

    s.clear();
    constexpr uint64_t M = 1024;
    for (uint64_t i = 0; i < M; i++)
    {
        s.insert(i);
        s.erase((i * 257) % M);
    }

    ix_EXPECT(s.size() == 384);
}

ix_TEST_CASE("ix_HashSet: ix_DumbInt256")
{
    ix_HashSet<ix_DumbInt256> s;
    ix_EXPECT(s.empty());
    ix_EXPECT(!s.contains(9));
    s.insert(1);
    ix_EXPECT(s.size() == 1);
    ix_EXPECT(!s.contains(9));
    ix_EXPECT(s.contains(1));
    s.erase(9);
    ix_EXPECT(s.size() == 1);
    ix_EXPECT(!s.contains(9));
    ix_EXPECT(s.contains(1));
    s.erase(1);
    ix_EXPECT(s.empty());
    ix_EXPECT(!s.contains(9));
    ix_EXPECT(!s.contains(1));

    constexpr uint64_t N = 256;
    s.clear();
    for (uint64_t i = 0; i < N; i++)
    {
        s.insert(i);
    }

    for (uint64_t i = 0; i < N; i++)
    {
        ix_EXPECT(s.contains(i));
    }

    s.clear();
    constexpr uint64_t M = 1024;
    for (uint64_t i = 0; i < M; i++)
    {
        s.insert(i);
        s.erase((i * 257) % M);
    }

    ix_EXPECT(s.size() == 384);
}

ix_TEST_CASE("ix_HashSet: capacity")
{
    ix_HashSet<uint64_t> s;
    ix_EXPECT(s.capacity() == 0);
    s.insert(1);
    ix_EXPECT(s.capacity() == 6);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    s.insert(5);
    s.insert(6);
    ix_EXPECT(s.size() == 6);
    ix_EXPECT(s.bucket_count() == 8);
    s.insert(7);
    ix_EXPECT(s.size() == 7);
    ix_EXPECT(s.bucket_count() == 16);
}

ix_TEST_CASE("ix_HashSet: ix_DumbString")
{
    ix_HashSet<ix_DumbString> s;
    ix_EXPECT(s.empty());
    ix_EXPECT(!s.contains("9"));
    s.insert("1");
    ix_EXPECT(s.size() == 1);
    ix_EXPECT(!s.contains("9"));
    ix_EXPECT(s.contains("1"));
    s.erase("9");
    ix_EXPECT(s.size() == 1);
    ix_EXPECT(!s.contains("9"));
    ix_EXPECT(s.contains("1"));
    s.erase("1");
    ix_EXPECT(s.empty());
    ix_EXPECT(!s.contains("9"));
    ix_EXPECT(!s.contains("1"));

    constexpr uint64_t N = 256;
    s.clear();
    for (uint64_t i = 0; i < N; i++)
    {
        s.emplace(static_cast<int>(i));
    }

    for (uint64_t i = 0; i < N; i++)
    {
        ix_EXPECT(s.contains(ix_DumbString(static_cast<int>(i))));
    }
}

ix_TEST_CASE("ix_HashSet: empty")
{
    const ix_HashSet<uint64_t> s;
    ix_EXPECT(s.empty());
}

ix_TEST_CASE("ix_HashSet: range-based for")
{
    constexpr uint64_t N = 256;
    ix_HashSet<uint64_t> s;
    for (uint64_t i = 0; i < N; i++)
    {
        s.insert(i * i);
    }

    uint64_t num = 0;
    for (const uint64_t x : s)
    {
        num += 1;
        ix_EXPECT(s.contains(x));
    }

    ix_EXPECT(num == N);
    ix_EXPECT(num == s.size());
}

ix_TEST_CASE("ix_HashSet: equality")
{
    ix_EXPECT(ix_HashSet<int>{} == ix_HashSet<int>{});          // NOLINT(readability-container-size-empty)
    ix_EXPECT(ix_HashSet<int>{} != (ix_HashSet<int>{1, 2, 3})); // NOLINT(readability-container-size-empty)
    ix_EXPECT(ix_HashSet<int>{1, 2, 3} == ix_HashSet<int>{1, 2, 3});
    ix_EXPECT((ix_HashSet<int>{1, 5, 3}) != (ix_HashSet<int>{1, 2, 3}));
    ix_EXPECT(
        ix_HashSet<ix_DumbString>{"1", "2", "3", "4", "5"} == ix_HashSet<ix_DumbString>{"1", "2", "3", "4", "5"}
    );
}

ix_TEST_CASE("ix_HashSet: ix_StringHashSet")
{
    ix_StringHashSet m;
    m.insert("hello");
    m.insert("world");
    m.insert("foo");
    m.insert("bar");
    ix_EXPECT(m.size() == 4);
    ix_EXPECT(m.contains(ix_DumbString("hello").data()));
    ix_EXPECT(m.contains(ix_DumbString("world").data()));
    ix_EXPECT(m.contains(ix_DumbString("foo").data()));
    ix_EXPECT(m.contains(ix_DumbString("bar").data()));
    ix_EXPECT(!m.contains(ix_DumbString("foobar").data()));
}

ix_TEST_CASE("ix_HashSet: many collision")
{
    ix_HashSet<ix_DumbInt256, DumbHash> m;
    constexpr int N = 512;
    for (int i = 0; i < N; i++)
    {
        m.insert(i);
    }
    for (int i = 0; i < N; i++)
    {
        ix_EXPECT(m.contains(i));
    }
}

ix_TEST_CASE("ix_HashSet: contains")
{
    ix_HashSet<ix_DumbInt256> m;

    constexpr uint64_t N = 32;
    for (uint64_t i = 0; i < N; i++)
    {
        m.insert(i);
    }
    for (uint64_t i = 0; i < N; i++)
    {
        m.insert(i);
    }
    ix_EXPECT(m.size() == N);
    for (uint64_t i = 0; i < N; i++)
    {
        ix_EXPECT(m.contains(i));
        ix_EXPECT(!m.contains(i + N));
    }
}

ix_TEST_CASE("ix_HashSet: edge cases")
{
    ix_HashSet<ix_DumbInt256, EqHash256> m;
    ix_EXPECT(!m.contains(1));
    m.erase(1);

    m.insert(5);
    m.insert(6);
    m.insert(7);
    m.insert(13);
    m.erase(5 + 8);
    ix_EXPECT(m.contains(5));
    ix_EXPECT(m.contains(6));
    ix_EXPECT(m.contains(7));
    ix_EXPECT(!m.contains(13));
}

ix_TEST_CASE("ix_HashSet: reserve() when threre's elements.")
{
    ix_HashSet<uint64_t> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    s.reserve(100);
    ix_EXPECT(s.size() == 4);
    ix_EXPECT(s.capacity() >= 100);
    ix_EXPECT(s.contains(1));
    ix_EXPECT(s.contains(2));
    ix_EXPECT(s.contains(3));
    ix_EXPECT(s.contains(4));
}

ix_TEST_CASE("ix_HashSet: back/erase_back")
{
    ix_HashSet<uint64_t> s;
    s.insert(10);
    s.insert(20);
    s.insert(30);
    s.insert(40);
    const uint64_t x = s.back();
    ix_EXPECT(s.contains(x));
    s.erase(x);
    ix_EXPECT(!s.contains(x));
    ix_EXPECT(s.contains(s.back()));
}
#endif
