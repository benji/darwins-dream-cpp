#include "Clocks.h"

#include <chrono>
#include <ctime>
#include <map>
#include "common.h"

Timer::Timer(){ reset(); }

long long Timer::now(){
  std::chrono::time_point<std::chrono::high_resolution_clock> tp = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count();
}

void Timer::start(){
  ++count;
  lastTime = now();
}

void Timer::pause(){
  long long n = now();
  elapsed += n - lastTime;
  lastTime = 0;
}

void Timer::reset(){
  //cout<<"reseting with count="<<count<<endl;
  elapsed = 0;
  lastTime = 0;
  count = 0;
}

void Timer::status(string msg){
  long long e = elapsed;
  if (lastTime != 0) {
    e += (now() - lastTime);
  }
  cout<<"CLOCK: "<< (e/1000) <<"ms : "<<msg << " (called "<<count<<" times)"<< endl;
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

Clocks::~Clocks(){std::map<std::string, std::string>::iterator iter;
  map<int, Timer*>::iterator it;
  for (it = timers.begin(); it != timers.end(); ++it) {
    delete it->second;
  }
}

