#include <iostream>
#include "Creature.h"
#include "utils.h"
using namespace std;

Creature::Creature(Species& species, int x, int y):species(species),x(x),y(y){
  this->createCell(x,y,0);
}

void Creature::createCell(int x, int y, int z){
  Cell* c = new Cell(x,y,z);
  this->cells.push_back(c);
}

void Creature::grow(){
  int idx = this->cells.size()-1;
  if (idx < 0){
    cerr<<"Negative index"<<endl;
    exit(-2);
  }

  // TODO: We take only the first proba here!
  float* probas = this->species.dna[0]->probas;
  growNewCell(this->cells.at(idx), probas);
}

void Creature::growNewCell(Cell* c, float* growthProbas){
  bool canGrow[6] = {
    c->x>0,
    true,
    c->y>0,
    true,
    c->z>0,
    true
  };

  float sumProbasCanGrow = 0;
  for (int i=0;i<6;i++){
    sumProbasCanGrow += canGrow[i]?growthProbas[i]:0;
  }

  float p = randDouble()*sumProbasCanGrow;

  int x=c->x, y=c->y, z=c->z;

  float sumPrevProbas = 0;
  if      (canGrow[0] && p<(sumPrevProbas+=growthProbas[0])) x+=1;
  else if (canGrow[1] && p<(sumPrevProbas+=growthProbas[1])) x-=1;
  else if (canGrow[2] && p<(sumPrevProbas+=growthProbas[2])) y+=1;
  else if (canGrow[3] && p<(sumPrevProbas+=growthProbas[3])) y-=1;
  else if (canGrow[4] && p<(sumPrevProbas+=growthProbas[4])) z+=1;
  else if (canGrow[5])                                       z-=1;
  else {
    cout << "ERROR: Cell has no room to grow" << endl;
    return;
  }

  createCell(x,y,z);
}

void Creature::die(){
  std::vector<Cell*>::iterator itCell;
  for (itCell = cells.begin(); itCell != cells.end(); ++itCell) {
    Cell* cell = (*itCell);
    delete cell;
  }
}
