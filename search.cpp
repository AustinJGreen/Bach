//
// Created by austi on 12/9/2017.
//

#include "search.h"

search::search(position pos, roll roll, int root, int curplayer, int depth)
{
    current_pos = pos;
    current_roll = roll;
    root_player = root;
    current_player = curplayer;
    double_cnt = 0;
    this->depth = depth;
}

search search::next_search(position& next_pos, const roll& next_roll)
{
    int next_player = current_player;
    if (!current_roll.isDoubles())
    {
        next_player = (current_player + 1) % TEAM_COUNT;
    }

    search next = search(next_pos, next_roll, root_player, next_player, depth - 1);
    if (current_roll.isDoubles())
    {
        next.double_cnt = double_cnt + 1;
        if (next.double_cnt >= 3)
        {
            next_pos.removeLeading(next.current_player);
            next.double_cnt = 0;
            next.next_player = (next.current_player + 1) % TEAM_COUNT;
        }
    }

    return next;
}

position search::getPos() const
{
    return current_pos;
}

roll search::getRoll() const
{
    return current_roll;
}

int search::getRootPlayer() const
{
    return root_player;
}

int search::getCurPlayer() const
{
    return current_player;
}

int search::getDepth() const
{
    return depth;
}