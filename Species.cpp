#include "common.h"
#include "World.h"
#include "Species.h"

Species::Species(){
  this->dna.push_back(new DNA());
  // TODO HSL
  //HSVtoRGB( &this->r, &this->g, &this->b, randDouble(), 1, .45 );
  setColor(randDouble(),randDouble(),randDouble());
}

void Species::setColor(float r, float g, float b){
  this->r=r; this->g=g; this->b=b;
}

int Species::killOldAndWeakCreatures(){
  std::list<Creature*>::iterator itCreature = creatures.begin();

  int deathCount = 0;
  while (itCreature != creatures.end()){
    Creature* c = (*itCreature);
    if (world.cycle - c->creationCycle > world.maxCells || !c->hasEnoughEnergy()){
      if (DEBUG) cout << "Creature dies." <<endl;
      c->die();
      delete c;
      itCreature = creatures.erase(itCreature);
      ++deathCount;
    }else{
      ++itCreature;
    }
  }

  return deathCount;
}

Creature* Species::reproduce(int x, int y){
  Creature* c = new Creature( (*this), x, y );
  creatures.push_back(c);
  return c;
}

DNA::DNA(){
  for (int i=0;i<6;i++) {
    this->probas[i]=randDouble();
  }
}
