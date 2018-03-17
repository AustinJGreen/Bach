//
// Created by austi on 1/1/2018.
//

#ifndef BACH_BMUTEX_H
#define BACH_BMUTEX_H

#include <windows.h>

struct bmutex {
    bmutex() { InitializeCriticalSection(&cs); }
 ~bmutex() { DeleteCriticalSection(&cs); }
  void lock() { EnterCriticalSection(&cs); }
  void unlock() { LeaveCriticalSection(&cs); }

private:
  CRITICAL_SECTION cs;
};

#endif //BACH_BMUTEX_H
