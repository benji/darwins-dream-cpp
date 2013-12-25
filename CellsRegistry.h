#ifndef CELLS_REGISTRY_H
#define CELLS_REGISTRY_H

#include "Cell.h"

class CellsRegistry {
  public:
    CellsRegistry();
    void registerCell(Cell* c);
    void unregisterCell(Cell* c);
};

#endif
