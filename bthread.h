//
// Created by austi on 1/1/2018.
//

#ifndef BACH_BTHREAD_H
#define BACH_BTHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

class bthread
{
private:
    int error;
    bool running;
    pthread_t thread;
public:
    bthread();
    bool start(void* (*func)(void*), void* arg);
    bool join();
    int getError() const;
    bool isRunning() const;
};

#endif //BACH_BTHREAD_H
