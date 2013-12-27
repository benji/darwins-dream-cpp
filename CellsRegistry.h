#ifndef CELLS_REGISTRY_H
#define CELLS_REGISTRY_H

#include "Cell.h"

class CellsRegistry {
  public:
    Cell****registryXYZ;
    int nbAvailableGroundTiles;
    vector<int> availableGroundTiles;

    CellsRegistry();
    void registerCell(Cell* c);
    void unregisterCell(Cell* c);
    bool existsXYZ(int x, int y, int z);
    ~CellsRegistry();
    int* reserveRandomAvailableGroundPos(int* pos);
};

#endif
