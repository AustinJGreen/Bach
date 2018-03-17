//
// Created by austi on 12/9/2017.
//

#include "random.h"

void random::init()
{
    srand(time(NULL));
}

int random::next(int min, int max)
{
    return min + (rand() % (max - min));
}