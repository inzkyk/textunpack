#pragma once

#include "ix.hpp"
#include "ix_hash.hpp"

template <typename T1, typename T2>
struct ix_Pair
{
    T1 first;
    T2 second;
};

template <typename T1, typename T2>
ix_FORCE_INLINE constexpr bool operator==(const ix_Pair<T1, T2> &x, const ix_Pair<T1, T2> &y)
{
    return (x.first == y.first) && (x.second == y.second);
}

template <typename T1, typename T2>
struct ix_Hash<ix_Pair<T1, T2>>
{
    ix_FORCE_INLINE constexpr static size_t hash(const ix_Pair<T1, T2> &pair)
    {
        return ix_wymix(ix_Hash<T1>::hash(pair.first), ix_Hash<T2>::hash(pair.second));
    }
};

template <typename Key, typename Value>
struct ix_KVPair
{
    Key key;
    Value value;
};
