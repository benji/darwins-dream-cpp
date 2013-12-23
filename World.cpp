#include "World.h"
#include <iostream>
World::World(){}

void World::infest(int nbSpecies, int nbCreaturesPerSpecies){
  for (int i=0; i<nbSpecies; i++){
    Species s;
    for (int j=0; j<nbCreaturesPerSpecies; j++){
      Creature c;
      s.creatures.push_back(c);
    }
    species.push_back(s);
  }
}
