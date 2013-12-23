#ifndef UTILS_CPP
#define UTILS_CPP

#include <cstdlib>
#include <ctime>

bool SRAND_INITIALIZED = false;

void initSRand(){
  if (!SRAND_INITIALIZED){
    srand(time(NULL));
    SRAND_INITIALIZED = true;
  }
}

double randDouble(){
  initSRand();
  return 1.*(rand() % 100000) / 100000.;
}

// [0;max[
int randInt(int max){
  initSRand();
  return rand() % max;
}


#endif
