#pragma once

#include "ix.hpp"
#include "ix_Option.hpp"

void ix_skip_to_next_word(const char **c);
const char *ix_read_string(const char **c);
int ix_read_int(const char **c);
unsigned int ix_read_uint(const char **c);
float ix_read_float(const char **c);
void ix_next_line(const char **c);
void ix_skip_empty_lines(const char **c);
void ix_next_non_empty_line(const char **c);

template <typename T>
ix_Option<T> ix_string_convert(const char *s);

extern template ix_Option<float> ix_string_convert<float>(const char *s);
extern template ix_Option<double> ix_string_convert<double>(const char *s);
extern template ix_Option<long> ix_string_convert<long>(const char *s);
extern template ix_Option<long long> ix_string_convert<long long>(const char *s);
extern template ix_Option<unsigned long> ix_string_convert<unsigned long>(const char *s);
extern template ix_Option<unsigned long long> ix_string_convert<unsigned long long>(const char *s);
