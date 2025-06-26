#include "ix_HashMapSingleArray.hpp"

#if ix_DO_TEST
#include "ix_test.hpp"
#include "ix_test_HashMap.hpp"

ix_TEST_CASE("ix_HashMapSingleArray")
{
    ix_test_hash_map<ix_HashMapSingleArray>();
    ix_test_Map_iterator<ix_HashMapSingleArray, 1024>();
}
#endif
