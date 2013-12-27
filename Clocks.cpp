#include "Clocks.h"

#include <chrono>
#include <ctime>
#include <map>
#include "common.h"

Timer::Timer(){ reset(); }

long long Timer::now(){
  std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
}

void Timer::start(){
  lastTime = now();
}

void Timer::pause(){
  long long n = now();
  elapsed += n - lastTime;
  lastTime = 0;
}

void Timer::reset(){
  elapsed = 0;
  lastTime = 0;
}

void Timer::status(string msg){
  long long e = elapsed;
  if (lastTime != 0) {
    e += (now() - lastTime);
  }
  cout<<"CLOCK: "<< e <<"ms : "<<msg << endl;
}


Clocks::Clocks(){
}

void Clocks::start(int id){
  if (timers.find( id ) == timers.end()) {
    timers[id] = new Timer();
  }
  timers[id]->start();
}

void Clocks::reset(int id){
  if (timers[id] != NULL) { timers[id]->reset(); }
}

void Clocks::pause(int id){
  if (timers[id] != NULL) { timers[id]->pause(); }
}

void Clocks::status(int id, string msg){
  if (timers[id] != NULL) { timers[id]->status(msg); }
}

long long Clocks::elapsed(int id){
  if (timers[id] != NULL) { return timers[id]->elapsed; }
  return 0LL;
}

Clocks::~Clocks(){
  // TODO delete timers!
}

