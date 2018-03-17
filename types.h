//
// Created by austi on 12/9/2017.
//

#ifndef BACH_TYPES_H
#define BACH_TYPES_H

#include "roll.h"

static roll ALL_NO_DUPL[21] = {
        roll(1, 1),
        roll(1, 2),
        roll(1, 3),
        roll(1, 4),
        roll(1, 5),
        roll(1, 6),
        roll(2, 2),
        roll(2, 3),
        roll(2, 4),
        roll(2, 5),
        roll(2, 6),
        roll(3, 3),
        roll(3, 4),
        roll(3, 5),
        roll(3, 6),
        roll(4, 4),
        roll(4, 5),
        roll(4, 6),
        roll(5, 5),
        roll(5, 6),
        roll(6, 6)
};

static roll ALL_TAKEOUTS[6] = {
        roll(1, 1),
        roll(1, 2),
        roll(1, 3),
        roll(1, 4),
        roll(1, 5),
        roll(1, 6)
};

#define NONE 9007199254740991
#define YELLOW 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define TEAM_COUNT 4
#define MARBLE_COUNT 5
#define NEXT_TEAM(current) ((current + 1) % TEAM_COUNT)
#define PREV_TEAM(current) ((current + (TEAM_COUNT - 1)) % TEAM_COUNT)

#ifndef MSVC
//#define MSVC
#endif

#ifndef MSVC
#ifndef FLT_MAX
#define FLT_MAX std::numeric_limits<float>::min()
#endif
#endif

#endif

