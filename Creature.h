#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "Cell.h"
#include <string>
#include "Species.h"

using namespace std;

class Species;

class Creature {
  public:
    Species& species;
    int x,y;
    string name;
    vector<Cell*> cells; // TODO : use a vector, only additions here
    Creature(Species& species, int x, int y);
    void createCell(int x, int y, int z);
    void grow();
    void die();
    void growNewCell(Cell* c, float* growthProbas);
};

#endif
