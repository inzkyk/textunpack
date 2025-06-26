#include "ix_defer.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"

ix_TEST_CASE("ix_defer")
{
    bool ok = false;
    {
        auto _ = ix_defer([&]() { ok = true; });
    }
    ix_EXPECT(ok);
}

ix_TEST_CASE("ix_DEFER")
{
    bool ok = false;
    {
        ix_DEFER([&]() { ok = true; });
    }
    ix_EXPECT(ok);
}
#endif
