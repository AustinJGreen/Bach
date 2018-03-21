#include <iostream>
#include <sstream>
#include <string>
#include "random.h"
#include "tests.h"
#include "stopwatch.h"
#include "mcts.h"

using namespace std;

void simulate(bool outputMoves, int timePerMove, int games) {
    stopwatch sw = stopwatch();
    sw.start();

    position p = position();
    for (int i = 0; i < games; i++)
    {
        p.reset();
        while (p.getWinner() == -1)
        {
            roll randomRoll = roll(random::next(1, 7), random::next(1, 7));

            std::string playerStr = notation::toStr(p.getToMove());
            if (outputMoves) {
                std::cout << notation::toStr(p.getToMove()) << " rolls " << randomRoll.getDie1() << "+" << randomRoll.getDie2() << std::endl;
            }

            movelist ml = p.getLegalMoves(randomRoll, p.getToMove());
            if (ml.count() > 0)
            {
                if (p.getToMove() == YELLOW)
                {
                    int64 best = mcts::search(p, randomRoll, timePerMove);
                    if (outputMoves) {
                        std::cout << notation::toStr(p.getToMove()) << " plays " << notation::toStr(best) << std::endl;
                    }
                    p.doMove(best, randomRoll);
                }
                else
                {
                    int moveIndex = random::next(0, ml.count());
                    int64 move = ml[moveIndex];
                    if (outputMoves) {
                        std::cout << notation::toStr(p.getToMove()) << " plays " << notation::toStr(move) << std::endl;
                    }
                    p.doMove(move, randomRoll);
                }
            }
            else
            {
                p.next(randomRoll);
            }
        }

        std::cout << notation::toStr(p.getWinner()) << " won." << std::endl;
    }

    sw.stop();
    std::cout << "elapsed ms = " << sw.elapsedMs() << std::endl;
}

void commandLine()
{
    position pos = position();
    string cmd, token;
    bool rollSet = false;
    roll lastRoll = roll(1, 1);

    do {
        getline(cin, cmd);
        istringstream is(cmd);
        token.clear();
        is >> skipws >> token;

        if (token  == "newgame") {
            pos.reset();
        }

        if (token == "roll") {
            is >> token;
            string rollStr = token;
            lastRoll = notation::fromStr(rollStr);

            movelist ml = pos.getLegalMoves(lastRoll, pos.getToMove());
            if (ml.count() == 0)
            {
                pos.next(lastRoll);
            }
            else if (ml.count() == 1)
            {
                int64 onlyMove = ml[0];
                cout << "move " << notation::toStr(onlyMove) << endl;
                pos.doMove(onlyMove, lastRoll);
            }

            rollSet = true;
        }

        if (token == "tomove") {
            std::string playerStr = notation::toStr(pos.getToMove());
            cout << playerStr << endl;
        }

        if (token == "move")
        {
            if (cmd.length() <= 5) {
                cout << "Illegal move" << endl;
            } else {
                string moveStr = cmd.substr(5);

                int64 move = NONE;
                movelist ml = pos.getLegalMoves(lastRoll, pos.getToMove());
                for (int i = 0; i < ml.count(); i++) {
                    std::string cur = notation::toStr(ml[i]);
                    if (cur == moveStr) {
                        move = ml[i];
                    }
                }

                if (!pos.isLegal(move)) {
                    cout << "Illegal move" << endl;
                } else {
                    pos.doMove(move, lastRoll);
                }
            }
        }

        if (token == "moves") {
            if (!rollSet) {
                cout << "No dice has been rolled yet." << endl << "Please enter the rolled dice using the command: roll x+y" << endl;
            }
            else {
                movelist ml = pos.getLegalMoves(lastRoll, pos.getToMove());
                if (ml.count() == 0) {
                    cout << "No moves" << endl;
                }
                else {
                    for (int j = 0; j < ml.count(); j++) {
                        cout << notation::toStr(ml[j]) << endl;
                    }
                }
            }
        }

        if (token == "go") {
            while (is >> token)
            {
                if (token == "infinite") {
                    mcts::searchInf(pos, lastRoll);
                }
                else {
                    int ms = stoi(token);
                    int64 best = mcts::search(pos, lastRoll, ms);
                    cout << "bestmove " << notation::toStr(best) << endl;
                }
            }
        }

        if (token == "stop") {
            int64 best = mcts::stopSearch();
            cout << "bestmove " << notation::toStr(best) << endl;
        }

    } while (cmd != "quit"); // Command line args are one-shot

    //Make sure all searches stop
    mcts::stopSearch();
}

int main() {
    //TODO:
    //-Store probability of rolls during game, use to evaluate
    //-Take full advantage of thread count (ex: 4 threads, 2 moves, double up threads)

    //Inits
    position::init();
    random::init();
    mcts::init();

    //Tests
    tests::testMagic();
    tests::testNotation();
    tests::testMoves();
    tests::testMoveGeneration();
    tests::testPosition();
    tests::testHashing();
    tests::testEval();
    tests::testSearch();
    tests::testMemory();

    commandLine();
    //simulate(false, 10000, 10);

    return 0;
}

