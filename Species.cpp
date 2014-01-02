#include "common.h"
#include "World.h"
#include "Species.h"

static long idGen = 0;

Species::Species(Species* originalSpecies){
  id = idGen++;
  if (originalSpecies == NULL){
    this->dna.push_back(new DNA(NULL));
  }else{
    vector<DNA*>::iterator itDNA;
    for (itDNA = originalSpecies->dna.begin(); itDNA != originalSpecies->dna.end(); ++itDNA) {
      DNA* from = (*itDNA);
      this->dna.push_back(new DNA(from));
    }
    // mutation:
    int randIdx = randInt(dna.size());
    delete this->dna[randIdx];
    this->dna[randIdx] = new DNA(NULL);
  }
  // TODO HSL
  HSVtoRGB( &(this->r), &(this->g), &(this->b), randDouble(), 1, .65 );
  //setColor(randDouble(),randDouble(),randDouble());
}

DNA* Species::getDNA(int idx) {
  while (idx >= (int)dna.size()){
    dna.push_back(new DNA(NULL));
  }
  return dna[idx];
}

void Species::setColor(float r, float g, float b){
  this->r=r; this->g=g; this->b=b;
}

int Species::killOldAndWeakCreatures(){
  std::list<Creature*>::iterator itCreature = creatures.begin();

  int deathCount = 0;
  while (itCreature != creatures.end()){
    Creature* c = (*itCreature);
    if (world.cycle - c->creationCycle > world.maxCells || !c->hasEnoughEnergy()){
      if (DEBUG) cout << "Creature dies." <<endl;
      delete c;
      itCreature = creatures.erase(itCreature);
      ++deathCount;
    }else{
      ++itCreature;
    }
  }

  return deathCount;
}

Creature* Species::reproduce(int x, int y){
  Creature* c = new Creature( (*this), x, y );
  creatures.push_back(c);
  return c;
}

Species::~Species(){
  list<Creature*>::iterator itCreature;
  for (itCreature = creatures.begin(); itCreature != creatures.end(); ++itCreature) {
    delete (*itCreature);
  }

  vector<DNA*>::iterator itDNA;
  for (itDNA = dna.begin(); itDNA != dna.end(); ++itDNA) {
    delete (*itDNA);
  }
}

DNA::DNA(DNA* from){
  for (int i=0;i<6;i++) {
    this->probas[i] = (from!=NULL) ? from->probas[i] : randDouble();
  }
}
