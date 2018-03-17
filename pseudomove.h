//
// Created by austi on 12/9/2017.
//

#ifndef BACH_PSEUDOMOVE_H
#define BACH_PSEUDOMOVE_H

#define int32 unsigned long int

#include "types.h"
#include "magic.h"
#include "notation.h"

//from = 0-53 (6 bits) //0 (from is indexed +1) so -1 is 0
//to = 0-52 (6 bits) //6
//team = 0-4 (3 bits) //12
//capturedTeam = 0-5 (3 bits) //15 (indexed at 0, meaning no capture), 1 is YELLOW, etc.

inline int32 pseudomove_make(int team, int from, int to)
{
    return (team << 12) | (to << 6) | (from + 1);
}

inline bool pseudomove_isTakeout(int32 move)
{
    return (move & 63) == 0;
}

inline void pseudomove_setCapturedTeam(int32 &move, int capturedTeam)
{
    move = move | ((capturedTeam + 1) << 15);
}

inline void pseudomove_removeCapture(int32 &move)
{
    move = move & 4095;
}

inline int pseudomove_getFrom(int32 move)
{
    return (move & 63) - 1;
}

inline int pseudomove_getTo(int32 move)
{
    return (move >> 6) & 63;
}

inline int pseudomove_getTeam(int32 move)
{
    return (move >> 12) & 7;
}

inline int pseudomove_getCapturedTeam(int32 move)
{
    return ((move >> 15) & 7) - 1;
}


inline bool pseudomove_isValid(int32 move)
{
    return pseudomove_getFrom(move) >= -1 && pseudomove_getFrom(move) <= 52 && pseudomove_getTo(move) >= 0 &&
            pseudomove_getTo(move) < 53 && pseudomove_getTo(move) > pseudomove_getFrom(move);
}
#endif