#include "ix_StringView.hpp"
#include "ix_Option.hpp"
#include "ix_Writer.hpp"

size_t ix_Print<ix_StringView>::print(ix_Writer &writer, const ix_StringView &sv)
{
    const size_t length = sv.length();
    writer.write(sv.data(), length);
    return length;
}

#if ix_DO_TEST
#include "ix_hash.hpp"
#include "ix_test.hpp"

ix_TEST_CASE("ix_StringView::ix_StringView")
{
    // from a string literal
    {
        const ix_StringView empty("");
        ix_EXPECT_EQSTR(empty.data(), "");
        ix_EXPECT(empty.length() == 0);

        const ix_StringView hello("hello");
        ix_EXPECT_EQSTR(hello.data(), "hello");
        ix_EXPECT(hello.length() == 5);
    }

    // from a string buffer
    {
        const char *s = "hello world";
        const ix_StringView hello(s, s + 5);
        ix_EXPECT(hello.length() == 5);
        ix_EXPECT(hello == ix_StringView("hello"));
        const ix_StringView world(s + 6, s + 11);
        ix_EXPECT(world.length() == 5);
        ix_EXPECT(world == ix_StringView("world"));
    }

    // from a string buffer, with length specified
    {
        const char *s = "hello world";
        const ix_StringView hello(s, 5);
        ix_EXPECT(hello.length() == 5);
        ix_EXPECT(hello == ix_StringView("hello"));
        const ix_StringView world(s + 6, 5);
        ix_EXPECT(world.length() == 5);
        ix_EXPECT(world == ix_StringView("world"));
    }
}

ix_TEST_CASE("ix_StringView::operators")
{
    const ix_StringView empty("");
    const ix_StringView hello("hello");
    const ix_StringView world("world");

    // ix_StringView and ix_StringView
    {
        ix_EXPECT(empty == empty);
        ix_EXPECT(hello == hello);
        ix_EXPECT(empty != hello);
        ix_EXPECT(hello != empty);
        ix_EXPECT(hello != world);
    }

    // ix_StringView and const char *
    {
        ix_EXPECT(empty == "");
        ix_EXPECT(empty != "hello");
        ix_EXPECT("" == empty);
        ix_EXPECT("hello" != empty);

        ix_EXPECT(hello != "");
        ix_EXPECT(hello == "hello");
        ix_EXPECT("" != hello);
        ix_EXPECT("hello" == hello);
        ix_EXPECT("hell" != hello);
        ix_EXPECT("hellloo" != hello);
        ix_EXPECT(hello != "hell");
        ix_EXPECT(hello != "hellloo");
    }
}

ix_TEST_CASE("ix_StringView: hash")
{
    const ix_StringView empty("");
    const ix_StringView hello("hello");
    ix_EXPECT(ix_Hash<ix_StringView>::hash(empty) == ix_Hash<ix_StringView>::hash(empty));
    ix_EXPECT(ix_Hash<ix_StringView>::hash(empty) == ix_hash_str(""));
    ix_EXPECT(ix_Hash<ix_StringView>::hash(hello) == ix_Hash<ix_StringView>::hash(hello));
    ix_EXPECT(ix_Hash<ix_StringView>::hash(hello) == ix_hash_str("hello"));
}

ix_TEST_CASE("ix_StringView: null")
{
    const ix_StringView null(nullptr);
    ix_EXPECT_EQ(null.data(), nullptr);
    ix_EXPECT_EQ(null.length(), 0);
}
#endif
