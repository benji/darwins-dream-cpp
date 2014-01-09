#include "common.h"
#include "World.h"
#include "Clocks.h"

World::World(int length, int maxCells, float reproductionRate, float mutationRate):
  cycle(0),length(length),maxCells(maxCells){}

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
  speciesCollectionMutex.lock();
  species.push_back(s);
  speciesCollectionMutex.unlock();
  return s;
}

Creature* World::reproduce(Species* s, Creature* parent){
  int pos[2];
  int* ptr;
  if (LOCALITY_ENABLED){
    int x,y;
    if (parent == NULL){
      x = randInt(length);
      y = randInt(length);
    }else{
      Cell* root = parent->cells[0];
      x = root->x;
      y = root->y;
    }
    ptr = registry.reserveRandomAvailableGroundPosAround(pos, x, y, LOCALITY_RADIUS);
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

Species* World::evolve(Species* s, Creature* c){
  Species* newSpecies = createSpecies(s);
  reproduce(newSpecies, c);
  //cout<<"Reproduced evolved species from "<<c->x<<","<<c->y<<" to "<<(*(newSpecies->creatures.begin()))->x<<","<<(*(newSpecies->creatures.begin()))->y<<endl;
  return newSpecies;
}

void World::sunshineOnSlice(int minX, int maxX){ // [minX; maxX]
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

void World::reproductionMutationOnCreature(Creature* c){
  Species* s = &(c->species);

  if (randDouble() < MUTATION_RATE){
    if (evolve(s, c) != NULL) {
      ++cycleNewSpeciesCount;
      ++cycleBirthCount;
    }
  }
  if (randDouble() < REPRODUCTION_RATE){
    if (reproduce(s, c) != NULL) ++cycleBirthCount;
  }
}

void World::reproductionMutationOnSlice(int minX, int maxX){ // [minX; maxX]
  vector<Creature*> creatures;
  for (int i=minX; i<=maxX; ++i){
    for (int j=0; j<world.length; ++j){
      Cell* c = registry.registryXYZ[i][j][0];
      if (c != NULL){
        creatures.push_back(c->creature);
      }
    }
  }
  
  random_shuffle(std::begin(creatures), std::end(creatures));

  vector<Creature*>::iterator itCreature;
  for (itCreature = creatures.begin(); itCreature != creatures.end(); ++itCreature) {
    Creature* c = (*itCreature);
    reproductionMutationOnCreature(c);
  }
}

// Threads need to treat distant areas on the map to avoid thread concurrency problems (registry)
// Slices are subdivisions of the X axis into NB_THREADS*2 parts:
// 1 | 2 | 3 | ... | n
// odd:  1 3 ... n-1
// even: 2 4 ... n
void World::applyLocalReproductionMutationOnSlices(int odd){
  int offset = odd?1:0;
  int nbThreads = NB_THREADS;
  thread** threads = new thread*[nbThreads];
  for (int i=0; i<nbThreads; i++){
    //cout<<(((2*i+offset  )*length)  /(2*nbThreads))<<" to "<<(((2*i+offset+1)*length)  /(2*nbThreads) - 1)<<endl;
    threads[i] = new thread(
      &World::reproductionMutationOnSlice, 
      this, 
      ((2*i+offset  )*length)  /(2*nbThreads),
      ((2*i+offset+1)*length)  /(2*nbThreads) - 1
    );
  }
  for (int i=0; i<nbThreads; i++){
    threads[i]->join();
    delete threads[i];
  }
  delete [] threads;
}

void World::lifecycle(){
  CLOCKS.start(CLOCK_LIFECYCLE);
  if (DEBUG || OUT_SUMMARY) cout << "===== Cycle " << cycle << " =====" << endl;

  std::list<Species*>::iterator itSpecies;
  std::list<Creature*>::iterator itCreature;
  std::vector<Cell*>::iterator itCell;
  cycleBirthCount = 0;
  cycleDeathCount = 0;
  cycleNewSpeciesCount = 0;

  // sunshine
  CLOCKS.start(CLOCK_SUNSHINE);
  int nbThreads = NB_THREADS;
  thread** threads = new thread*[nbThreads];
  for (int i=0; i<nbThreads; i++){
    threads[i] = new thread(
      &World::sunshineOnSlice, this,
      (i*length)/nbThreads, ((i+1)*length)/nbThreads - 1
    );
  }
  for (int i=0; i<nbThreads; i++){
    threads[i]->join();
    delete threads[i];
  }
  delete [] threads;
  CLOCKS.pause(CLOCK_SUNSHINE);

  // death
  CLOCKS.start(CLOCK_DEATH);
  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    cycleDeathCount += s->killOldAndWeakCreatures();
  }
  CLOCKS.pause(CLOCK_DEATH);

  // reproduction & mutation
  CLOCKS.start(CLOCK_REPRODUCTION);
  vector<Creature*>::iterator itCreatureV;
  vector<Creature*> creaturesCopy = collectCreaturesCopy();
  random_shuffle(std::begin(creaturesCopy), std::end(creaturesCopy));

  if (LOCALITY_ENABLED){
    bool b = cycle % 2 == 0;
    applyLocalReproductionMutationOnSlices(b);
    applyLocalReproductionMutationOnSlices(!b);
  } else {

    for (itCreatureV = creaturesCopy.begin(); itCreatureV != creaturesCopy.end(); ++itCreatureV) {
      Creature* c = (*itCreatureV);
      reproductionMutationOnCreature(c);
    }
  }
  if (DEBUG || OUT_SUMMARY) {
    cout << cycleBirthCount << " new creatures." << endl;
    cout << cycleNewSpeciesCount << " new species." << endl;
  }
  CLOCKS.pause(CLOCK_REPRODUCTION);

  // growth
  CLOCKS.start(CLOCK_GROWTH);
  for (itCreatureV = creaturesCopy.begin(); itCreatureV != creaturesCopy.end(); ++itCreatureV) {
    Creature* c = (*itCreatureV);
    if (c->grow() == NULL && GROW_OR_DIE) {
      c->species.kill(c);
      ++cycleDeathCount;
    }
  }
  CLOCKS.pause(CLOCK_GROWTH);

  if (DEBUG || OUT_SUMMARY) cout << cycleDeathCount << " creatures died." << endl;

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

// Makes sure that 2 cells have not been assigned the same position (x, y, z)
// For debug purposes only as it should never happen.
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
