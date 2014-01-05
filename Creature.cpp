#include "common.h"
#include "Creature.h"
#include "World.h"
#include "Clocks.h"

Creature::Creature(Species& species, int x, int y):species(species),x(x),y(y){
  this->createCell(x,y,0,false);
  creationCycle = world.cycle;
}

Cell* Creature::createCell(int x, int y, int z, bool registerCell){
  Cell* c = new Cell(x,y,z,registerCell);
  this->cells.push_back(c);
  return c;
}

Cell* Creature::grow(){
  int idx = this->cells.size()-1;
  DNA* dna = this->species.dna[idx];
  return growNewCell(this->cells.at(idx), dna->probas, dna->growthDirection);
}

Cell* Creature::growNewCell(Cell* c, float* growthProbas, int growthDirection){
  int x=c->x, y=c->y, z=c->z;

  if (z==0){
    z=1; // RULE_SEEDS_Z0
    if (world.registry.registryXYZ[x][y][z+1] != NULL){
      //cout << "Second cell is blocked" << endl;
      return NULL;
    }
    return createCell(x,y,z, true);
  }

  if (VARIABLE_GROWTH){

    // tries to find a path

    bool canGrow[6] = {
      x < world.length-1 && world.registry.registryXYZ[x+1][y][z] == NULL, // x+1
      x > 0              && world.registry.registryXYZ[x-1][y][z] == NULL, // x-1
      y < world.length-1 && world.registry.registryXYZ[x][y+1][z] == NULL, // y+1
      y > 0              && world.registry.registryXYZ[x][y-1][z] == NULL, // y-1
      true               && world.registry.registryXYZ[x][y][z+1] == NULL, // z+1
      z > 1              && world.registry.registryXYZ[x][y][z-1] == NULL  // z-1
    };

    float sumProbasCanGrow = 0;
    for (int i=0;i<6;i++){
      if (canGrow[i]) {
        sumProbasCanGrow += growthProbas[i];
      }
    }

    float p = randDouble()*sumProbasCanGrow;

    float sumPrevProbas = 0;
    if      (canGrow[0] && p<(sumPrevProbas+=growthProbas[0])) ++x;
    else if (canGrow[1] && p<(sumPrevProbas+=growthProbas[1])) --x;
    else if (canGrow[2] && p<(sumPrevProbas+=growthProbas[2])) ++y;
    else if (canGrow[3] && p<(sumPrevProbas+=growthProbas[3])) --y;
    else if (canGrow[4] && p<(sumPrevProbas+=growthProbas[4])) ++z;
    else if (canGrow[5])                                       --z;
    else {
      //cout << "Cell has no room to grow" << endl;
      return NULL;
    }
    return createCell(x,y,z, true);

  } else {

    // static growth

    if (growthDirection == 0) {
      ++x;
      if (x > world.length-1) return NULL;
    } else if (growthDirection == 1) {
      --x;
      if (x < 0) return NULL;
    } else if (growthDirection == 2) {
      ++y;
      if (y > world.length-1) return NULL;
    } else if (growthDirection == 3) {
      --y;
      if (y < 0) return NULL;
    } else if (growthDirection == 4) {
      ++z;
    } else if (growthDirection == 5) {
      --z;
      if (z < 1) return NULL;
    } else {
      cout << "Invalid growth direction: "<<growthDirection<<endl;
      exit(2);
    }

    if (world.registry.registryXYZ[x][y][z] == NULL) {
      return createCell(x,y,z, true);
    }
    return NULL;
  }
}

bool Creature::hasEnoughEnergy(){
  float totalEnergy = 0;
  vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    Cell* cell = (*itCell);
    totalEnergy += cell->energy;
  }
  return totalEnergy >= (cells.size()-1)*world.minimumEnergyPerCell;
}

Creature::~Creature(){
  vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    delete (*itCell);
  }
}
