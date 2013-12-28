#include "common.h"
#include "World.h"
#include "Clocks.h"
#include <algorithm>

World::World(int length, int maxCells, float reproductionRate, float mutationRate):cycle(0),length(length),maxCells(maxCells),reproductionRate(reproductionRate),mutationRate(mutationRate),minimumEnergyPerCell(.25){}

void World::infest(int nbSpecies, int nbCreaturesPerSpecies){
  for (int i=0; i<nbSpecies; i++){
    Species* s = new Species();
    for (int j=0; j<nbCreaturesPerSpecies; j++){
      reproduce(s);
    }
    species.push_back(s);
  }
}

Creature* World::reproduce(Species* s){
  int pos[2];
  int* ptr = registry.reserveRandomAvailableGroundPos(pos);
  if (ptr != NULL){
    Creature* c = s->reproduce( pos[0], pos[1] );
    return c;
  }else{
    return NULL;
  }
}

void World::lifecycle(){
  std::list<Species*>::iterator itSpecies = species.begin();
  std::list<Creature*>::iterator itCreature;
  std::vector<Cell*>::iterator itCell;

  // sunshine
  CLOCKS.start(CLOCK_SUNSHINE);
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    for (itCreature = s->creatures.begin(); itCreature != s->creatures.end(); ++itCreature) {
      Creature* c = (*itCreature);
      for (itCell = c->cells.begin(); itCell != c->cells.end(); ++itCell) {
        (*itCell)->energy = 0;
      }
    }
  }

  for (int i=0; i<world.length; ++i){
    for (int j=0; j<world.length; ++j){
      float energyFromSun = 1;
      for (int k=world.maxCells-1; k>=0; --k){
        Cell* c = world.registry.registryXYZ[i][j][k];
        if (c != NULL){
          c->energy = energyFromSun;
          energyFromSun/=2.;
        }
      }
    }
  }
  CLOCKS.pause(CLOCK_SUNSHINE);

  // death
  CLOCKS.start(CLOCK_DEATH);
  int deathCount = 0;
  itSpecies = species.begin();
  while (itSpecies != species.end()) {
    Species* s = (*itSpecies);
    deathCount += s->killOldAndWeakCreatures();

    if (s->creatures.size() == 0){
      if (DEBUG) cout << "Species goes extinct." <<endl;
      delete s;
      itSpecies = species.erase(itSpecies);
    }else{
      ++itSpecies;
    }
  }
  CLOCKS.pause(CLOCK_DEATH);
  if (DEBUG || OUT_SUMMARY) cout << deathCount << " creatures died." << endl;

  // reproduction
  CLOCKS.start(CLOCK_REPRODUCTION);
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
  CLOCKS.pause(CLOCK_REPRODUCTION);
  if (DEBUG || OUT_SUMMARY) cout << birthCount << " new creatures." << endl;


  // growth
  CLOCKS.start(CLOCK_GROWTH);
  vector<Creature*> creaturesCopy = collectCreaturesCopy();
  random_shuffle(std::begin(creaturesCopy), std::end(creaturesCopy));

  vector<Creature*>::iterator itCreatureV;
  for (itCreatureV = creaturesCopy.begin(); itCreatureV != creaturesCopy.end(); ++itCreatureV) {
    Creature* c = (*itCreatureV);
    c->grow();
  }
  CLOCKS.pause(CLOCK_GROWTH);
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

