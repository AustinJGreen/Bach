//
// Created by austi on 12/9/2017.
//

#ifndef BACH_SCORE_H
#define BACH_SCORE_H

#include "position.h"
#include <algorithm>

class score
{
private:
    float values[TEAM_COUNT];

    void swap(float arr[], int indicies[], int a, int b) const;
    void quickSort(float arr[], int indicies[], int low, int high) const;
    int partition(float arr[], int indicies[], int low, int high) const;
public:
    score();
    float operator[](int index);
    float getVector(int player) const;
    void set(int player, float value);
};

#endif