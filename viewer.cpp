#include <GL/gl.h>
#include <GL/glut.h>
#include "viewer.h"
#include "common.h"
#include "World.h"

static vector<Cube*>* worldCubes = new vector<Cube*>();
static vector<Cube*>* nextWorldCubes = NULL;
static vector<Cube*>* dominantSpeciesCubes = new vector<Cube*>();
static vector<Cube*>* nextDominantSpeciesCubes = NULL;

void Viewer::initialize(){
  Rendering::initialize(Viewer::drawWorld, Viewer::drawDominantSpecies);
}

void Viewer::cleanupCubes(vector<Cube*>* cubes){
  vector<Cube*>::iterator itCube;
  for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
    Cube* cube = (*itCube);
    delete cube;
  }
  cubes->clear();
  delete cubes;
}

void Viewer::drawCubes(vector<Cube*>* cubes){
  vector<Cube*>::iterator itCube;
  for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
    Cube* cube = (*itCube);
    Rendering::drawCube( cube->x, cube->y, cube->z, cube->r, cube->g, cube->b );
  }
}

void Viewer::drawDominantSpecies(){
  if (nextDominantSpeciesCubes != NULL){ //swap requested
    cleanupCubes(dominantSpeciesCubes);
    dominantSpeciesCubes = nextDominantSpeciesCubes;
    nextDominantSpeciesCubes = NULL;
  }
  drawCubes(dominantSpeciesCubes);
}

void Viewer::drawWorld(){
  if (nextWorldCubes != NULL){ //swap requested
    cleanupCubes(worldCubes);
    worldCubes = nextWorldCubes;
    nextWorldCubes = NULL;
  }
  drawCubes(worldCubes);
}

Cube* Viewer::createCube(int x, int y, int z, Species* s){
  Cube* c = new Cube();
  c->x = x;
  c->y = y;
  c->z = z;
  c->r = s->r;
  c->g = s->g;
  c->b = s->b;
  return c;
}

void Viewer::addSpeciesCubes(vector<Cube*>* destination, Species* s){
  list<Creature*>::iterator itCreature;
  vector<Cell*>::iterator itCell;

  for (itCreature = s->creatures.begin(); itCreature != s->creatures.end(); ++itCreature) {
    Creature* c = (*itCreature);
    for (itCell = c->cells.begin(); itCell != c->cells.end(); ++itCell) {
      Cell* cell = (*itCell);
      destination->push_back(createCube(cell->x, cell->y, cell->z, s));
    }
  }
}

void Viewer::updateWorldCubes(){
  if (nextWorldCubes != NULL) return;

  vector<Cube*>* tmpCubes = new vector<Cube*>();
  list<Species*>::iterator itSpecies;

  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    addSpeciesCubes(tmpCubes, s);
  }

  nextWorldCubes = tmpCubes;
}

void Viewer::updateDominantSpeciesCubes(bool showBiggestCreatureOrTypical){
  if (nextDominantSpeciesCubes != NULL) return;

  vector<Cube*>* tmpCubes = new vector<Cube*>();
  list<Species*>::iterator itSpecies;
  Species* dominantSpecies = NULL;

  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    if (dominantSpecies == NULL || s->creatures.size() > dominantSpecies->creatures.size()){
      dominantSpecies = s;
    }
  }

  if (dominantSpecies == NULL) {
    cout << "No dominant species found, is anybody alive?" << endl;
    return;
  }

  //Creature* c = dominantSpecies->creatures.front();
  //int balance = c->getBalance();
  //cout << "dominant species balance=" << balance <<endl;

  int x=0, y=0, z=0;
  tmpCubes->push_back(createCube(x, y, z, dominantSpecies));

  if (showBiggestCreatureOrTypical) { // show biggest creature
    cout<<"Showing biggest creature of dominant species."<<endl;

    list<Creature*>::iterator itCreature;
    vector<Cell*>::iterator itCell;
    unsigned int maxCellsCount = 0;
    Creature* biggest = NULL;

    for (itCreature = dominantSpecies->creatures.begin(); itCreature != dominantSpecies->creatures.end(); ++itCreature) {
      Creature* c = (*itCreature);
      if (c->cells.size() > maxCellsCount) {
        maxCellsCount = c->cells.size();
        biggest = c;
      }
    }
    for (itCell = biggest->cells.begin(); itCell != biggest->cells.end(); ++itCell) {
      Cell* cell = (*itCell);
      x = cell->x - biggest->x;
      y = cell->y - biggest->y;
      z = cell->z;
      
      tmpCubes->push_back(createCube(x, y, z, dominantSpecies));
    }
  } else { // show DNA creature
    cout<<"Showing Typical DNA creature."<<endl;

    vector<DNA*>::iterator itDNA;
    int directionIdx;

    for (itDNA = dominantSpecies->dna.begin(); itDNA != dominantSpecies->dna.end(); ++itDNA) {
      DNA* dna = (*itDNA);
      
      directionIdx = dna->growthDirection;

      if      (directionIdx==0) ++x;
      else if (directionIdx==1) --x;
      else if (directionIdx==2) ++y;
      else if (directionIdx==3) --y;
      else if (directionIdx==4) ++z;
      else if (directionIdx==5) --z;

      tmpCubes->push_back(createCube(x, y, z, dominantSpecies));
    }
  }

  nextDominantSpeciesCubes = tmpCubes;
}

// TODO : call this method! when is glutmainloop over?
void Viewer::cleanup(){
  delete worldCubes;
  if (nextWorldCubes != NULL) delete nextWorldCubes;
  delete dominantSpeciesCubes;
  if (nextDominantSpeciesCubes != NULL) delete nextDominantSpeciesCubes;
}
