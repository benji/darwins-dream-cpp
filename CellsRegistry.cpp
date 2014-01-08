#include "CellsRegistry.h"

#include "Cell.h"
#include "World.h"
#include "Clocks.h"
#include <algorithm>

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
  if (registryXYZ[c->x][c->y][c->z] != NULL) {
    cout<<"ERROR: Position "<<c->x<<","<<c->y<<","<<c->z<<" has already been taken!";
    exit(8);
  }
  registryXYZ[c->x][c->y][c->z] = c;
}

int* CellsRegistry::reserveRandomAvailableGroundPos(int* pos){
  if (availableGroundTiles.size() == 0) {
    return NULL;
  }
  int idx = randInt(availableGroundTiles.size());
  indexToPos(pos, world.length, availableGroundTiles[idx]);
  availableGroundTiles.erase(availableGroundTiles.begin()+idx);
  return pos;
}

int* CellsRegistry::reserveRandomAvailableGroundPosAround(int* returnPos, int parentX, int parentY, int sqrLen){
  int xMin = max(0, parentX-sqrLen);
  int xMax = min(world.length-1, parentX+sqrLen);

  int yMin = max(0, parentY-sqrLen);
  int yMax = min(world.length-1, parentY+sqrLen);

  vector<int*> availablePos;

  for (int i=xMin; i<=xMax; ++i){
    for (int j=yMin; j<=yMax; ++j){
      if (registryXYZ[i][j][0] == NULL){
        availablePos.push_back(new int[2]{i,j}); // TODO cleanup
      }
    }
  }

  if (availablePos.size() == 0) return NULL;

  int i = randInt(availablePos.size());
  
  returnPos[0] = availablePos[i][0];
  returnPos[1] = availablePos[i][1];

  return returnPos;
}

void CellsRegistry::unregisterCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = NULL;
  if (!LOCAL_REPRODUCTION && c->z == 0){
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
