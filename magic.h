//
// Created by austi on 12/9/2017.
//

#ifndef BACH_MAGIC_H
#define BACH_MAGIC_H

#include "types.h"
#include <intrin.h>

typedef unsigned long long int int64;

#ifdef MSVC
inline int64 magic_marbleCount(int64 value)
{
    return __popcnt64(value);
}
#else
inline int64 magic_marbleCount(int64 value)
{
    return __builtin_popcountll(value);
}
#endif

inline bool magic_isValid(int64 value)
{
    return (value >> 53) == 0 && magic_marbleCount(value) <= 5;
}

inline bool magic_isWon(int64 value)
{
    return (value & 8725724278030336UL) == 8725724278030336UL;
}

inline int64 magic_get(int64 value, int hole)
{
    return (value & ((int64)1 << hole)) >> hole;
}

inline int64 magic_getRange(int64 value, int from, int to)
{
    return (value >> from) & (((int64)1 << (to - from + 1)) - 1);
}

inline int64 magic_getUnsafe(int64 value)
{
    return value & (((int64)1 << 48) - 1);
}

inline int64 magic_getSafe(int64 value)
{
    return value & ((int64)8725724278030336UL);
}

inline void magic_set(int64 &value, int hole)
{
    value |= ((int64)1 << hole);
}

#ifdef MSVC
inline int magic_getMsb(int64 value)
{
    unsigned long idx;
    _BitScanReverse64(&idx, value);
    return (int64)idx;
}
#else
inline int magic_getMsb(int64 value)
{
    return 63 ^ __builtin_clzll(value);
}
#endif

#ifdef MSVC
inline int magic_getLsb(int64 value)
{
    unsigned long idx;
    _BitScanForward64(&idx, value);
    return (int64)idx;
}
#else
inline int magic_getLsb(int64 value)
{
    return __builtin_ctzll(value);
}
#endif

int64 magic_getRot48(int64 value, int from_team, int to_team);

inline void magic_remove(int64 &value, int hole)
{
    //Safe, removes if hole is taken, unchanged otherwise
    value = ~(~value | ((int64)1 << hole));
}

inline void magic_qremove(int64 &value, int hole)
{
    //Quick remove, unsafe (hole MUST BE SET)
    value ^= ((int64)1 << hole);
}

int magic_popMsb(int64 &value);

#endif