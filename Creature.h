#ifndef CREATURE_H
#define CREATURE_H

#include "common.h"
#include "Cell.h"
#include "Species.h"

class Cell;
class Species;

class Creature {
  public:
    Species& species;
    int x,y;
    long creationCycle;
    vector<Cell*> cells; // TODO : use a vector, only additions here
    Creature(Species& species, int x, int y);
    void createCell(int x, int y, int z, bool registerCell);
    void grow();
    void die();
    void growNewCell(Cell* c, float* growthProbas);
};

#endif
