#include "ix_string.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#endif

#include <stdlib.h>
#include <string.h>

#if ix_DO_TEST
ix_TEST_CASE("ix_strlen")
{
    ix_EXPECT(ix_strlen("") == 0);
    ix_EXPECT(ix_strlen("foo") == 3);
    ix_EXPECT(ix_strlen("foo bar") == 7);
    ix_EXPECT(ix_strlen("foo\0bar") == 3);

    ix_EXPECT(ix_strlen_runtime("") == 0);
    ix_EXPECT(ix_strlen_runtime("foo") == 3);
    ix_EXPECT(ix_strlen_runtime("foo bar") == 7);
    ix_EXPECT(ix_strlen_runtime("foo\0bar") == 3);
}

ix_TEST_CASE("ix_strcmp")
{
    ix_EXPECT(ix_strcmp("", "") == 0);
    ix_EXPECT(ix_strcmp("", "hello") < 0);
    ix_EXPECT(ix_strcmp("hello", "") > 0);
    ix_EXPECT(ix_strcmp("hello", "hello") == 0);

    ix_EXPECT(ix_strcmp_runtime("", "") == 0);
    ix_EXPECT(ix_strcmp_runtime("", "hello") < 0);
    ix_EXPECT(ix_strcmp_runtime("hello", "") > 0);
    ix_EXPECT(ix_strcmp_runtime("hello", "hello") == 0);
}
#endif

bool ix_starts_with(const char *a, const char *b)
{
    if (ix_UNLIKELY(*b == '\0'))
    {
        return true;
    }

    while (*a == *b)
    {
        a += 1;
        b += 1;
        if (*b == '\0')
        {
            return true;
        }
    }

    return false;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_starts_with")
{
    ix_EXPECT(ix_starts_with("", ""));
    ix_EXPECT(ix_starts_with("foo", ""));
    ix_EXPECT(ix_starts_with("foo", "foo"));
    ix_EXPECT(ix_starts_with("foobar", "foo"));
    ix_EXPECT(ix_starts_with("こんにちは", "こん"));
    ix_EXPECT(!ix_starts_with("", "foo"));
    ix_EXPECT(!ix_starts_with("bar", "foo"));
    ix_EXPECT(!ix_starts_with("foa", "foo"));
    ix_EXPECT(!ix_starts_with("こんにちは", "もち"));
}
#endif

bool ix_wstarts_with(const wchar_t *a, const wchar_t *b)
{
    if (ix_UNLIKELY(*b == L'\0'))
    {
        return true;
    }

    while (*a == *b)
    {
        a += 1;
        b += 1;
        if (*b == L'\0')
        {
            return true;
        }
    }

    return false;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_starts_with")
{
    ix_EXPECT(ix_wstarts_with(L"", L""));
    ix_EXPECT(ix_wstarts_with(L"foo", L""));
    ix_EXPECT(ix_wstarts_with(L"foo", L"foo"));
    ix_EXPECT(ix_wstarts_with(L"foobar", L"foo"));
    ix_EXPECT(ix_wstarts_with(L"こんにちは", L"こん"));
    ix_EXPECT(!ix_wstarts_with(L"", L"foo"));
    ix_EXPECT(!ix_wstarts_with(L"bar", L"foo"));
    ix_EXPECT(!ix_wstarts_with(L"foa", L"foo"));
    ix_EXPECT(!ix_wstarts_with(L"こんにちは", L"もち"));
}
#endif

bool ix_contains(const char *s, char c)
{
    return (strchr(s, c) != nullptr);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_contains")
{
    ix_EXPECT(ix_contains("foo", 'f'));
    ix_EXPECT(ix_contains("foo", 'o'));
    ix_EXPECT(ix_contains("foo", '\0'));
    ix_EXPECT(!ix_contains("foo", 'b'));
    ix_EXPECT(!ix_contains("foo", 'a'));
    ix_EXPECT(!ix_contains("", 'f'));
    ix_EXPECT(!ix_contains("", 'o'));
}
#endif

char *ix_strstr(char *haystack, const char *needle)
{
    return strstr(haystack, needle);
}

const char *ix_strstr(const char *haystack, const char *needle)
{
    return strstr(haystack, needle);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_strstr")
{
    ix_EXPECT_EQSTR(ix_strstr("", ""), "");
    ix_EXPECT_EQSTR(ix_strstr("foo", ""), "foo");
    ix_EXPECT_EQSTR(ix_strstr("foofoo", "foo"), "foofoo");
    ix_EXPECT_EQSTR(ix_strstr("foobar", "bar"), "bar");

    ix_EXPECT(ix_strstr("", "foo") == nullptr);
    ix_EXPECT(ix_strstr("foo", "bar") == nullptr);

    char buf[64];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\0';
    ix_EXPECT(ix_strstr(buf, "bc") == buf + 1);
}
#endif

const char *ix_strchr(const char *haystack, char c)
{
    return strchr(haystack, c);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_strchr")
{
    ix_EXPECT(ix_strchr("", 'X') == nullptr);
    const char *msg = "FooBar";
    ix_EXPECT(ix_strchr(msg, 'F') == msg);
    ix_EXPECT(ix_strchr(msg, 'o') == msg + 1);
    ix_EXPECT(ix_strchr(msg, 'B') == msg + 3);
    ix_EXPECT(ix_strchr(msg, '\0') == msg + ix_strlen(msg));

    char buf[64];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\0';
    ix_EXPECT(ix_strchr(buf, 'b') == buf + 1);
}
#endif

const void *ix_memchr(const char *haystack, char c, size_t length)
{
    return memchr(haystack, c, length);
}

void *ix_memchr(char *haystack, char c, size_t length)
{
    return memchr(haystack, c, length);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_memchr")
{
    ix_EXPECT(ix_memchr("", 'X', 0) == nullptr);
    const char *msg = "FooBar";
    ix_EXPECT(ix_memchr(msg, 'F', 0) == nullptr);
    ix_EXPECT(ix_memchr(msg, 'F', 1) == msg);
    ix_EXPECT(ix_memchr(msg, 'F', 3) == msg);
    ix_EXPECT(ix_memchr(msg, 'o', 6) == msg + 1);
    ix_EXPECT(ix_memchr(msg, 'B', 6) == msg + 3);
    ix_EXPECT(ix_memchr(msg, '\0', 7) == msg + ix_strlen(msg));

    char buf[64];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\0';
    ix_EXPECT(ix_memchr(buf, 'b', 2) == buf + 1);
}
#endif

char *ix_memnext(char *haystack, char c)
{
    while (*haystack != c)
    {
        haystack += 1;
    }
    return haystack;
}

const char *ix_memnext(const char *haystack, char c)
{
    while (*haystack != c)
    {
        haystack += 1;
    }
    return haystack;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_memnext")
{
    const char *msg = "FooBar";
    ix_EXPECT(ix_memnext(msg, 'F') == msg);
    ix_EXPECT(ix_memnext(msg, 'o') == msg + 1);
    ix_EXPECT(ix_memnext(msg, 'B') == msg + 3);
    ix_EXPECT(ix_memnext(msg, '\0') == msg + ix_strlen(msg));

    char buf[64];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\0';
    ix_EXPECT(ix_memnext(buf, 'b') == buf + 1);
}

#endif

char *ix_memnext2(char *haystack, char c0, char c1)
{
    while ((*haystack != c0) && (*haystack != c1))
    {
        haystack += 1;
    }
    return haystack;
}

const char *ix_memnext2(const char *haystack, char c0, char c1)
{
    while ((*haystack != c0) && (*haystack != c1))
    {
        haystack += 1;
    }
    return haystack;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_memnext2")
{
    const char *msg = "FooBar";
    ix_EXPECT(ix_memnext2(msg, 'F', 'B') == msg);
    ix_EXPECT(ix_memnext2(msg, 'B', 'F') == msg);
    ix_EXPECT(ix_memnext2(msg, 'B', 'a') == msg + 3);
    ix_EXPECT(ix_memnext2(msg, 'a', 'B') == msg + 3);
    ix_EXPECT(ix_memnext2(msg, '\0', 'a') == msg + 4);
    ix_EXPECT(ix_memnext2(msg, 'a', '\0') == msg + 4);
    ix_EXPECT(ix_memnext2(msg, '\0', '\0') == msg + ix_strlen(msg));

    char buf[64];
    buf[0] = 'a';
    buf[1] = 'b';
    buf[2] = 'c';
    buf[3] = '\0';
    ix_EXPECT(ix_memnext2(buf, 'b', 'c') == buf + 1);
    ix_EXPECT(ix_memnext2(buf, 'c', 'b') == buf + 1);
}
#endif

size_t ix_strlen_runtime(const char *s)
{
    return strlen(s);
}

int ix_strcmp_runtime(const char *a, const char *b)
{
    return strcmp(a, b);
}

size_t ix_wstrlen(const wchar_t *s)
{
    const wchar_t *start = s;
    while (*s != '\0')
    {
        s += 1;
    }
    return static_cast<size_t>(s - start);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_wstrlen")
{
    ix_EXPECT_EQ(ix_wstrlen(L""), 0);
    ix_EXPECT_EQ(ix_wstrlen(L"FOO"), 3);
    ix_EXPECT_EQ(ix_wstrlen(L"あ"), 1);
    if constexpr (sizeof(wchar_t) == 2)
    {
        // UTF-16 (Windows)
        ix_EXPECT_EQ(ix_wstrlen(L"𩹉𠮷😟"), 6);
    }
    else
    {
        // UTF-32 (others)
        ix_EXPECT_EQ(ix_wstrlen(L"𩹉𠮷😟"), 3);
    }
}
#endif

int ix_wstrcmp(const wchar_t *a, const wchar_t *b)
{
    while (*a != L'\0')
    {
        if (*a != *b)
        {
            break;
        }

        a += 1;
        b += 1;
    }

    return (*a - *b);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_wstrcmp")
{
    ix_EXPECT_EQ(ix_wstrcmp(L"", L""), 0);
    ix_EXPECT_EQ(ix_wstrcmp(L"FOO", L"FOO"), 0);
    ix_EXPECT_EQ(ix_wstrcmp(L"あ", L"あ"), 0);
    ix_EXPECT_EQ(ix_wstrcmp(L"𩹉𠮷😟", L"𩹉𠮷😟"), 0);

    ix_EXPECT(ix_wstrcmp(L"", L"FOO") != 0);
    ix_EXPECT(ix_wstrcmp(L"FOO", L"") != 0);
    ix_EXPECT(ix_wstrcmp(L"", L"FOO") != 0);
    ix_EXPECT(ix_wstrcmp(L"あ", L"あHOO") != 0);
    ix_EXPECT(ix_wstrcmp(L"あHOO", L"あ") != 0);
    ix_EXPECT(ix_wstrcmp(L"あ", L"い") != 0);
    ix_EXPECT(ix_wstrcmp(L"𩹉𠮷😟", L"あ") != 0);
    ix_EXPECT(ix_wstrcmp(L"あ", L"𩹉𠮷😟") != 0);
    ix_EXPECT(ix_wstrcmp(L"𩹉", L"𠮷") != 0);
    ix_EXPECT(ix_wstrcmp(L"𠮷", L"𩹉") != 0);
}
#endif

bool ix_wildcard_match(const char *string, const char *wildcard)
{
    char w = *wildcard;
    char s = *string;

    while ((s != '\0') && (w != '*'))
    {
        if ((s != w) && (w != '?'))
        {
            return false;
        }
        wildcard += 1;
        string += 1;
        w = *wildcard;
        s = *string;
    }

    const char *backtrack_wildcard = nullptr;
    const char *backtrack_string = nullptr;
    while (s != '\0')
    {
        if (w == '*')
        {
            wildcard += 1;
            w = *wildcard;
            if (w == '\0')
            {
                return true;
            }
            backtrack_wildcard = wildcard;
            backtrack_string = string + 1;
        }
        else if ((w == s) || (w == '?'))
        {
            wildcard += 1;
            string += 1;
            w = *wildcard;
            s = *string;
        }
        else
        {
            wildcard = backtrack_wildcard;
            string = backtrack_string;
            w = *wildcard;
            s = *string;
            backtrack_string += 1;
        }
    }

    while (w == '*')
    {
        wildcard += 1;
        w = *wildcard;
    }

    return (w == '\0');
}

#if ix_DO_TEST
ix_TEST_CASE("ix_wildcard_match")
{
    ix_EXPECT(ix_wildcard_match("", ""));
    ix_EXPECT(ix_wildcard_match("foo", "foo"));
    ix_EXPECT(!ix_wildcard_match("foo", ""));
    ix_EXPECT(!ix_wildcard_match("", "foo"));
    ix_EXPECT(!ix_wildcard_match("foooo", "foo"));
    ix_EXPECT(!ix_wildcard_match("foo", "fooooo"));

    ix_EXPECT(ix_wildcard_match("f", "?"));
    ix_EXPECT(ix_wildcard_match("fo", "??"));
    ix_EXPECT(ix_wildcard_match("foo", "???"));
    ix_EXPECT(ix_wildcard_match("foo", "f??"));
    ix_EXPECT(ix_wildcard_match("foo", "f?o"));
    ix_EXPECT(!ix_wildcard_match("fo", "?"));
    ix_EXPECT(!ix_wildcard_match("foo", "??"));
    ix_EXPECT(!ix_wildcard_match("fooo", "???"));
    ix_EXPECT(!ix_wildcard_match("fooo", "f??"));
    ix_EXPECT(!ix_wildcard_match("fooo", "f?o"));
    ix_EXPECT(!ix_wildcard_match("f", "f?"));
    ix_EXPECT(!ix_wildcard_match("fo", "f??"));
    ix_EXPECT(!ix_wildcard_match("foo", "f???"));
    ix_EXPECT(!ix_wildcard_match("foo", "fo??"));
    ix_EXPECT(!ix_wildcard_match("foo", "fo?o"));

    ix_EXPECT(ix_wildcard_match("a", "*?"));
    ix_EXPECT(ix_wildcard_match("a", "?*"));
    ix_EXPECT(ix_wildcard_match("ab", "*??"));
    ix_EXPECT(ix_wildcard_match("ab", "?*?"));
    ix_EXPECT(ix_wildcard_match("ab", "??*"));
    ix_EXPECT(ix_wildcard_match("abc", "*??"));
    ix_EXPECT(ix_wildcard_match("abc", "?*?"));
    ix_EXPECT(ix_wildcard_match("abc", "??*"));
    ix_EXPECT(ix_wildcard_match("abc", "*???"));
    ix_EXPECT(ix_wildcard_match("abc", "?*??"));
    ix_EXPECT(ix_wildcard_match("abc", "??*?"));
    ix_EXPECT(ix_wildcard_match("abc", "???*"));
    ix_EXPECT(ix_wildcard_match("abc", "abc*"));
    ix_EXPECT(ix_wildcard_match("aaaaabc", "*abc"));
    ix_EXPECT(ix_wildcard_match("abcccccd", "***cd"));
    ix_EXPECT(ix_wildcard_match("abc", "a*?c"));
    ix_EXPECT(ix_wildcard_match("abc", "a?*c"));
    ix_EXPECT(ix_wildcard_match("abcde", "*c*"));
    ix_EXPECT(ix_wildcard_match("abcde", "a*?c?*"));
    ix_EXPECT(ix_wildcard_match("abcde", "a?*c*?"));
    ix_EXPECT(ix_wildcard_match("abcde", "*b*c*"));
    ix_EXPECT(ix_wildcard_match("abcde", "*b*d*"));
    ix_EXPECT(ix_wildcard_match("abcdefghijklmnopqrstuvwxyz", "*a*d*x*z*"));
    ix_EXPECT(ix_wildcard_match("abcdefghijklmnopqrstuvwxyz", "*a*???*h*???*x*?*z*"));
    ix_EXPECT(ix_wildcard_match("xxxxxxxxxxxyyyyyyyyyyyyyzzzzzzzzzzzzz", "x*y*z"));
    ix_EXPECT(ix_wildcard_match("xxxxxxxxxxxyyyyyyyyyyyyyzzzzzzzzzzzzz", "xxx*yyy*zzzz"));
    ix_EXPECT(ix_wildcard_match("xxxxxxxxxxxyyyyyyyyyyyyyzzzzzzzzzzzzz", "x*xy*yz*z"));
    ix_EXPECT(ix_wildcard_match("xxxxxxxxxxxyyyyyyyyyyyyyzzzzzzzzzzzzz", "*xxxxxxxxxxxyyyyyyyyyyyyyzzzzzzzzzzzzz*"));
    ix_EXPECT(ix_wildcard_match(
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "*******a********a*******************a*****a*******************a************a*"
    ));

    ix_EXPECT(!ix_wildcard_match("", "?"));
    ix_EXPECT(!ix_wildcard_match("", "*?"));
    ix_EXPECT(!ix_wildcard_match("", "?*"));
    ix_EXPECT(!ix_wildcard_match("abc", "abc*d"));
    ix_EXPECT(!ix_wildcard_match("abc", "z*abc"));
    ix_EXPECT(!ix_wildcard_match("xxxxxxxxxxxyyyyyyyyyyyyyyyzzzzzzzzzzzzz", "xxx*yyy*zzza"));

    ix_EXPECT(ix_wildcard_match("*", "*"));
    ix_EXPECT(ix_wildcard_match("a*ba*ba*b", "a*b"));
    ix_EXPECT(ix_wildcard_match("a******b", "a*"));
    ix_EXPECT(ix_wildcard_match("a******b", "*b"));
    ix_EXPECT(ix_wildcard_match("a******aaab", "a*ab"));
}
#endif
