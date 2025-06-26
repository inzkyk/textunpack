#pragma once

#include "ix.hpp"
#include "ix_polyfill.hpp"

bool ix_contains(const char *s, char c);
bool ix_starts_with(const char *a, const char *b);
bool ix_wildcard_match(const char *string, const char *wildcard);
bool ix_wstarts_with(const wchar_t *a, const wchar_t *b);
int ix_wstrcmp(const wchar_t *a, const wchar_t *b);
size_t ix_wstrlen(const wchar_t *s);

char *ix_strstr(char *haystack, const char *needle);
const char *ix_strstr(const char *haystack, const char *needle);

const char *ix_strchr(const char *haystack, char c);

void *ix_memchr(char *haystack, char c, size_t length);
const void *ix_memchr(const char *haystack, char c, size_t length);

char *ix_memnext(char *haystack, char c);
const char *ix_memnext(const char *haystack, char c);

char *ix_memnext2(char *haystack, char c0, char c1);
const char *ix_memnext2(const char *haystack, char c0, char c1);

size_t ix_strlen_runtime(const char *s);
int ix_strcmp_runtime(const char *a, const char *b);

ix_FORCE_INLINE constexpr size_t ix_strlen(const char *s)
{
#if !ix_MEASURE_COVERAGE
    if (!ix_is_constant_evaluated())
    {
        return ix_strlen_runtime(s);
    }
#endif

    const char *start = s;
    while (*s != '\0')
    {
        s += 1;
    }
    return static_cast<size_t>(s - start);
}

ix_FORCE_INLINE constexpr int ix_strcmp(const char *a, const char *b)
{
#if !ix_MEASURE_COVERAGE
    if (!ix_is_constant_evaluated())
    {
        return ix_strcmp_runtime(a, b);
    }
#endif

    while (*a != '\0')
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
