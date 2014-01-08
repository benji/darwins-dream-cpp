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
  // TODO: dont instanciate this array every time, it's costly, we should create it once only for each thread
  // make sure it's thread safe.
  int tmpArrLen = LOCALITY_RADIUS*2 + 1;
  int availablePosTmp[tmpArrLen*tmpArrLen][2];

  int xMin = max(0, parentX-sqrLen);
  int xMax = min(world.length-1, parentX+sqrLen);

  int yMin = max(0, parentY-sqrLen);
  int yMax = min(world.length-1, parentY+sqrLen);

  int nbAvailablePos = 0;
  for (int i=xMin; i<=xMax; ++i){
    for (int j=yMin; j<=yMax; ++j){
      if (registryXYZ[i][j][0] == NULL){
        availablePosTmp[nbAvailablePos][0] = i;
        availablePosTmp[nbAvailablePos][1] = j;
        ++nbAvailablePos;
      }
    }
  }

  if (nbAvailablePos == 0) return NULL;

  int i = randInt(nbAvailablePos);
  
  returnPos[0] = availablePosTmp[i][0];
  returnPos[1] = availablePosTmp[i][1];

  return returnPos;
}

void CellsRegistry::unregisterCell(Cell* c){
  registryXYZ[c->x][c->y][c->z] = NULL;
  if (!LOCALITY_ENABLED && c->z == 0){
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
