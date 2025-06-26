#pragma once

#include "ix.hpp"

#define ix_PI 3.14159265358979323846f
#define ix_2PI 6.28318530717958647693f

float ix_to_degree(float x);
float ix_to_radian(float x);
float ix_normalize_radian(float x);
float ix_normalize_degree(float x);

template <typename T>
constexpr T ix_log10(T n)
{
    T ans = 0;
    while (n > 9)
    {
        n /= 10;
        ans += 1;
    }
    return ans;
}

template <typename T>
ix_FORCE_INLINE T ix_abs(T x)
{
    return (x > T{0}) ? x : -x;
}

// cf. https://zenn.dev/mod_poppo/articles/integer-division
template <typename T>
ix_FORCE_INLINE constexpr T ix_quot(T n, T d)
{
    return n / d;
}

template <typename T>
ix_FORCE_INLINE constexpr T ix_rem(T n, T d)
{
    return n % d;
}

template <typename T>
constexpr T ix_div(T n, T d)
{
    T quot = n / d;
    const bool diffrent_sign = ((n ^ d) < 0);
    const bool divisible = ((n % d) == 0);
    const bool needs_correction = diffrent_sign && !divisible;
    return needs_correction ? (quot - 1) : quot;
}

template <typename T>
constexpr T ix_mod(T n, T d)
{
    T rem = n % d;
    const bool diffrent_sign = ((n ^ d) < 0);
    const bool divisible = (rem == 0);
    const bool needs_correction = diffrent_sign && !divisible;
    return needs_correction ? (rem + d) : rem;
}
