#pragma once

#include "ix.hpp"
#include "ix_Result.hpp"

ix_Result ix_time_init();
uint64_t ix_time_now();
uint64_t ix_time_lap(uint64_t *p);

double ix_time_diff_to_sec(uint64_t diff);
double ix_time_diff_to_ms(uint64_t diff);
double ix_time_diff_to_us(uint64_t diff);
double ix_time_diff_to_ns(uint64_t diff);
