#pragma once

#include "ix.hpp"

namespace ix_detail::hash_container
{

constexpr uint32_t DISP_LENGTH = 8;
constexpr uint32_t HIGHER_HASH_LENGTH = 32 - DISP_LENGTH;
constexpr uint32_t HIGHER_HASH_MASK = (1UL << HIGHER_HASH_LENGTH) - 1;
constexpr size_t INITIAL_LENGTH = 8;
constexpr size_t MAX_LOAD_FACTOR_NUMERATOR = 8;
constexpr size_t MAX_LOAD_FACTOR_DENOMINATOR = 10;

struct Bucket
{
    uint32_t disp_and_higher_hash;
    uint32_t index;
};

uint32_t increment_disp(uint32_t disp_and_higher_hash);
void emplace_bucket(Bucket *bucket, Bucket bucket_to_emplace, Bucket *buckets_begin, Bucket *buckets_end);

} // namespace ix_detail::hash_container
