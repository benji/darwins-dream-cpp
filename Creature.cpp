#include "Creature.h"

Creature::Creature(int x, int y):x(x),y(y){
  this->createCell(x,y,0);
}

void Creature::createCell(int x, int y, int z){
  Cell c(x,y,z);
  this->cells.push_back(c);
}
