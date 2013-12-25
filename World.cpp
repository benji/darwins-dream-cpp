#include <iostream>
using namespace std;
#include "World.h"
#include "utils.h"

World::World(int length, float reproductionRate, float mutationRate):cycle(0),length(length),reproductionRate(reproductionRate),mutationRate(mutationRate){}

void World::infest(int nbSpecies, int nbCreaturesPerSpecies){
  for (int i=0; i<nbSpecies; i++){
    Species* s = new Species();
    s->setColor(randDouble(),randDouble(),randDouble());
    for (int j=0; j<nbCreaturesPerSpecies; j++){
      reproduce(s);
    }
    species.push_back(s);
  }
}

int* World::findFreeGroundPos(int* pos){
  pos[0] = randInt(this->length);
  pos[1] = randInt(this->length);
  return pos;
}

void World::reproduce(Species* s){
  int pos[2];
  findFreeGroundPos(pos);
  s->reproduce( pos[0], pos[1] );
}

void World::lifecycle(){
  std::list<Species*>::iterator itSpecies = species.begin();
  std::list<Creature*>::iterator itCreature;

  // death
  while (itSpecies != species.end()) {
    Species* s = (*itSpecies);
    s->killOldCreatures();

    if (s->creatures.size() == 0){
      delete s;
      itSpecies = species.erase(itSpecies);
    }else{
      ++itSpecies;
    }
  }

  // reproduction
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    int nbCreatures = s->creatures.size();

    for (int i=0;i<nbCreatures;i++) {
      if (randDouble() < reproductionRate && nbCreatures < 30){
        reproduce(s);
      }
    }
  }


  // growth
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    for (itCreature = s->creatures.begin(); itCreature != s->creatures.end(); ++itCreature) {
      Creature* c = (*itCreature);
      c->grow();
    }
  }

  cycle++;
}
