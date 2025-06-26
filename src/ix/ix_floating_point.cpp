#include "ix_floating_point.hpp"
#include "ix_limits.hpp"
#include "ix_math.hpp"
#include "ix_min_max.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#endif

template <typename T>
bool ix_is_approximately_equal(T x, T y)
{
    constexpr T scale = T{100.0};
    constexpr T epsilon = ix_numeric_limits<T>::epsilon();
    const T diff = ix_abs(x - y);
    const T threashold = epsilon * (scale + ix_max(ix_abs(x), ix_abs(y)));
    return diff < threashold;
}

template bool ix_is_approximately_equal(float x, float y);
template bool ix_is_approximately_equal(double x, double y);

#if ix_DO_TEST
ix_TEST_CASE("ix_is_approximately_equal")
{
    ix_EXPECT(ix_is_approximately_equal(0.3F, 0.3F));

    ix_EXPECT(!ix_is_approximately_equal(0.3F, 0.33F));
    ix_EXPECT(!ix_is_approximately_equal(0.3F, 0.303F));
    ix_EXPECT(!ix_is_approximately_equal(0.3F, 0.3003F));
    ix_EXPECT(!ix_is_approximately_equal(0.3F, 0.30003F));
    ix_EXPECT(ix_is_approximately_equal(0.3F, 0.300003F));

    ix_EXPECT(!ix_is_approximately_equal(-0.3F, -0.33F));
    ix_EXPECT(!ix_is_approximately_equal(-0.3F, -0.303F));
    ix_EXPECT(!ix_is_approximately_equal(-0.3F, -0.3003F));
    ix_EXPECT(!ix_is_approximately_equal(-0.3F, -0.30003F));
    ix_EXPECT(ix_is_approximately_equal(-0.3F, -0.300003F));

    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.33));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.303));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.3003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.30003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.300003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.3000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.30000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.300000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.3000000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.30000000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.300000000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.3000000000003));
    ix_EXPECT(!ix_is_approximately_equal(0.3, 0.30000000000003));
    ix_EXPECT(ix_is_approximately_equal(0.3, 0.300000000000003));

    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.33));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.303));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.3003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.30003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.300003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.3000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.30000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.300000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.3000000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.30000000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.300000000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.3000000000003));
    ix_EXPECT(!ix_is_approximately_equal(-0.3, -0.30000000000003));
    ix_EXPECT(ix_is_approximately_equal(-0.3, -0.300000000000003));

    ix_EXPECT(!ix_is_approximately_equal(1.0e3F, 1.0e3F + 1.0F));
    ix_EXPECT(ix_is_approximately_equal(1.0e7F, 1.0e7F + 1.0F));
    ix_EXPECT(ix_is_approximately_equal(1.0F, 1.0F + (ix_numeric_limits<float>::epsilon() * 100.0F)));
    ix_EXPECT(!ix_is_approximately_equal(1.0F, 1.0F + (ix_numeric_limits<float>::epsilon() * 200.0F)));

    ix_EXPECT(!ix_is_approximately_equal(1.0e3, 1.0e3 + 1.0));
    ix_EXPECT(ix_is_approximately_equal(1.0e17, 1.0e17 + 1.0));
    ix_EXPECT(ix_is_approximately_equal(1.0, 1.0 + (ix_numeric_limits<double>::epsilon() * 100.0)));
    ix_EXPECT(!ix_is_approximately_equal(1.0, 1.0 + (ix_numeric_limits<double>::epsilon() * 200.0)));
}
#endif

template <typename T>
bool ix_is_approximately_equal_scaled(T x, T y, T scale)
{
    constexpr T epsilon = ix_numeric_limits<T>::epsilon();
    const T diff = ix_abs(x - y);
    const T threashold = epsilon * (scale + ix_max(ix_abs(x), ix_abs(y)));
    return diff < threashold;
}

template bool ix_is_approximately_equal_scaled(float x, float y, float scale);
template bool ix_is_approximately_equal_scaled(double x, double y, double scale);

#if ix_DO_TEST
ix_TEST_CASE("ix_is_approximately_equal")
{

    ix_EXPECT(ix_is_approximately_equal_scaled(0.3F, 0.3F, 10.0F));
    ix_EXPECT(!ix_is_approximately_equal_scaled(0.3F, 0.33F, 10.0F));
    ix_EXPECT(ix_is_approximately_equal_scaled(-0.3F, -0.3F, 10.0F));
    ix_EXPECT(!ix_is_approximately_equal_scaled(-0.3F, -0.33F, 10.0F));
    ix_EXPECT(!ix_is_approximately_equal_scaled(1.0e3F, 1.0e3F + 1.0F, 10.0F));
    ix_EXPECT(ix_is_approximately_equal_scaled(1.0e7F, 1.0e7F + 1.0F, 10.0F));
    ix_EXPECT(ix_is_approximately_equal_scaled(1.0F, 1.0F + ix_numeric_limits<float>::epsilon(), 0.0F));
    ix_EXPECT(!ix_is_approximately_equal_scaled(1.0F, 1.0F + (ix_numeric_limits<float>::epsilon() * 2.0F), 0.0F));

    ix_EXPECT(ix_is_approximately_equal_scaled(0.3, 0.3, 10.0));
    ix_EXPECT(!ix_is_approximately_equal_scaled(0.3, 0.33, 10.0));
    ix_EXPECT(ix_is_approximately_equal_scaled(-0.3, -0.3, 10.0));
    ix_EXPECT(!ix_is_approximately_equal_scaled(-0.3, -0.33, 10.0));
    ix_EXPECT(!ix_is_approximately_equal_scaled(1.0e3, 1.0e3 + 1.0, 10.0));
    ix_EXPECT(ix_is_approximately_equal_scaled(1.0e17, 1.0e17 + 1.0, 10.0));
    ix_EXPECT(ix_is_approximately_equal_scaled(1.0, 1.0 + ix_numeric_limits<double>::epsilon(), 0.0));
    ix_EXPECT(!ix_is_approximately_equal_scaled(1.0, 1.0 + (ix_numeric_limits<double>::epsilon() * 2.0), 0.0));
}
#endif
