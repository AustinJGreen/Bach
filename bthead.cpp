//
// Created by austi on 1/1/2018.
//

#include "bthread.h"

bthread::bthread() {
    error = 0;
    running = 0;
}

bool bthread::start(void *(*func)(void *), void* arg) {
    if (running) {
        return 0;
    }
    error = pthread_create(&thread, NULL, func, arg);
    if (error == 0)
    {
        running = 1;
        return 1;
    }
    else
    {
        running = 0;
        return 0;
    }
}

bool bthread::join() {
    if (running) {
        error = pthread_join(thread, NULL);
        running = 0;
        return error == 0;
    }
    else {
        return 0;
    }
}

int bthread::getError() const {
    return error;
}

bool bthread::isRunning() const {
    return running;
}
