#include "ix.hpp"

#include <stdio.h>
#include <stdlib.h>

static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(int16_t) == 2);
static_assert(sizeof(int32_t) == 4);
static_assert(sizeof(int64_t) == 8);
static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(uint32_t) == 4);
static_assert(sizeof(uint64_t) == 8);

#if ix_PLATFORM(WASM)
static_assert(sizeof(size_t) == 4);
static_assert(sizeof(ptrdiff_t) == 4);
#else
static_assert(sizeof(size_t) == 8);
static_assert(sizeof(ptrdiff_t) == 8);
#endif

[[noreturn]] void ix_abort()
{
    abort();
}

[[noreturn]] void ix_abort_with_info(const char *file, int line, const char *msg)
{
    ix_UNUSED(fprintf(stderr, "\n%s:%d: %s\n", file, line, msg));
    ix_abort();
}
