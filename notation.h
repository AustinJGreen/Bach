//
// Created by austi on 12/9/2017.
//

#ifndef BACH_NOTATION_H
#define BACH_NOTATION_H

#include <string>
#include <cassert>
#include "move.h"
#include "pseudomove.h"
#include "magic.h"

class notation
{
private:
    static std::string teams[TEAM_COUNT];
    static char team_chars[TEAM_COUNT];
public:
    notation();
    static roll fromStr(std::string rollStr);
    static int fromStr(std::string sq, int perspective);
    static std::string toStr(int sq, int perspective);
    static std::string toStr(int64 move);
    static std::string toStr(int32 move);
    static std::string toStr(int team);
};

#endif