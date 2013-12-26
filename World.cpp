#include "common.h"
#include "World.h"
#include <algorithm>

World::World(int length, int maxCells, float reproductionRate, float mutationRate):cycle(0),length(length),maxCells(maxCells),reproductionRate(reproductionRate),mutationRate(mutationRate){}

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

Creature* World::reproduce(Species* s){
  int pos[2];
  if (registry.findRandomAvailableGroundPos(pos) != NULL){
    return s->reproduce( pos[0], pos[1] );
  }else{
    return NULL;
  }
}

void World::lifecycle(){
  std::list<Species*>::iterator itSpecies = species.begin();
  std::list<Creature*>::iterator itCreature;

  // death
  int deathCount = 0;
  while (itSpecies != species.end()) {
    Species* s = (*itSpecies);
    deathCount += s->killOldCreatures();

    if (s->creatures.size() == 0){
      if (DEBUG) cout << "Species goes extinct." <<endl;
      delete s;
      itSpecies = species.erase(itSpecies);
    }else{
      ++itSpecies;
    }
  }
  if (DEBUG || OUT_SUMMARY) cout << deathCount << " creatures died." << endl;

  // reproduction
  vector<Species*> speciesCopy = collectSpeciesCopy();
  random_shuffle(std::begin(speciesCopy), std::end(speciesCopy));

  int birthCount = 0;
  vector<Species*>::iterator itSpeciesV;
  for (itSpeciesV = speciesCopy.begin(); itSpeciesV != speciesCopy.end(); ++itSpeciesV) {
    Species* s = (*itSpeciesV);
    int nbCreatures = s->creatures.size();

    for (int i=0;i<nbCreatures;i++) {
      if (randDouble() < reproductionRate){
        if (reproduce(s) != NULL) ++birthCount;
      }
    }
  }
  if (DEBUG || OUT_SUMMARY) cout << birthCount << " new creatures." << endl;


  // growth
  vector<Creature*> creaturesCopy = collectCreaturesCopy();
  random_shuffle(std::begin(creaturesCopy), std::end(creaturesCopy));

  vector<Creature*>::iterator itCreatureV;
  for (itCreatureV = creaturesCopy.begin(); itCreatureV != creaturesCopy.end(); ++itCreatureV) {
    Creature* c = (*itCreatureV);
    c->grow();
  }

  cycle++;
}

vector<Creature*> World::collectCreaturesCopy(){
  vector<Creature*> collected;
  list<Species*>::iterator itSpecies;
  list<Creature*>::iterator itCreature;

  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    for (itCreature = s->creatures.begin(); itCreature != s->creatures.end(); ++itCreature) {
      Creature* c = (*itCreature);
      collected.push_back(c);
    }
  }

  return collected;
}

vector<Species*> World::collectSpeciesCopy(){
  vector<Species*> collected;
  list<Species*>::iterator itSpecies;

  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    collected.push_back(s);
  }

  return collected;
}













