cmake_minimum_required(VERSION 3.9)
project(Bach)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++0x -pthread")

add_executable(Bach main.cpp move.h movelist.h notation.h position.cpp position.h pseudomove.h random.cpp random.h roll.cpp roll.h score.cpp score.h search.cpp search.h stopwatch.cpp stopwatch.h tests.cpp tests.h types.h eval.cpp notation.cpp magic.h magic.cpp pseudomove.cpp move.cpp pstate.h prng.h mcts.h bthread.h bthead.cpp bmutex.h)