#include "Species.h"
#include "utils.h"
#include <iostream>

Species::Species(){
  this->dna.push_back(DNA());
  cout << "new species!" << endl;
}

void Species::setColor(float r, float g, float b){
  this->r=r; this->g=g; this->b=b;
}

void Species::killOldCreatures(){
  std::list<Creature>::iterator itCreature = creatures.begin();

  while (itCreature != creatures.end()){
    if (itCreature->cells.size() > 30){ //TODO remove hardcoded value
      itCreature->die();
      itCreature = creatures.erase(itCreature);
    }else{
      ++itCreature;
    }
  }
}

DNA::DNA(){
  for (int i=0;i<6;i++) {
    this->probas[i]=randDouble();
  }
}
