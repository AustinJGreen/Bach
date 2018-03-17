//
// Created by austi on 12/9/2017.
//

#ifndef BACH_TESTS_H
#define BACH_TESTS_H
#include "eval.h"
#include "random.h"
#include "move.h"
#include "roll.h"

#include <iostream>
#include <cassert>

class tests
{
public:
    static void testMagic();
    static void testNotation();
    static void testMoveGeneration();
    static void testPosition();
    static void testHashing();
    static void testMoves();
    static void testEval();
    static void testSearch();
    static void testMemory();
};

#endif