#include "common.h"
#include "World.h"
#include "Species.h"
#include <algorithm>

static long idGen = 0;

Species::Species(Species* originalSpecies){
  id = idGen++;

  if (originalSpecies == NULL){
    // completely new species
    while (!generateDna()) ;
  }else{
    // evolution of an existing species
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

  HSVtoRGB( &(this->r), &(this->g), &(this->b), randDouble(), 1, .65 );
}

// Don't allow species with impossible shape (z<1 or collision with self)
bool Species::generateDna(){
  clearDna();

  DNA* d = new DNA(NULL);
  d->growthDirection = 4; // first cell has to go up
  this->dna.push_back(d);

  int pos[MAX_CELLS][3];
  pos[0][0] = pos[0][1] = pos[0][2] = 0;
  pos[1][0] = pos[1][1] = 0;
  pos[1][2] = 1;

  vector<int> takenDirs;
  for (int i=2; i<world.maxCells; ++i){
    int x = pos[i-1][0], y = pos[i-1][1], z = pos[i-1][2];

    takenDirs.clear();

    for (int j=0; j<i-1; ++j){
      int prevX = pos[j][0], prevY = pos[j][1], prevZ = pos[j][2];
      if (prevX == x+1 && prevY == y   && prevZ == z)   takenDirs.push_back(0);
      if (prevX == x-1 && prevY == y   && prevZ == z)   takenDirs.push_back(1);
      if (prevX == x   && prevY == y+1 && prevZ == z)   takenDirs.push_back(2);
      if (prevX == x   && prevY == y-1 && prevZ == z)   takenDirs.push_back(3);
      if (prevX == x   && prevY == y   && prevZ == z+1) takenDirs.push_back(4);
      if (prevX == x   && prevY == y   && prevZ == z-1) takenDirs.push_back(5);
    }

    // all positions around are already taken:
    if (takenDirs.size() == 6) return false;

    bool canGoDown = find(takenDirs.begin(), takenDirs.end(), 5) == takenDirs.end();

    // the only position available is downwards but Z=0 is disallowed:
    if (canGoDown && takenDirs.size() == 5 && z<=1) return false;

    // don't allow Z=0
    if (canGoDown && z == 1) takenDirs.push_back(5);

    int direction = randInt(6 - takenDirs.size());
    for (unsigned int k=0; k<takenDirs.size(); ++k){
      if (direction >= takenDirs[k]){
        ++direction;
      }
    }
    DNA* d = new DNA(NULL);
    d->growthDirection = direction;
    this->dna.push_back(d);

    pos[i][0] = x;
    pos[i][1] = y;
    pos[i][2] = z;

    if      (direction == 0) pos[i][0] = x+1;
    else if (direction == 1) pos[i][0] = x-1;
    else if (direction == 2) pos[i][1] = y+1;
    else if (direction == 3) pos[i][1] = y-1;
    else if (direction == 4) pos[i][2] = z+1;
    else if (direction == 5) pos[i][2] = z-1;
  }

  return true;
}

void Species::setColor(float r, float g, float b){
  this->r=r; this->g=g; this->b=b;
}

void Species::kill(Creature* c){
  list<Creature*>::iterator it = find(creatures.begin(), creatures.end(), c);
  if (it == creatures.end()){
    cout<<"Cannot kill creature: not found!"<<endl;
  }else{
    delete *it;
    creatures.erase(it);
  }
}

int Species::killOldAndWeakCreatures(){
  std::list<Creature*>::iterator itCreature = creatures.begin();

  int deathCount = 0;
  while (itCreature != creatures.end()){
    Creature* c = (*itCreature);
    if (c->cells.size() >= (unsigned int)world.maxCells 
        || world.cycle - c->creationCycle >= world.maxCells 
        || !c->hasEnoughEnergy()){
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

void Species::clearDna(){
  vector<DNA*>::iterator itDNA;
  for (itDNA = dna.begin(); itDNA != dna.end(); ++itDNA) {
    delete (*itDNA);
  }
  this->dna.clear();
}

Species::~Species(){
  list<Creature*>::iterator itCreature;
  for (itCreature = creatures.begin(); itCreature != creatures.end(); ++itCreature) {
    delete (*itCreature);
  }

  clearDna();
}

DNA::DNA(DNA* from){
  for (int i=0;i<6;i++) {
    this->probas[i] = (from!=NULL) ? from->probas[i] : randDouble();
  }
  growthDirection = from!=NULL ? from->growthDirection:randInt(6);
}
