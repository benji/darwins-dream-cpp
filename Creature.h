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
    vector<Cell*> cells;
    Creature(Species& species, int x, int y);
    Cell* createCell(int x, int y, int z);
    Cell* grow();
    Cell* growNewCell(Cell* c, float* growthProbas, int growthDirection);
    bool hasEnoughEnergy();
    bool isBalanced();
    ~Creature();
};

#endif
