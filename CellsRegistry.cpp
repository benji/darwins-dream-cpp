#include "CellsRegistry.h"

#include "Cell.h"
#include "World.h"

CellsRegistry::CellsRegistry(){
  registryXYZ = new Cell***[world.length];
  registryXY0 = new Cell**[world.length];

  for (int i = 0; i < world.length; ++i) {
    registryXYZ[i] = new Cell**[world.length];
    registryXY0[i] = new Cell*[world.length];

    for (int j = 0; j < world.length; ++j){
      registryXYZ[i][j] = new Cell*[world.maxCells];
      registryXY0[i][j] = NULL;

      for (int k = 0; k < world.maxCells; ++k){
        registryXYZ[i][j][k] = NULL;
      }
    }
  }
}

bool CellsRegistry::existsXYZ(int x, int y, int z){
  return registryXYZ[x][y][z] != NULL;
}

bool CellsRegistry::existsXY0(int x, int y){
  return registryXY0[x][y] != NULL;
}

void CellsRegistry::registerCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = c;
  registryXY0[c->x][c->y] = c;
}

void CellsRegistry::unregisterCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = NULL;
  registryXY0[c->x][c->y] = NULL;
}

CellsRegistry::~CellsRegistry(){
  for (int i = 0; i < world.length; ++i) {
    for (int j = 0; j < world.length; ++j){
      delete [] registryXYZ[i][j];
    }
    delete [] registryXYZ[i];
    delete [] registryXY0[i];
  }
  delete [] registryXYZ;
  delete [] registryXY0;

}
