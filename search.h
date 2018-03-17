//
// Created by austi on 12/9/2017.
//

#ifndef BACH_SEARCH_H
#define BACH_SEARCH_H
#include "position.h"

class search
{
private:
    position current_pos;
    roll current_roll;

    int double_cnt;
    int root_player;
    int current_player;
    int next_player;
    int depth;
public:
    search(position pos, roll roll, int root, int current, int depth);
    search next_search(position& next_pos, const roll& next_roll);

    position getPos() const;
    roll getRoll() const;
    int getRootPlayer() const;
    int getCurPlayer() const;
    int getDepth() const;
};

#endif
