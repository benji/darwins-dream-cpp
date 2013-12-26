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

int* World::findFreeGroundPos(int* pos){
  int tries = 0;
  while (tries<20){
    pos[0] = randInt(this->length);
    pos[1] = randInt(this->length);
    if (!world.registry.existsXYZ(pos[0],pos[1],0)){
      return pos;
    }
    ++tries;
  }
  return NULL;
}

void World::reproduce(Species* s){
  int pos[2];
  if (findFreeGroundPos(pos) != NULL){
    s->reproduce( pos[0], pos[1] );
  }
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
  vector<Species*> shuffledSpecies = collectShuffleSpecies();
  vector<Species*>::iterator itSpeciesV;
  for (itSpeciesV = shuffledSpecies.begin(); itSpeciesV != shuffledSpecies.end(); ++itSpeciesV) {
    Species* s = (*itSpeciesV);
    int nbCreatures = s->creatures.size();

    for (int i=0;i<nbCreatures;i++) {
      if (randDouble() < reproductionRate){
        reproduce(s);
      }
    }
  }


  // growth
  vector<Creature*> shuffledCreatures = collectShuffleCreatures();
  vector<Creature*>::iterator itCreatureV;
  for (itCreatureV = shuffledCreatures.begin(); itCreatureV != shuffledCreatures.end(); ++itCreatureV) {
    Creature* c = (*itCreatureV);
    c->grow();
  }

  cycle++;
}

vector<Creature*> World::collectShuffleCreatures(){
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
  
  random_shuffle(std::begin(collected), std::end(collected));

  return collected;
}

vector<Species*> World::collectShuffleSpecies(){
  vector<Species*> collected;

  list<Species*>::iterator itSpecies;

  for (itSpecies = species.begin(); itSpecies != species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    collected.push_back(s);
  }
  
  random_shuffle(std::begin(collected), std::end(collected));

  return collected;
}













