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
  registryXYZ[c->x][c->y][c->z] = c;
  if (c->z == 0){
    cerr << "Disallowed cell with Z=0" << endl;
    throw 1;
  }
}

int* CellsRegistry::reserveRandomAvailableGroundPos(int* pos){
  if (availableGroundTiles.size() == 0) {
    return NULL;
  }
  int idx = randInt(availableGroundTiles.size());
  indexToPos(pos, world.length, availableGroundTiles[idx]);
  availableGroundTiles.erase(availableGroundTiles.begin()+idx);
//  registryXYZ[pos[0]][pos[1]][0] = 
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
      int idx = posToIndex(world.length, i, j);
      vector<int>::iterator it = find(availableGroundTiles.begin(), availableGroundTiles.end(), idx);
      if (it != availableGroundTiles.end()){
        availablePos.push_back(new int[2]{i,j});
      }
    }
  }

  int i = randInt(availablePos.size());
  
  returnPos[0] = availablePos[i][0];
  returnPos[1] = availablePos[i][1];

  int idx = posToIndex(world.length, returnPos[0], returnPos[1]);

  vector<int>::iterator it = find(availableGroundTiles.begin(), availableGroundTiles.end(), idx);
  availableGroundTiles.erase(it);
  
  return returnPos;
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
