#include "common.h"
#include "World.h"
#include "Clocks.h"
#include <algorithm>

World::World(int length, int maxCells, float reproductionRate, float mutationRate):cycle(0),length(length),maxCells(maxCells),reproductionRate(reproductionRate),mutationRate(mutationRate),minimumEnergyPerCell(.5){}

void World::infest(int nbSpecies, int nbCreaturesPerSpecies){
  for (int i=0; i<nbSpecies; i++){
    Species* s = createSpecies(NULL);
    for (int j=0; j<nbCreaturesPerSpecies; j++){
      reproduce(s, NULL);
    }
  }
}

Species* World::createSpecies(Species* originalSpecies){
  Species* s = new Species(originalSpecies);
  species.push_back(s);
  return s;
}

Creature* World::reproduce(Species* s, Creature* parent){
  int pos[2];
  int* ptr;
  if (true){//if (parent == NULL){
    ptr = registry.reserveRandomAvailableGroundPos(pos);
  }else{
    Cell* root = parent->cells[0];
    ptr = registry.reserveRandomAvailableGroundPosAround(pos, root->x, root->y, 5);
  }
  if (ptr != NULL){
    Creature* c = s->reproduce( pos[0], pos[1] );
    return c;
  }else{
    return NULL;
  }
}

Species* World::evolve(Species* s){
  //cout<<"NEW SPECIES!!!"<<endl;
  Species* newSpecies = createSpecies(s);
  reproduce(newSpecies, NULL);
  return newSpecies;
}

void World::lifecycle(){
  CLOCKS.start(CLOCK_LIFECYCLE);
  if (DEBUG || OUT_SUMMARY) cout << "===== Cycle "<<world.cycle<<" ====="<<endl;

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

  // reproduction & mutation
  CLOCKS.start(CLOCK_REPRODUCTION);
  vector<Creature*> creaturesCopy = collectCreaturesCopy();
  random_shuffle(std::begin(creaturesCopy), std::end(creaturesCopy));


  int birthCount = 0, newSpecies=0;
  vector<Creature*>::iterator itCreatureV;
  for (itCreatureV = creaturesCopy.begin(); itCreatureV != creaturesCopy.end(); ++itCreatureV) {
    Creature* c = (*itCreatureV);
    Species* s = &(c->species);

    if (randDouble() < mutationRate){
      if (evolve(s) != NULL) {
        ++newSpecies;
        ++birthCount;
      }
    }
    if (randDouble() < reproductionRate){
      if (reproduce(s, c) != NULL) ++birthCount;
    }
  }
  CLOCKS.pause(CLOCK_REPRODUCTION);
  if (DEBUG || OUT_SUMMARY) cout << birthCount << " new creatures." << endl;

  // growth
  CLOCKS.start(CLOCK_GROWTH);
  for (itCreatureV = creaturesCopy.begin(); itCreatureV != creaturesCopy.end(); ++itCreatureV) {
    Creature* c = (*itCreatureV);
    c->grow();
  }
  CLOCKS.pause(CLOCK_GROWTH);
  ++cycle;
  
  CLOCKS.pause(CLOCK_LIFECYCLE);
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

World::~World(){
  cout << "Ending world." << endl;
  list<Species*>::iterator itSpecies;
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    delete (*itSpecies);
  }
}
