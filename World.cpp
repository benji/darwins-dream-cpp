#include "common.h"
#include "World.h"
#include "Clocks.h"
#include <algorithm>
#include <thread>

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
  if (LOCAL_REPRODUCTION){
    int x,y;
    if (parent == NULL){
      x = randInt(length);
      y = randInt(length);
    }else{
      Cell* root = parent->cells[0];
      x = root->x;
      y = root->y;
    }
    ptr = registry.reserveRandomAvailableGroundPosAround(pos, x, y, LOCAL_REPRODUCTION_RADIUS);
  }else{
    ptr = registry.reserveRandomAvailableGroundPos(pos);
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

void sunshine(int minX, int maxX){ // [minX; maxX]
  for (int i=minX; i<=maxX; ++i){
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
}

void World::lifecycle(){
  CLOCKS.start(CLOCK_LIFECYCLE);
  if (DEBUG || OUT_SUMMARY) cout << "===== Cycle "<<world.cycle<<" ====="<<endl;

  std::list<Species*>::iterator itSpecies = species.begin();
  std::list<Creature*>::iterator itCreature;
  std::vector<Cell*>::iterator itCell;

  // sunshine
  CLOCKS.start(CLOCK_SUNSHINE);
  int nbThreads = NB_THREADS;
  thread** threads = new thread*[nbThreads];
  for (int i=0; i<nbThreads; i++){
    threads[i] = new thread(sunshine,(i*length)/nbThreads,((i+1)*length)/nbThreads - 1);
  }
  for (int i=0; i<nbThreads; i++){
    threads[i]->join();
    delete threads[i];
  }
  CLOCKS.pause(CLOCK_SUNSHINE);

  // death
  CLOCKS.start(CLOCK_DEATH);
  int deathCount = 0;
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    deathCount += s->killOldAndWeakCreatures();
  }
  CLOCKS.pause(CLOCK_DEATH);

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
    if (c->grow() == NULL && GROW_OR_DIE) {
      c->species.kill(c);
      ++deathCount;
    }
  }
  CLOCKS.pause(CLOCK_GROWTH);

  if (DEBUG || OUT_SUMMARY) cout << deathCount << " creatures died." << endl;

  // cleanup species with no creatures
  int nbExtinctSpecies = 0;
  itSpecies = species.begin();
  while (itSpecies != species.end()) {
    Species* s = (*itSpecies);
    if (s->creatures.size() == 0){
      if (DEBUG) cout << "Species goes extinct." <<endl;
      prepareSpeciesForDelete(s);
      delete s;
      itSpecies = species.erase(itSpecies);
      ++nbExtinctSpecies;
    }else{
      ++itSpecies;
    }
  }

  if (DEBUG || OUT_SUMMARY) cout << nbExtinctSpecies << " species go extinct." << endl;

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

void World::prepareSpeciesForDelete(Species* s){
  // rewiring species ancestors
  std::list<Species*>::iterator iter;
  for (iter = world.species.begin(); iter != world.species.end(); ++iter){
    Species* current = (*iter);
    if (current->ancestor == s){
      current->ancestor = s->ancestor;
      current->distanceToAncestor += s->distanceToAncestor;
    }
  }
}

void World::checkConsistency(){
  vector<int> takenIndexes;
  list<Species*>::iterator itSpecies;
  list<Creature*>::iterator itCreature;
  vector<Cell*>::iterator itCell;
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);

    for (itCreature = s->creatures.begin(); itCreature != s->creatures.end(); ++itCreature) {
      Creature* c = (*itCreature);
      
      for (itCell = c->cells.begin(); itCell != c->cells.end(); ++itCell) {
        Cell* cell = (*itCell);
        int index = world.length*world.length*cell->z + world.length*cell->y + cell->x;
        vector<int>::iterator it = find(takenIndexes.begin(), takenIndexes.end(), index);
        if (it == takenIndexes.end()){
          takenIndexes.push_back(index);
        } else {
          cout << "Duplicate cell found at " << cell->x << ", " << cell->y << ", " << cell->z << endl;
          exit(4);
        }
      }
    }
  }
  cout<<"World is OK: no duplicate found."<<endl;
}

World::~World(){
  cout << "Ending world." << endl;
  list<Species*>::iterator itSpecies;
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    delete (*itSpecies);
  }
}
