#ifndef CELL_H
#define CELL_H

#include "common.h"

class Cell {
  public:
    int x,y,z;
    Cell(int x, int y, int z);
    ~Cell();
};

#endif
