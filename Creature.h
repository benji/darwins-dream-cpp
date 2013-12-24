#ifndef CREATURE_H
#define CREATURE_H

#include <list>
#include "Cell.h"
#include "Species.h"

using namespace std;

class Species;

class Creature {
  public:
    Species& species;
    int x,y;
    list<Cell> cells;
    Creature(Species& species, int x, int y);
    void createCell(int x, int y, int z);
    void grow();
    void growNewCell(Cell c, float* growthProbas);
};

#endif
