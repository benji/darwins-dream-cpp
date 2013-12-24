#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

bool SRAND_INITIALIZED = false;

void initSRand(){
  if (!SRAND_INITIALIZED) {
    srand(time(0));
    SRAND_INITIALIZED = true;
  }
}

double randDouble(){
  initSRand();
  return 1.*(rand() % 100000) / 100000.;
}

// [0;max[
int randInt(int max){
  if (max <= 0){
    cerr << "randInt: Invalid max: "<<max << endl;
    exit(-1);
  }
  initSRand();
  return rand() % max;
}

