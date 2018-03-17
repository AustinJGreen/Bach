//
// Created by austi on 12/9/2017.
//

#include "roll.h"

roll::roll()
{
    die1 = -1;
    die2 = -1;
}

roll::roll(int die1, int die2)
{
    this->die1 = die1;
    this->die2 = die2;
}

int roll::getDie1() const
{
    return die1;
}

int roll::getDie2() const
{
    return die2;
}

int roll::total() const
{
    return die1 + die2;
}

bool roll::isDoubles() const
{
    return die1 == die2;
}

bool roll::canTakeOut() const
{
    return die1 == 1 || die1 == 6 || die2 == 1 || die2 == 6;
}