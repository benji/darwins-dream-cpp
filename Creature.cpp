#include "common.h"
#include "Creature.h"
#include "World.h"
#include "Clocks.h"

Creature::Creature(Species& species, int x, int y):species(species),x(x),y(y){
  this->createCell(x,y,0,false);
  creationCycle = world.cycle;
}

void Creature::createCell(int x, int y, int z, bool registerCell){
  Cell* c = new Cell(x,y,z,registerCell);
  this->cells.push_back(c);
}

void Creature::grow(){
  int idx = this->cells.size()-1;

  // TODO: We take only the first proba here!
  float* probas = this->species.dna[0]->probas;
  growNewCell(this->cells.at(idx), probas);
}

void Creature::growNewCell(Cell* c, float* growthProbas){
  int x=c->x, y=c->y, z=c->z;

  if (z==0){
    z=1; // RULE_SEEDS_Z0
  } else {

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
    if      (canGrow[0] && p<(sumPrevProbas+=growthProbas[0])) x+=1;
    else if (canGrow[1] && p<(sumPrevProbas+=growthProbas[1])) x-=1;
    else if (canGrow[2] && p<(sumPrevProbas+=growthProbas[2])) y+=1;
    else if (canGrow[3] && p<(sumPrevProbas+=growthProbas[3])) y-=1;
    else if (canGrow[4] && p<(sumPrevProbas+=growthProbas[4])) z+=1;
    else if (canGrow[5])                                       z-=1;
    else {
      //cout << "Cell has no room to grow" << endl;
      return;
    }
  }
  createCell(x,y,z, true);
}

bool Creature::hasEnoughEnergy(){
  float totalEnergy = 0;
  std::vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    Cell* cell = (*itCell);
    totalEnergy += cell->energy;
  }
  return totalEnergy >= (cells.size()-1)*world.minimumEnergyPerCell;
}

void Creature::die(){
  std::vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    Cell* cell = (*itCell);
    delete cell;
  }
}
