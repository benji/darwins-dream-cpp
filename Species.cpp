#include "Species.h"
#include "utils.h"
#include <iostream>

Species::Species(){
  this->dna.push_back(new DNA());
  cout << "new species!" << endl;
}

void Species::setColor(float r, float g, float b){
  this->r=r; this->g=g; this->b=b;
}

void Species::killOldCreatures(){
  std::list<Creature*>::iterator itCreature = creatures.begin();

  while (itCreature != creatures.end()){
    Creature* c = (*itCreature);
    if (c->cells.size() > 30){ //TODO remove hardcoded value
      cout<<"DEAD"<<endl;
      c->die();
      itCreature = creatures.erase(itCreature);
    }else{
      ++itCreature;
    }
  }
}

void Species::reproduce(int x, int y){
  cout<<"reproduction!"<<endl;
  Creature* c = new Creature( (*this), x, y );
  creatures.push_back(c);
}

DNA::DNA(){
  for (int i=0;i<6;i++) {
    this->probas[i]=randDouble();
  }
}
