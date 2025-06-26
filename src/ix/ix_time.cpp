#include "ix_time.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#endif

#include <sokol_time.h>

ix_Result ix_time_init()
{
    stm_setup();
    return ix_OK;
}

uint64_t ix_time_now()
{
    return stm_now();
}

#if ix_DO_TEST
ix_TEST_CASE("ix_time_now")
{
    const uint64_t x = ix_time_now();
    ix_EXPECT(x > 0);
    const uint64_t y = ix_time_now();
    ix_EXPECT(x <= y);
}
#endif

double ix_time_diff_to_sec(uint64_t diff)
{
    return stm_sec(diff);
}

double ix_time_diff_to_ms(uint64_t diff)
{
    return stm_ms(diff);
}

double ix_time_diff_to_us(uint64_t diff)
{
    return stm_us(diff);
}

double ix_time_diff_to_ns(uint64_t diff)
{
    return stm_ns(diff);
}

#if ix_DO_TEST
ix_TEST_CASE("ix_time_diff_to_XXX")
{
    const uint64_t x = ix_time_now();
    const uint64_t y = ix_time_now();
    const uint64_t diff = y - x;
    ix_EXPECT(ix_time_diff_to_sec(diff) >= 0.0);
    ix_EXPECT(ix_time_diff_to_ms(diff) >= 0.0);
    ix_EXPECT(ix_time_diff_to_us(diff) >= 0.0);
    ix_EXPECT(ix_time_diff_to_ns(diff) >= 0.0);
}
#endif

uint64_t ix_time_lap(uint64_t *p)
{
    const uint64_t now = ix_time_now();
    const uint64_t lap = now - *p;
    *p = now;
    return lap;
}

#if ix_DO_TEST
ix_TEST_CASE("ix_time_lap")
{
    uint64_t p = ix_time_now();
    const uint64_t diff = ix_time_lap(&p);
    ix_EXPECT(ix_time_diff_to_ns(diff) >= 0.0);
}
#endif
