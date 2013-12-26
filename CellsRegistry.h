#ifndef CELLS_REGISTRY_H
#define CELLS_REGISTRY_H

#include "Cell.h"

class CellsRegistry {
  public:
    Cell****registryXYZ;
    Cell***registryXY0;

    CellsRegistry();
    void registerCell(Cell* c);
    void unregisterCell(Cell* c);
    bool existsXYZ(int x, int y, int z);
    bool existsXY0(int x, int y);
    ~CellsRegistry();
};

#endif
