//
// Created by austi on 12/9/2017.
//

#ifndef BACH_RANDOM_H
#define BACH_RANDOM_H

#include <stdlib.h>
#include <time.h>

class random
{
public:
    static void init();
    static int next(int min, int max);
};

#endif