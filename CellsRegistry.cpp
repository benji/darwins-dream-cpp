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

  nbAvailableGroundTiles = world.length*world.length;
}

bool CellsRegistry::existsXYZ(int x, int y, int z){
  return registryXYZ[x][y][z] != NULL;
}

bool CellsRegistry::existsXY0(int x, int y){
  return registryXY0[x][y] != NULL;
}

void CellsRegistry::registerCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = c;
  if (c->z == 0){
    registryXY0[c->x][c->y] = c;
    nbAvailableGroundTiles--;
  }
}

int* CellsRegistry::findRandomAvailableGroundPos(int* pos){
  if (nbAvailableGroundTiles == 0) {
    return NULL;
  }
  if (nbAvailableGroundTiles < 0) throw "More cells than tiles!";

  int idx = randInt(nbAvailableGroundTiles);  

  int tmp[2] = {0,0};
  int i=0;
  while (i <= idx){
    indexToPos(tmp, world.length, i);
    if (registryXY0[tmp[0]][tmp[1]] != NULL){
      idx++;
    }
    i++;
  }

  indexToPos(pos, world.length, idx);
  return pos;
}

void CellsRegistry::unregisterCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = NULL;
  if (c->z == 0){
    registryXY0[c->x][c->y] = NULL;
    nbAvailableGroundTiles++;
  }
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
