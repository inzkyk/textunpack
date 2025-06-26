#include "ix_scanf.hpp"
#include "ix_compare.hpp"

#if ix_DO_TEST
#include "ix_random.hpp"
#include "ix_test.hpp"
#endif

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

static bool ix_is_white(char c)
{
    return (c == ' ') || (c == '\n');
}

void ix_skip_to_next_word(const char **c)
{
    while ((**c != '\0') && !ix_is_white(**c))
    {
        *c += 1;
    }

    while ((**c != '\0') && ix_is_white(**c))
    {
        *c += 1;
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_skip_to_next_word")
{
    const char *text = "hello world!";
    const char *p = text;
    ix_skip_to_next_word(&p);
    ix_EXPECT(p == text + 6);
    ix_EXPECT_EQSTR(p, "world!");

    ix_skip_to_next_word(&p);
    ix_EXPECT(p == text + 12);
    ix_EXPECT_EQSTR(p, "");

    ix_skip_to_next_word(&p);
    ix_EXPECT(p == text + 12);
    ix_EXPECT_EQSTR(p, "");
}
#endif

static char buf_ix_read_string[256];

const char *ix_read_string(const char **c)
{
    size_t size_written = 0;
    const size_t buf_len = ix_LENGTH_OF(buf_ix_read_string);
    while ((**c != '\0') && !ix_is_white(**c) && (size_written < buf_len - 1))
    {
        buf_ix_read_string[size_written] = **c;
        size_written += 1;
        *c += 1;
    }

    ix_ASSERT(size_written < buf_len);
    buf_ix_read_string[size_written] = '\0';

    if (size_written == buf_len - 1 && **c != '\0')
    {
        return nullptr;
    }

    while (ix_is_white(**c))
    {
        *c += 1;
    }

    return buf_ix_read_string;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_read_string")
{
    // one word
    {
        const char *text = "hello";
        const char *p = ix_read_string(&text);
        ix_EXPECT(p != nullptr);
        ix_EXPECT_EQSTR(p, "hello");
    }

    // two words
    {
        const char *text = "hello world";
        const char *p = ix_read_string(&text);
        ix_EXPECT(p != nullptr);
        ix_EXPECT_EQSTR(p, "hello");
    }

    // length 255
    {
        char buf[255 + 1];
        ix_rand_fill_alphanumeric(buf, ix_LENGTH_OF(buf) - 1);
        const char *text = buf;
        const char *p = ix_read_string(&text);
        ix_EXPECT(p != nullptr);
    }

    // length 256
    {
        char buf[256 + 1];
        ix_rand_fill_alphanumeric(buf, ix_LENGTH_OF(buf) - 1);
        const char *text = buf;
        const char *p = ix_read_string(&text);
        ix_EXPECT(p == nullptr);
    }
}
#endif

float ix_read_float(const char **c)
{
    const float f = strtof(*c, nullptr);
    ix_skip_to_next_word(c);
    return f;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_read_float")
{
    // one float
    {
        const char *text = "12.3";
        const float f = ix_read_float(&text);
        ix_EXPECT_EQF(f, 12.3F);
        ix_EXPECT(*text == '\0');
    }

    // three floats
    {
        const char *text = "12.3 45.6 78.9";
        float f;
        f = ix_read_float(&text);
        ix_EXPECT_EQF(f, 12.3F);
        f = ix_read_float(&text);
        ix_EXPECT_EQF(f, 45.6F);
        f = ix_read_float(&text);
        ix_EXPECT_EQF(f, 78.9F);
        ix_EXPECT(*text == '\0');
    }
}
#endif

int ix_read_int(const char **c)
{
    const int i = static_cast<int>(strtol(*c, nullptr, 10));
    ix_skip_to_next_word(c);
    return i;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_read_int")
{
    // one int
    {
        const char *text = "123";
        const int i = ix_read_int(&text);
        ix_EXPECT(i == 123);
        ix_EXPECT(*text == '\0');
    }

    // three ints
    {
        const char *text = "123 -456 789";
        int i;
        i = ix_read_int(&text);
        ix_EXPECT(i == 123);
        i = ix_read_int(&text);
        ix_EXPECT(i == -456);
        i = ix_read_int(&text);
        ix_EXPECT(i == 789);
        ix_EXPECT(*text == '\0');
    }
}
#endif

unsigned int ix_read_uint(const char **c)
{
    const unsigned int i = static_cast<unsigned int>(strtoul(*c, nullptr, 10));
    ix_skip_to_next_word(c);
    return i;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_read_uint: three ints")
{
    const char *text = "123 456 789";
    unsigned int i;
    i = ix_read_uint(&text);
    ix_EXPECT(i == 123);
    i = ix_read_uint(&text);
    ix_EXPECT(i == 456);
    i = ix_read_uint(&text);
    ix_EXPECT(i == 789);
    ix_EXPECT(*text == '\0');
}
#endif

void ix_next_line(const char **c)
{
    if (**c == '\0')
    {
        return;
    }

    while (**c != '\n')
    {
        *c += 1;
    }

    *c += 1;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_next_line")
{
    const char *text = "hello\n"
                       "\n"
                       "world\n"
                       "\n";
    ix_next_line(&text);
    ix_EXPECT_EQSTR(text, "\nworld\n\n");
    ix_next_line(&text);
    ix_EXPECT_EQSTR(text, "world\n\n");
    ix_next_line(&text);
    ix_EXPECT_EQSTR(text, "\n");
    ix_next_line(&text);
    ix_EXPECT_EQSTR(text, "");
    ix_next_line(&text);
    ix_EXPECT_EQSTR(text, "");
}
#endif

void ix_skip_empty_lines(const char **c)
{
    while (**c == '\n')
    {
        ix_next_line(c);
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_skip_empty_lines")
{
    const char *text = "hello\n"
                       "\n"
                       "world\n"
                       "\n";
    ix_skip_empty_lines(&text);
    ix_EXPECT_EQSTR(text, "hello\n\nworld\n\n");
    ix_next_line(&text);
    ix_skip_empty_lines(&text);
    ix_EXPECT_EQSTR(text, "world\n\n");
    ix_next_line(&text);
    ix_skip_empty_lines(&text);
    ix_EXPECT_EQSTR(text, "");
}
#endif

void ix_next_non_empty_line(const char **c)
{
    ix_next_line(c);
    while (**c == '\n')
    {
        ix_next_line(c);
    }
}

#if ix_DO_TEST
ix_TEST_CASE("ix_next_non_empty_line")
{
    const char *text = "hello\n"
                       "\n"
                       "world\n"
                       "\n";
    ix_next_non_empty_line(&text);
    ix_EXPECT_EQSTR(text, "world\n\n");
    ix_next_non_empty_line(&text);
    ix_EXPECT_EQSTR(text, "");
    ix_next_non_empty_line(&text);
    ix_EXPECT_EQSTR(text, "");
}
#endif

template <typename T>
ix_Option<T> ix_string_convert(const char *s)
{
    const bool starts_with_space = (*s == ' ');
    if (starts_with_space)
    {
        return ix_ERROR_INVALID_PARAMETER;
    }

    if constexpr (ix_is_same_v<T, float>)
    {
        char *end;
        errno = 0;
        const float v = strtof(s, &end);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(
                ix_equal(v, HUGE_VALF) || ix_equal(v, -HUGE_VALF) || //
                ix_equal(v, FLT_MIN) || ix_equal(v, -FLT_MIN) || ix_equal(v, 0.0F)
            );
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, double>)
    {
        char *end;
        errno = 0;
        const double v = strtod(s, &end);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(
                ix_equal(v, HUGE_VAL) || ix_equal(v, -HUGE_VAL) || //
                ix_equal(v, DBL_MIN) || ix_equal(v, -DBL_MIN) || ix_equal(v, 0.0)
            );
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, long>) // cppcheck-suppress multiCondition
    {
        char *end;
        errno = 0;
        const long v = strtol(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT((v == LONG_MIN) || (v == LONG_MAX));
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, long long>) // cppcheck-suppress multiCondition
    {
        char *end;
        errno = 0;
        const long long v = strtoll(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT((v == LLONG_MIN) || (v == LLONG_MAX));
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, unsigned long>) // cppcheck-suppress multiCondition
    {
        const bool has_minus_sign = (*s == '-');
        if (has_minus_sign)
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        char *end;
        errno = 0;
        const unsigned long v = strtoul(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(v == ULONG_MAX);
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    else if constexpr (ix_is_same_v<T, unsigned long long>) // cppcheck-suppress multiCondition
    {
        const bool has_minus_sign = (*s == '-');
        if (has_minus_sign)
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        char *end;
        errno = 0;
        const unsigned long long v = strtoull(s, &end, 10);
        if ((s == end) || (*end != '\0'))
        {
            return ix_ERROR_INVALID_PARAMETER;
        }

        if (errno == ERANGE)
        {
            ix_ASSERT(v == ULLONG_MAX);
            return ix_ERROR_INVALID_PARAMETER;
        }

        return v;
    }
    ix_UNREACHABLE();
}

template ix_Option<float> ix_string_convert<float>(const char *s);
template ix_Option<double> ix_string_convert<double>(const char *s);
template ix_Option<long> ix_string_convert<long>(const char *s);
template ix_Option<long long> ix_string_convert<long long>(const char *s);
template ix_Option<unsigned long> ix_string_convert<unsigned long>(const char *s);
template ix_Option<unsigned long long> ix_string_convert<unsigned long long>(const char *s);

#if ix_DO_TEST
ix_TEST_CASE("ix_string_convert<float>")
{
    ix_EXPECT(ix_string_convert<float>("123.456").is_ok());
    ix_EXPECT_EQF(ix_string_convert<float>("123.456").unwrap(), 123.456F);
    ix_EXPECT(ix_string_convert<float>("-123.456").is_ok());
    ix_EXPECT_EQF(ix_string_convert<float>("-123.456").unwrap(), -123.456F);

    ix_EXPECT(ix_string_convert<float>("").is_error());
    ix_EXPECT(ix_string_convert<float>(" 100").is_error());
    ix_EXPECT(ix_string_convert<float>("10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<float>("-10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<float>("0.00000000000000000000000000000000000000000000001").is_error());
    ix_EXPECT(ix_string_convert<float>("-0.00000000000000000000000000000000000000000000001").is_error());
    ix_EXPECT(ix_string_convert<float>("-100x").is_error());
    ix_EXPECT(ix_string_convert<float>("hello world").is_error());

    ix_EXPECT(ix_string_convert<float>("3.40282347e+38").is_ok());
    ix_EXPECT_EQ(ix_string_convert<float>("3.40282347e+38").unwrap(), FLT_MAX);
    ix_EXPECT(ix_string_convert<float>("3.40282347e+39").is_error());

    ix_EXPECT(ix_string_convert<float>("-3.40282347e+38").is_ok());
    ix_EXPECT_EQ(ix_string_convert<float>("-3.40282347e+38").unwrap(), -FLT_MAX);
    ix_EXPECT(ix_string_convert<float>("-3.40282347e+39").is_error());

    ix_EXPECT(ix_string_convert<float>("1.17549435e-38").is_ok());
    ix_EXPECT_EQ(ix_string_convert<float>("1.17549435e-38").unwrap(), FLT_MIN);
    ix_EXPECT(ix_string_convert<float>("1.17549435e-100").is_error());

    ix_EXPECT(ix_string_convert<float>("-1.17549435e-38").is_ok());
    ix_EXPECT_EQ(ix_string_convert<float>("-1.17549435e-38").unwrap(), -FLT_MIN);
    ix_EXPECT(ix_string_convert<float>("-1.17549435e-100").is_error());
}

ix_TEST_CASE("ix_string_convert<double>")
{
    ix_EXPECT(ix_string_convert<double>("123.456").is_ok());
    ix_EXPECT_EQD(ix_string_convert<double>("123.456").unwrap(), 123.456);
    ix_EXPECT(ix_string_convert<double>("-123.456").is_ok());
    ix_EXPECT_EQD(ix_string_convert<double>("-123.456").unwrap(), -123.456);

    ix_EXPECT(ix_string_convert<double>("").is_error());
    ix_EXPECT(ix_string_convert<double>(" 100").is_error());
    ix_EXPECT(ix_string_convert<double>("1.0e10000").is_error());
    ix_EXPECT(ix_string_convert<double>("-1.0e10000").is_error());
    ix_EXPECT(ix_string_convert<double>("1.0e-10000").is_error());
    ix_EXPECT(ix_string_convert<double>("-1.0e-10000").is_error());
    ix_EXPECT(ix_string_convert<double>("-100x").is_error());
    ix_EXPECT(ix_string_convert<double>("hello world").is_error());

    ix_EXPECT(ix_string_convert<double>("1.7976931348623157e+308").is_ok());
    ix_EXPECT_EQ(ix_string_convert<double>("1.7976931348623157e+308").unwrap(), DBL_MAX);
    ix_EXPECT(ix_string_convert<double>("1.7976931348623157e+309").is_error());

    ix_EXPECT(ix_string_convert<double>("-1.7976931348623157e+308").is_ok());
    ix_EXPECT_EQ(ix_string_convert<double>("-1.7976931348623157e+308").unwrap(), -DBL_MAX);
    ix_EXPECT(ix_string_convert<double>("1.7976931348623157e+310").is_error());

    ix_EXPECT(ix_string_convert<double>("2.2250738585072014e-308").is_ok());
    ix_EXPECT_EQ(ix_string_convert<double>("2.2250738585072014e-308").unwrap(), DBL_MIN);
    ix_EXPECT(ix_string_convert<double>("2.2250738585072014e-400").is_error());

    ix_EXPECT(ix_string_convert<double>("-2.2250738585072014e-308").is_ok());
    ix_EXPECT_EQ(ix_string_convert<double>("-2.2250738585072014e-308").unwrap(), -DBL_MIN);
    ix_EXPECT(ix_string_convert<double>("-2.2250738585072014e-400").is_error());
}

ix_TEST_CASE("ix_string_convert<long>")
{
    ix_EXPECT(ix_string_convert<long>("100").is_ok());
    ix_EXPECT(ix_string_convert<long>("100").unwrap() == 100);
    ix_EXPECT(ix_string_convert<long>("-100").is_ok());
    ix_EXPECT(ix_string_convert<long>("-100").unwrap() == -100);

    ix_EXPECT(ix_string_convert<long>("").is_error());
    ix_EXPECT(ix_string_convert<long>(" 100").is_error());
    // cf. https://developercommunity.visualstudio.com/t/Cannot-detect-strtol-range-errors-with-A/10412869
    ix_EXPECT(ix_string_convert<long>("10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<long>("-10000000000000000000000000000000000000000000000").is_error());

    ix_EXPECT(ix_string_convert<long>("-100x").is_error());
    ix_EXPECT(ix_string_convert<long>("hello world").is_error());

    if constexpr (sizeof(long) == 4)
    {
        ix_EXPECT(ix_string_convert<long>("-2147483648").is_ok());
        ix_EXPECT(ix_string_convert<long>("-2147483648").unwrap() == LONG_MIN);

        ix_EXPECT(ix_string_convert<long>("2147483647").is_ok());
        ix_EXPECT(ix_string_convert<long>("2147483647").unwrap() == LONG_MAX);
        // cf. https://developercommunity.visualstudio.com/t/Cannot-detect-strtol-range-errors-with-A/10412869
        ix_EXPECT(ix_string_convert<long>("-2147483649").is_error());
        ix_EXPECT(ix_string_convert<long>("2147483648").is_error());
    }
    else
    {
        ix_EXPECT(ix_string_convert<long>("-9223372036854775808").is_ok());
        ix_EXPECT(ix_string_convert<long>("-9223372036854775808").unwrap() == LONG_MIN);
        ix_EXPECT(ix_string_convert<long>("-9223372036854775809").is_error());

        ix_EXPECT(ix_string_convert<long>("9223372036854775807").is_ok());
        ix_EXPECT(ix_string_convert<long>("9223372036854775807").unwrap() == LONG_MAX);
        ix_EXPECT(ix_string_convert<long>("9223372036854775808").is_error());
    }
}

ix_TEST_CASE("ix_string_convert<long long>")
{
    ix_EXPECT(ix_string_convert<long long>("100").is_ok());
    ix_EXPECT(ix_string_convert<long long>("100").unwrap() == 100);
    ix_EXPECT(ix_string_convert<long long>("-100").is_ok());
    ix_EXPECT(ix_string_convert<long long>("-100").unwrap() == -100);

    ix_EXPECT(ix_string_convert<long long>("").is_error());
    ix_EXPECT(ix_string_convert<long long>(" 100").is_error());
    ix_EXPECT(ix_string_convert<long long>("10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<long long>("-10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<long long>("-100x").is_error());
    ix_EXPECT(ix_string_convert<long long>("hello world").is_error());

    if constexpr (sizeof(long long) == 8)
    {
        ix_EXPECT(ix_string_convert<long long>("-9223372036854775808").is_ok());
        ix_EXPECT(ix_string_convert<long long>("-9223372036854775808").unwrap() == LLONG_MIN);
        ix_EXPECT(ix_string_convert<long long>("-9223372036854775809").is_error());

        ix_EXPECT(ix_string_convert<long long>("9223372036854775807").is_ok());
        ix_EXPECT(ix_string_convert<long long>("9223372036854775807").unwrap() == LLONG_MAX);
        ix_EXPECT(ix_string_convert<long long>("9223372036854775808").is_error());
    }
    else
    {
        ix_EXPECT(false);
    }
}

ix_TEST_CASE("ix_string_convert<unsigned long>")
{
    ix_EXPECT(ix_string_convert<unsigned long>("100").is_ok());
    ix_EXPECT(ix_string_convert<unsigned long>("100").unwrap() == 100);

    ix_EXPECT(ix_string_convert<unsigned long>("").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>(" 100").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>("-100").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>("10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>("-10000000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>("100x").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>("-100x").is_error());
    ix_EXPECT(ix_string_convert<unsigned long>("hello world").is_error());

    if constexpr (sizeof(unsigned long) == 4)
    {
        ix_EXPECT(ix_string_convert<unsigned long>("4294967295").is_ok());
        ix_EXPECT(ix_string_convert<unsigned long>("4294967295").unwrap() == ULONG_MAX);
        ix_EXPECT(ix_string_convert<unsigned long>("4294967296").is_error());
    }
    else
    {
        ix_EXPECT(ix_string_convert<unsigned long>("18446744073709551615").is_ok());
        ix_EXPECT(ix_string_convert<unsigned long>("18446744073709551615").unwrap() == ULONG_MAX);
        ix_EXPECT(ix_string_convert<unsigned long>("18446744073709551616").is_error());
    }
}

ix_TEST_CASE("ix_string_convert<unsigned long long>")
{
    ix_EXPECT(ix_string_convert<unsigned long long>("100").is_ok());
    ix_EXPECT(ix_string_convert<unsigned long long>("100").unwrap() == 100);

    ix_EXPECT(ix_string_convert<unsigned long long>("").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>(" 100").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>("-100").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>("10000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>("-10000000000000000000000000000000000000000000").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>("100x").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>("-100x").is_error());
    ix_EXPECT(ix_string_convert<unsigned long long>("hello world").is_error());

    if constexpr (sizeof(unsigned long long) == 8)
    {
        ix_EXPECT(ix_string_convert<unsigned long long>("18446744073709551615").is_ok());
        ix_EXPECT(ix_string_convert<unsigned long long>("18446744073709551615").unwrap() == ULLONG_MAX);
        ix_EXPECT(ix_string_convert<unsigned long long>("18446744073709551616").is_error());
    }
    else
    {
        ix_EXPECT(false);
    }
}
#endif
