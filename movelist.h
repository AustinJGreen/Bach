//
// Created by austi on 12/9/2017.
//

#ifndef BACH_MOVELIST_H
#define BACH_MOVELIST_H

#include <cassert>
#include "move.h"

#define MAX 69

class movelist
{
private:
    int64 list[MAX] = { NONE };
    int cnt;

public:
    movelist()
    {
        cnt = 0;
    }

    int64 operator[](int index)
    {
        assert(index >= 0 && index < cnt);
        return list[index];
    }

    void add(int64 move)
    {
        assert(cnt < MAX);
        list[cnt++] = move;
    }

    void removeAt(int index)
    {
        assert(index >= 0 && index < cnt);
        for (int j = index; j < cnt; j++)
        {
            list[j] = list[j + 1];
        }
        cnt--;
    }

    int count() const
    {
        return cnt;
    }
};

#endif