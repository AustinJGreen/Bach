//
// Created by austi on 12/9/2017.
//

#include "move.h"

bool move_isValid(int64 m)
{
    if (move_isTwoStep(m))
    {
        return pseudomove_isValid(move_getFirst(m)) && pseudomove_isValid(move_getSecond(m));
    }
    else
    {
        return pseudomove_isValid(move_getFirst(m));
    }
}