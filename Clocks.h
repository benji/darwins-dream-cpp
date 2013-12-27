#ifndef CLOCKS_H
#define CLOCKS_H

#include <chrono>
#include <ctime>
#include <map>
#include "common.h"

class Timer {
  public:
    long long elapsed;
    long long lastTime;

    Timer();
    long long now();
    void start();
    void pause();
    void reset();
    void status(string msg);
};

class Clocks {
  public:
    map<int, Timer*> timers;

    Clocks();
    void start(int id);
    void reset(int id);
    void pause(int id);
    void status(int id, string msg);
    long long elapsed(int id);
    ~Clocks();
};

#endif


