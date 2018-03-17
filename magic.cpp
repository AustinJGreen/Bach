//
// Created by austi on 12/9/2017.
//

#include "magic.h"

int64 magic_getRot48(int64 value, int from_team, int to_team)
{
    int64 unsafeHoles = magic_getUnsafe(value);
    if (from_team == to_team)
    {
        return unsafeHoles;
    }

    //CURRENT RELATIVE TO YELLOW
    int delta = (from_team - to_team) * 12;
    if (delta < 0)
    {
        delta += 48;
    }

    int64 unchangedMask = (((int64)1 << (48 - delta)) - 1);
    int64 same = unsafeHoles & unchangedMask;//bits rotating left not past the 48th bit
    int64 mod = unsafeHoles >> (48 - delta); //bits being rotated past 48bits
    int64 rot = _rotl64(same, delta);
    return rot | mod;
}

int magic_popMsb(int64 &value)
{
    int msb = magic_getMsb(value);
    value &= (((int64)1 << msb) - 1);
    return msb;
}