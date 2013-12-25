#ifndef WORLD_H
#define WORLD_H

#include <list>
#include "Species.h"

using namespace std;

class World {
  public:
    list<Species*> species;
    long cycle;
    int length;
    float reproductionRate, mutationRate;
    World(int length, float reproductionRate, float mutationRate);
    void infest(int nbSpecies, int nbCreaturesPerSpecies);
    int* findFreeGroundPos(int* pos);
    void lifecycle();
    void reproduce(Species* s);
};

#endif
