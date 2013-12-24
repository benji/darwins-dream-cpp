#include <iostream>
using namespace std;
#include "World.h"
#include "utils.h"

World::World(int length):length(length){}

void World::infest(int nbSpecies, int nbCreaturesPerSpecies){
  for (int i=0; i<nbSpecies; i++){
    Species s;
    s.setColor(randDouble(),randDouble(),randDouble());
    for (int j=0; j<nbCreaturesPerSpecies; j++){
      int pos[2];
      findFreeGroundPos(pos);
      Creature c( s, pos[0], pos[1] );
      // TODO: do I need to free pos?
      s.creatures.push_back(c);
    }
    species.push_back(s);
  }
}

int* World::findFreeGroundPos(int* pos){
  pos[0] = randInt(this->length);
  pos[1] = randInt(this->length);
  return pos;
}

void World::lifecycle(){
  std::list<Species>::iterator itSpecies;
  std::list<Creature>::iterator itCreature;

  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    for (itCreature = itSpecies->creatures.begin(); itCreature != itSpecies->creatures.end(); ++itCreature) {
      Creature c = *itCreature;
      itCreature->grow();
    }
  }
}
