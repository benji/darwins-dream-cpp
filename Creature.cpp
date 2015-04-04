#include "common.h"
#include "Creature.h"
#include "World.h"
#include "Clocks.h"

Creature::Creature(Species& species, int x, int y):species(species),x(x),y(y){
  this->createCell(x,y,0);
  creationCycle = world.cycle;
}

Cell* Creature::createCell(int x, int y, int z){
  Cell* c = new Cell(this, x, y, z);
  this->cells.push_back(c);
  return c;
}

Cell* Creature::grow(){
  int idx = this->cells.size()-1;
  int dnaSize = species.dna.size();
  if (idx == dnaSize){ // fully grown
    return NULL;
  }
  DNA* dna = this->species.dna[idx];
  return growNewCell(this->cells.at(idx), dna->growthDirection);
}

Cell* Creature::growNewCell(Cell* c, int growthDirection){
  int x=c->x, y=c->y, z=c->z;

  if (z==0){
    z=1; // RULE_SEEDS_Z0
    if (world.registry.registryXYZ[x][y][z] != NULL){
      return NULL;
    }
    return createCell(x,y,z);
  }

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
    return createCell(x,y,z);
  }

  return NULL;
}

bool Creature::hasEnoughEnergy(){
  float totalEnergy = 0;
  vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    Cell* cell = (*itCell);
    totalEnergy += cell->energy;
  }
  return totalEnergy >= (cells.size()-1)*MIN_ENERGY_PER_CELL;
}

int Creature::getBalance(){
  int dx=0, dy=0;
  vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    Cell* cell = (*itCell);
    dx += cell->x - x;
    dy += cell->y - y;
  }
  return dx*dx+dy*dy;
}

bool Creature::isBalanced(){
  return getBalance() < BALANCING_THRESHOLD;
}

Creature::~Creature(){
  vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    delete (*itCell);
  }
}
