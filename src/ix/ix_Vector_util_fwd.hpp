#pragma once

#include "ix.hpp"

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, T &&x);

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, const T &x);

template <typename Vec, typename T>
void ix_Vector_insert(Vec &vec, T *pos, const T *start, const T *end);

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos);

template <typename Vec, typename T>
void ix_Vector_erase(Vec &vec, T *pos, size_t n);

// NOLINTBEGIN(bugprone-macro-parentheses)
#define ix_VECTOR_UTIL_FRIEND_DECLARE(cls)                                  \
    friend void ix_Vector_insert<cls, T>(cls &, T *, T && x);               \
    friend void ix_Vector_insert<cls, T>(cls &, T *, const T &x);           \
    friend void ix_Vector_insert<cls, T>(cls &, T *, const T *, const T *); \
    friend void ix_Vector_erase<cls, T>(cls &, T *);                        \
    friend void ix_Vector_erase<cls, T>(cls &, T *, size_t)
// NOLINTEND(bugprone-macro-parentheses)
