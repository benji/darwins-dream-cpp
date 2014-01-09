#ifndef CELLS_REGISTRY_H
#define CELLS_REGISTRY_H

#include "Cell.h"

class CellsRegistry {
  public:
    // updated only by register() and unregister()
    Cell**** registryXYZ;

    // used only for non-local reproduction
    int nbAvailableGroundTiles;
    vector<int> availableGroundTiles;

    CellsRegistry();
    void registerCell(Cell* c);
    void unregisterCell(Cell* c);
    bool existsXYZ(int x, int y, int z);
    int* reserveRandomAvailableGroundPos(int* pos);
    int* reserveRandomAvailableGroundPosAround(int* returnPos, int parentX, int parentY, int sqrLen);
    ~CellsRegistry();
};

#endif
