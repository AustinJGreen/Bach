//
// Created by austi on 12/15/2017.
//

#ifndef BACH_MCTS_H
#define BACH_MCTS_H

#include "position.h"
#include "random.h"
#include "stopwatch.h"
#include "bthread.h"
#include "bmutex.h"
#include <iostream>

namespace mcts
{
    #define THREAD_COUNT 8

    //Global thread readonly variables
    static position *start;
    static roll r;
    static movelist moveList;
    static int searchTime;

    //Thread variables
    static int moveNumber; //Current move number (needs to be thread safe)
    static float* arr; //move scores

    //Thread variable mutexes
    static bmutex m_moveNumber;
    static bmutex m_arr;
    static bmutex m_stop;
    static bmutex m_games;

    //Infinite search vars
    static bool stop;
    static int* gameCnt;
    static bthread threads[THREAD_COUNT];

    void init() {
        m_moveNumber = bmutex();
        m_arr = bmutex();
        m_stop = bmutex();
        m_games = bmutex();
    }

    void dispose() {
        if (arr != nullptr) {
            //Delete evals
            delete[] arr;
            arr = nullptr;
        }

        if (gameCnt != nullptr) {
            delete[] gameCnt;
            gameCnt = nullptr;
        }

        //Delete resources
        if (start != nullptr) {
            delete start;
            start = nullptr;
        }
    }

    void montecarlo(int64 move, int time, int idx) {
        int us = start->getToMove();

        stopwatch sw = stopwatch();
        sw.start();

        position p = position(*start);
        p.doMove(move, r);

        int wins = 0;
        int losses = 0;

        while (sw.elapsedMs() < time)
        {
            position sim = position(p);
            while (sim.getWinner() == -1 && sw.elapsedMs() < time)
            {
                roll randomRoll = roll(random::next(1, 7), random::next(1, 7));
                movelist ml = sim.getLegalMoves(randomRoll, sim.getToMove());
                if (ml.count() > 0)
                {
                    int moveIndex = random::next(0, ml.count());
                    int64 randomMove = ml[moveIndex];
                    sim.doMove(randomMove, randomRoll);
                }
                else
                {
                    sim.next(randomRoll);
                }
            }

            if (sim.getWinner() == us)
            {
                wins++;
            }
            else if (sim.getWinner() != -1)
            {
                losses++;
            }
        }

        if (losses == 0)
        {
            m_arr.lock();
            //std::cout << "games simulated for " << notation::toStr(move) << " = " << (wins + losses) << std::endl;
            arr[idx] = wins;
            m_arr.unlock();
        }
        else
        {
            m_arr.lock();
            //std::cout << "games simulated for " << notation::toStr(move) << " = " << (wins + losses) << std::endl;
            arr[idx] = wins / (float)(wins + losses);
            m_arr.unlock();
        }
    }

    void* startNext(void* ptr) {
        m_moveNumber.lock();
        int mn = moveNumber;
        moveNumber++;
        m_moveNumber.unlock();

        if (mn < moveList.count())
        {
            montecarlo(moveList[mn], searchTime / moveList.count(), mn);
        }
    }

    int64 search(position pos, const roll &roll, int time) {
        assert(start == nullptr);
        //Get candidates
        movelist ml = pos.getLegalMoves(roll, pos.getToMove());
        if (ml.count() == 0)
        {
            return NONE;
        }
        else if (ml.count() == 1)
        {
            return ml[0];
        }

        const int threadCount = 8;
        int epochs = ml.count() / threadCount;
        if (epochs == 0)
        {
            epochs = 1; //Always one epoch
        }

        //Initialize global variables that are used by threads
        start = new position(pos);
        r = roll;
        moveList = ml;
        searchTime = time / epochs;
        moveNumber = 0;
        arr = new float[ml.count()];
        for (int j = 0; j < ml.count(); j++)
        {
            arr[j] = 0;
        }

        //Thread variables
        bthread threads[threadCount];

        //Create and start running threads
        for (int i = 0; i < ml.count(); i++)
        {
            int id = i % threadCount;
            if (threads[id].isRunning())
            {
               //Wait for room for another thread
               threads[id].join();
            }

            //Start searches
            threads[id].start(startNext, NULL);
        }

        //Wait for all threads to complete
        for (int j = 0; j < ml.count(); j++)
        {
            threads[j].join();
        }

        //Get best move
        int64 bestMove = ml[0];
        float bestEval = arr[0];
        for (int j = 1; j < ml.count(); j++)
        {
            if (arr[j] > bestEval || (arr[j] == bestEval && random::next(0, 2) == 0))
            {
                bestEval = arr[j];
                bestMove = ml[j];
            }
        }

        dispose();
        return bestMove;
    }

    void* rotatingMonteCarlo(void* ptr) {
        roll moveRoll = r;

        int us = start->getToMove();

        movelist origMl = start->getLegalMoves(moveRoll, us);

        int moveNum = 0; //Cycle through moves for each game
        while (true)
        {
            m_stop.lock();
            if (stop)
            {
                m_stop.unlock();
                break;
            }
            m_stop.unlock();

            position sim = *start;
            int64 m = origMl[moveNum];
            sim.doMove(m, moveRoll);

            while (sim.getWinner() == -1)
            {
                m_stop.lock();
                if (stop)
                {
                    m_stop.unlock();
                    break;
                }
                m_stop.unlock();

                roll randomRoll = roll(random::next(1, 7), random::next(1, 7));
                movelist curMl = sim.getLegalMoves(randomRoll, sim.getToMove());
                if (curMl.count() > 0)
                {
                    int moveIndex = random::next(0, curMl.count());
                    int64 randomMove = curMl[moveIndex];
                    sim.doMove(randomMove, randomRoll);
                }
                else
                {
                    sim.next(randomRoll);
                }
            }

            m_games.lock();
            gameCnt[moveNum]++;
            m_games.unlock();

            //Update score
            if (sim.getWinner() == us)
            {
                m_arr.lock();
                arr[moveNum]++;
                m_arr.unlock();
            }

            moveNum++;
            moveNum %= origMl.count();
        }
    }

    void searchInf(position pos, const roll &roll) {
        assert(start == nullptr);

        //Get candidates
        movelist ml = pos.getLegalMoves(roll, pos.getToMove());
        if (ml.count() == 0)
        {
            return;
        }
        else if (ml.count() == 1)
        {
            return;
        }

        //Initialize global variables that are used by threads
        r = roll;
        start = new position(pos);
        arr = new float[ml.count()];
        gameCnt = new int[ml.count()];
        stop = false;

        for (int j = 0; j < ml.count(); j++)
        {
            arr[j] = 0;
            gameCnt[j] = 0;
        }

        //Create and start running threads
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            //Start searches
            threads[i].start(rotatingMonteCarlo, NULL);
        }
    }

    int64 stopSearch() {
        m_stop.lock();
        stop = true;
        m_stop.unlock();

        //Wait for all threads to exit
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            //Start searches
            threads[i].join();
        }

        movelist ml = start->getLegalMoves(r, start->getToMove());
        if (ml.count() == 0)
        {
            return NONE;
        }
        else if (ml.count() == 1)
        {
            return ml[0];
        }

        //Get best move
        int64 bestMove = NONE;
        float bestEval = -1;
        for (int j = 0; j < ml.count(); j++)
        {
            float curEval = arr[j] / ((gameCnt[j] == 0) ? 1 : gameCnt[j]);
            std::cout << notation::toStr(ml[j]) << " = " << curEval << " (" << arr[j] << "/" << gameCnt[j] << ")" << std::endl;
            if (j == 0 || curEval > bestEval || (curEval == bestEval && random::next(0, 2) == 0))
            {
                bestEval = curEval;
                bestMove = ml[j];
            }
        }

        dispose();
        return bestMove;
    }
}

#endif //BACH_MCTS_H
