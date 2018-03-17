//
// Created by austi on 12/9/2017.
//

#ifndef BACH_EVAL_H
#define BACH_EVAL_H

#include "position.h"
#include "score.h"
#include <iostream>

class eval
{
private:
    roll all_rolls[36];
    roll doubles_rolls[6];
    float dis_lookup[36];
    float dis_lookup_doubles[36];
public:
    eval();

    float calc_lookup(int dis, int doubleCnt, bool doublesOnly) const;
    float calc_prob(int64 magic, int from, int to) const;

    /*
    Scores the marble from 0 to 1, 1 being a scored marble, 0 being a dead marble
    */
    float score_marble(const position& pos, int team, int marble) const;

    /*
    Scores the team from 0 to 5, 5 being all marbles scored
    */
    float score_team(const position& pos, int team) const;

    score score_pos(const position& pos) const;

    float evaluate_marble(const position& pos, int team, int marble) const;
    float evaluate_team(const position& pos, int team) const;
    score evaluate_pos(const position& pos) const;
};

#endif