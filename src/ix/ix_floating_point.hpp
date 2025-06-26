#pragma once

#include "ix.hpp"

template <typename T>
bool ix_is_approximately_equal(T x, T y);

template <typename T>
bool ix_is_approximately_equal_scaled(T x, T y, T scale);

extern template bool ix_is_approximately_equal(float x, float y);
extern template bool ix_is_approximately_equal(double x, double y);
extern template bool ix_is_approximately_equal_scaled(float x, float y, float scale);
extern template bool ix_is_approximately_equal_scaled(double x, double y, double scale);
