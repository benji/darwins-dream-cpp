#include "CellsRegistry.h"

#include "Cell.h"
#include "World.h"
#include "Clocks.h"

CellsRegistry::CellsRegistry(){
  registryXYZ = new Cell***[world.length];
  for (int i = 0; i < world.length; ++i) {
    registryXYZ[i] = new Cell**[world.length];


    for (int j = 0; j < world.length; ++j){
      registryXYZ[i][j] = new Cell*[world.maxCells];
      availableGroundTiles.push_back(i+j*world.length);

      for (int k = 0; k < world.maxCells; ++k){
        registryXYZ[i][j][k] = NULL;
      }
    }
  }

  nbAvailableGroundTiles = world.length*world.length;
}

bool CellsRegistry::existsXYZ(int x, int y, int z){
  return registryXYZ[x][y][z] != NULL;
}

void CellsRegistry::registerCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = c;
  if (c->z == 0){
    cerr << "Disallowed cell with Z=0" << endl;
    throw 1;
  }
}

int* CellsRegistry::reserveRandomAvailableGroundPos(int* pos){
  CLOCKS.start(20);
  if (availableGroundTiles.size() == 0) {
    return NULL;
  }

  int idx = randInt(availableGroundTiles.size());
  CLOCKS.pause(20);

  CLOCKS.start(21);  
  indexToPos(pos, world.length, availableGroundTiles[idx]);
  CLOCKS.pause(21);

  CLOCKS.start(22);  
  availableGroundTiles.erase(availableGroundTiles.begin()+idx);
  CLOCKS.pause(22);

  return pos;
}

void CellsRegistry::unregisterCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = NULL;
  if (c->z == 0){
    availableGroundTiles.push_back(c->x + c->y * world.length);
  }
}

CellsRegistry::~CellsRegistry(){
  for (int i = 0; i < world.length; ++i) {
    for (int j = 0; j < world.length; ++j){
      delete [] registryXYZ[i][j];
    }
    delete [] registryXYZ[i];
  }
  delete [] registryXYZ;

}
