#ifndef WORLD_H
#define WORLD_H

#include <list>
#include "Species.h"

using namespace std;

class World {
  public:
    list<Species> species;
    int length;
    World(int length);
    void infest(int nbSpecies, int nbCreaturesPerSpecies);
    int* findFreeGroundPos();
};

#endif
