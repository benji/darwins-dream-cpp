#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "Species.h"
#include "CellsRegistry.h"

class Species;

class World {
  public:
    list<Species*> species;
    long cycle;
    int length;
    float reproductionRate, mutationRate;
    CellsRegistry registry;

    World(int length, float reproductionRate, float mutationRate);
    void infest(int nbSpecies, int nbCreaturesPerSpecies);
    int* findFreeGroundPos(int* pos);
    void lifecycle();
    void reproduce(Species* s);
};

#endif
