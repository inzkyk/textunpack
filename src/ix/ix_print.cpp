#include "ix_print.hpp"
#include "ix_Writer.hpp"

#include <inttypes.h>

size_t ix_Print<char>::print(ix_Writer &writer, char x)
{
    return writer.write_stringf("%c", x);
}

size_t ix_Print<int8_t>::print(ix_Writer &writer, int8_t x)
{
    return writer.write_stringf("%" PRId8, x);
}

size_t ix_Print<int16_t>::print(ix_Writer &writer, int16_t x)
{
    return writer.write_stringf("%" PRId16, x);
}

size_t ix_Print<int32_t>::print(ix_Writer &writer, int32_t x)
{
    return writer.write_stringf("%" PRId32, x);
}

size_t ix_Print<int64_t>::print(ix_Writer &writer, int64_t x)
{
    return writer.write_stringf("%" PRId64, x);
}

size_t ix_Print<uint8_t>::print(ix_Writer &writer, uint8_t x)
{
    return writer.write_stringf("%" PRIu8, x);
}

size_t ix_Print<uint16_t>::print(ix_Writer &writer, uint16_t x)
{
    return writer.write_stringf("%" PRIu16, x);
}

size_t ix_Print<uint32_t>::print(ix_Writer &writer, uint32_t x)
{
    return writer.write_stringf("%" PRIu32, x);
}

size_t ix_Print<uint64_t>::print(ix_Writer &writer, uint64_t x)
{
    return writer.write_stringf("%" PRIu64, x);
}

size_t ix_Print<float>::print(ix_Writer &writer, float x)
{
    return writer.write_stringf("%f", static_cast<double>(x));
}

size_t ix_Print<double>::print(ix_Writer &writer, double x)
{
    return writer.write_stringf("%lf", x);
}

size_t ix_Print<const void *>::print(ix_Writer &writer, const void *x)
{
    return writer.write_stringf("%p", x);
}

size_t ix_Print<decltype(nullptr)>::print(ix_Writer &writer, decltype(nullptr))
{
    writer.write_string("null");
    return ix_strlen("null");
}

#if ix_DO_TEST
#include "ix_test.hpp"

ix_TEST_CASE("ix_print")
{
    ix_EXPECT(ix_Print<int>::is_implemented);
    ix_EXPECT(ix_Print<float>::is_implemented);
    ix_EXPECT(ix_Print<double>::is_implemented);
    ix_EXPECT(ix_Print<void *>::is_implemented);
    ix_EXPECT(ix_Print<const char *>::is_implemented);
    ix_EXPECT(ix_Print<ix_nullptr_t>::is_implemented);
    ix_EXPECT(!ix_Print<ix_Print<int>>::is_implemented);
    ix_EXPECT(!ix_Print<ix_Writer>::is_implemented);

    char buf[128];
    auto writer = ix_Writer::from_existing_array(buf);

    ix_EXPECT_EQ(writer.print(int8_t{-123}), 4);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "-123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(int16_t{-123}), 4);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "-123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(int32_t{-123}), 4);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "-123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(int64_t{-123}), 4);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "-123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(uint8_t{123}), 3);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(uint16_t{123}), 3);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(uint32_t{123}), 3);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(uint64_t{123}), 3);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "123");
    writer.clear();

    ix_EXPECT_EQ(writer.print(12.3F), 9);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "12.300000");
    writer.clear();

    ix_EXPECT_EQ(writer.print(12.3), 9);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "12.300000");
    writer.clear();

    ix_EXPECT_EQ(writer.print(nullptr), 4);
    writer.end_string();
    ix_EXPECT_EQSTR(writer.data(), "null");
    writer.clear();

    const char *p = reinterpret_cast<const char *>(size_t{0xDEADBEEF});
    writer.print(p);
    writer.end_string();
    const bool upper = (ix_strstr(writer.data(), "DEADBEEF") != nullptr);
    const bool lower = (ix_strstr(writer.data(), "deadbeef") != nullptr);
    ix_EXPECT(upper || lower);
    writer.clear();
}
#endif
