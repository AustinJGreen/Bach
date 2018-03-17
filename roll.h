//
// Created by austi on 12/9/2017.
//

#ifndef BACH_ROLL_H
#define BACH_ROLL_H

class roll
{
private:
    int die1;
    int die2;
public:
    roll();
    roll(int die1, int die2);

    int getDie1() const;
    int getDie2() const;
    int total() const;

    bool isDoubles() const;
    bool canTakeOut() const;
};

#endif