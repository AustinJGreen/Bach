//
// Created by austi on 12/9/2017.
//

#ifndef BACH_MOVE_H
#define BACH_MOVE_H

#include <assert.h>
#include "pseudomove.h"

//bit1 = has two step
//1-19 bits = pseudomove1
//20-37 bits = pseudomove2

inline int64 move_make(int32 pmove)
{
    return pmove << 1; //Skip setting two step bit
}

inline int64 move_make(int32 move1, int32 move2)
{
    return 1 | ((int64)move1 << 1) | ((int64)move2 << 19);
}

inline int32 move_getFirst(int64 m)
{
    return (m >> 1) & 262143;
}

inline int32 move_getSecond(int64 m)
{
    return (m >> 19) & 262143;
}

inline bool move_isTwoStep(int64 m)
{
    return m & 1;
}

bool move_isValid(int64 m);


#endif