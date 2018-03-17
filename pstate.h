//
// Created by austi on 12/11/2017.
//

#ifndef BACH_PSTATE_H
#define BACH_PSTATE_H

struct pstate
{
    int64 key;
    int removed; //Int for storing any leading marbles that were removed
    int toMove;
    int doublesCnt;
    pstate *prev;

    bool isValid()
    {
        return key >= 0 && removed >= -1 && removed < 54 && toMove >= 0 && toMove <= TEAM_COUNT && doublesCnt >= 0 && doublesCnt <= 2;
    }
};

#endif //BACH_PSTATE_H
