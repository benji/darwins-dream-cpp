#ifndef CELL_H
#define CELL_H

#include "common.h"

class Cell {
  public:
    Creature* creature;
    int x,y,z;
    float energy;

    Cell(Creature* creature, int x, int y, int z);
    ~Cell();
};

#endif
