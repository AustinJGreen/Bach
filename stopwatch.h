//
// Created by austi on 12/9/2017.
//

#ifndef BACH_STOPWATCH_H
#define BACH_STOPWATCH_H

#include <chrono>
class stopwatch
{
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running;
public:
    stopwatch();

    void start();
    void stop();
    void reset();
    void restart();

    long long int elapsedNs() const;
    long long int elapsedUs() const;
    long long int elapsedMs() const;
};

#endif