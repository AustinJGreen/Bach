//
// Created by austi on 12/9/2017.
//

#include "stopwatch.h"

stopwatch::stopwatch()
{
    start_time = std::chrono::high_resolution_clock::now();
    running = false;
}

void stopwatch::start()
{
    if (!running)
    {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }
}

void stopwatch::stop()
{
    if (running)
    {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }
}

void stopwatch::reset()
{
    start_time = std::chrono::high_resolution_clock::now();
    end_time = std::chrono::high_resolution_clock::now();
    running = false;
}

void stopwatch::restart()
{
    start_time = std::chrono::high_resolution_clock::now();
    running = true;
}

long long int stopwatch::elapsedNs() const
{
    std::chrono::high_resolution_clock::time_point end;
    if (running)
    {
        end = std::chrono::high_resolution_clock::now();
    }
    else
    {
        end = end_time;
    }

    std::chrono::nanoseconds duration = (end - start_time);
    return duration.count();
}

long long int stopwatch::elapsedUs() const
{
    return elapsedNs() / 1000;
}

long long int stopwatch::elapsedMs() const
{
    return elapsedUs() / 1000;
}