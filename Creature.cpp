#include "Creature.h"

Creature::Creature(Species& species, int x, int y):species(species),x(x),y(y){//species(species),
  this->createCell(x,y,0);
}

void Creature::createCell(int x, int y, int z){
  Cell c(x,y,z);
  this->cells.push_back(c);
}

void Creature::grow(){
  
}
