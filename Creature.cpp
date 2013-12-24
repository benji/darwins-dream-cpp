#include "Creature.h"

Creature::Creature(Species& species, int x, int y):species(species),x(x),y(y){//species(species),
  this->createCell(x,y,0);
}

void Creature::createCell(int x, int y, int z){
  Cell c(x,y,z);
  this->cells.push_back(c);
}

void Creature::grow(){
  // TODO: We take only the first proba here!
  //Cell cell = growNewCell(this.cells[this.cells.size()-1], this.species.dna[0].probas)
  //if (cell!=null) this.cells.push(cell)
}


void Creature::growNewCell(Cell c, float* growthProbas){
  bool canGrow[6];
  //return ;
}
