#include <iostream>
using namespace std;
#include "World.h"
#include "utils.h"

World::World(int length):length(length){}

void World::infest(int nbSpecies, int nbCreaturesPerSpecies){
  for (int i=0; i<nbSpecies; i++){
    Species s;
    for (int j=0; j<nbCreaturesPerSpecies; j++){
      int* pos = findFreeGroundPos();
      Creature c( pos[0], pos[1] );
      s.creatures.push_back(c);
    }
    species.push_back(s);
  }
}

int* World::findFreeGroundPos(){
  int pos[2] = {randInt(this->length), randInt(this->length)};
  return pos;
}

void World::lifecycle(){
  cout << "lifecycle" << endl;
}
