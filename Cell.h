#ifndef CELL_H
#define CELL_H

#include "common.h"

class Cell {
  public:
    int x,y,z;
    float energy;
    Cell(int x, int y, int z, bool registerCell);
    ~Cell();
};

#endif
