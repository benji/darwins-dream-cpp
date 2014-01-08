#include <iostream>
#include <stdio.h>
#include <thread>
#include <GL/glut.h>
#include <algorithm>

#include "rendering.h"
#include "World.h"
#include "Species.h"
#include "Creature.h"
#include "Cell.h"
#include "Clocks.h"
#include "utils.h"
#include "common.h"

using namespace std;

struct Cube {
  public:
    int x,y,z;
    float r,g,b;
};

World world(WORLD_LENGTH, MAX_CELLS, 0.1, 0.01);

bool running = false;
thread* playLifeThread;
bool fullscreen = false;

long UPDATE_UI_EVERY_CYCLES = 100;
long lastRenderingCycle = -1;

vector<Cube*>* worldCubes = new vector<Cube*>();
vector<Cube*>* nextWorldCubes = NULL;
vector<Cube*>* dominantSpeciesCubes = new vector<Cube*>();
vector<Cube*>* nextDominantSpeciesCubes = NULL;

bool DEBUG = false;
bool OUT_SUMMARY = false;

Clocks CLOCKS;
int CLOCK_LIFECYCLE = 10;
int CLOCK_DEATH = 0;
int CLOCK_REPRODUCTION = 1;
int CLOCK_GROWTH = 2;
int CLOCK_SUNSHINE = 4;

void cleanupCubes(vector<Cube*>* cubes){
  vector<Cube*>::iterator itCube;
  for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
    Cube* cube = (*itCube);
    delete cube;
  }
  cubes->clear();
  delete cubes;
}

void drawCubes(vector<Cube*>* cubes){
  vector<Cube*>::iterator itCube;
  for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
    Cube* cube = (*itCube);
    Rendering::drawCube( cube->x, cube->y, cube->z, cube->r, cube->g, cube->b );
  }
}

void drawDominantSpecies(){
  if (nextDominantSpeciesCubes != NULL){ //swap requested
    cleanupCubes(dominantSpeciesCubes);
    dominantSpeciesCubes = nextDominantSpeciesCubes;
    nextDominantSpeciesCubes = NULL;
  }
  drawCubes(dominantSpeciesCubes);
}

void drawWorld(){
  if (nextWorldCubes != NULL){ //swap requested
    cleanupCubes(worldCubes);
    worldCubes = nextWorldCubes;
    nextWorldCubes = NULL;
  }
  drawCubes(worldCubes);
}

Cube* createCube(int x, int y, int z, Species* s){
  Cube* c = new Cube();
  c->x = x;
  c->y = y;
  c->z = z;
  c->r = s->r;
  c->g = s->g;
  c->b = s->b;
  return c;
}

void addSpeciesCubes(vector<Cube*>* destination, Species* s){
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

void updateWorldCubes(){
  if (nextWorldCubes != NULL) return;

  vector<Cube*>* tmpCubes = new vector<Cube*>();
  list<Species*>::iterator itSpecies;

  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    addSpeciesCubes(tmpCubes, s);
  }

  nextWorldCubes = tmpCubes;
}

void updateDominantSpeciesCubes(){
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

  if (dominantSpecies != NULL){
    int x=0, y=0, z=0;
    tmpCubes->push_back(createCube(x, y, z, dominantSpecies));
    vector<DNA*>::iterator itDNA;
    float max = 0, p;
    int maxIdx, directionIdx;

    for (itDNA = dominantSpecies->dna.begin(); itDNA != dominantSpecies->dna.end(); ++itDNA) {
      DNA* dna = (*itDNA);

      if (VARIABLE_GROWTH){
        max = 0;
        for (int i=0; i<6; ++i){
          p = dna->probas[i];
          if (max<p) {
            max = p;
            maxIdx = i;
          }
        }
        directionIdx = maxIdx;

      } else {
        directionIdx = dna->growthDirection;
      }

      if      (directionIdx==0) ++x;
      else if (directionIdx==1) --x;
      else if (directionIdx==2) ++y;
      else if (directionIdx==3) --y;
      else if (directionIdx==4) ++z;
      else if (directionIdx==5) --z;

      tmpCubes->push_back(createCube(x, y, z, dominantSpecies));
    }

    nextDominantSpeciesCubes = tmpCubes;
  }else{
    cout << "No dominance" << endl;
  }
}

void updateUI(){
  if (world.cycle == lastRenderingCycle) return;

  updateWorldCubes();
  updateDominantSpeciesCubes();

  string msg4("Sunshine");
  CLOCKS.status(CLOCK_SUNSHINE, msg4);
  CLOCKS.reset(CLOCK_SUNSHINE);

  string msg("Death");
  CLOCKS.status(CLOCK_DEATH, msg);
  CLOCKS.reset(CLOCK_DEATH);

  string msg2("Reproduction / Mutation");
  CLOCKS.status(CLOCK_REPRODUCTION, msg2);
  CLOCKS.reset(CLOCK_REPRODUCTION);

  string msg3("Growth");
  CLOCKS.status(CLOCK_GROWTH, msg3);
  CLOCKS.reset(CLOCK_GROWTH);

  list<Species*>::iterator itSpecies;
  long nbSpecies = 0,nbCreatures = 0, nbSpeciesWithMoreThanOneCreature=0;
  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    nbCreatures += s->creatures.size();
    if (s->creatures.size() > 1) ++nbSpeciesWithMoreThanOneCreature;
    ++nbSpecies;
  }
  cout << "Ending cycle "<<world.cycle<<" with "<<nbSpecies<<" species("<<nbSpeciesWithMoreThanOneCreature<<") and "<<nbCreatures<<" creatures."<<endl;

  float elapsed = CLOCKS.timers[CLOCK_LIFECYCLE]->elapsed/1000000.;
  if (elapsed >0){
    float cyclesPerSecs = world.cycle/elapsed;
    cout<<"Elapsed "<<elapsed<<" seconds, "<<cyclesPerSecs<<" cycles/s."<<endl;
  }

  lastRenderingCycle = world.cycle;
}

void playLife(){
  while (running && world.species.size() > 0){
    world.lifecycle();

    if (DEBUG || OUT_SUMMARY || world.cycle % UPDATE_UI_EVERY_CYCLES == 0) updateUI();
    if (DEBUG || OUT_SUMMARY) usleep(1000*500);
  }
  running = false;
  if (world.species.size() == 0){
    cout << "No creature survived." << endl;
    updateUI();
  }
}

void start(){
  if (!running){
    running = true;
    playLifeThread = new thread(playLife);
  }
}

void stop(){
  if (running){
    running = false;
    playLifeThread->join();
    delete playLifeThread;
  }
}

void exitWorld(){
  stop();
  // might still be used...
  //cleanupCubes(worldCubes);
  //cleanupCubes(dominantSpeciesCubes);
  cout << "Exiting." << endl;
  exit(0);
}

bool compByCreatures(const Species* a, const Species* b) {
  return a->creatures.size() > b->creatures.size();
}

void printSummary(){
  bool wasRunning = running;
  if (wasRunning) stop();

  world.species.sort(compByCreatures);

  cout << "Summary:" << endl;
  list<Species*>::iterator itSpecies = world.species.begin();
  int population = 0;

  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies){
    population += (*itSpecies)->creatures.size();
  }

  unsigned int threshold = population/100;
  cout<<"Total population: "<< population<<endl;
  cout<<"Species with more than 1% ("<<threshold<<") creatures:"<<endl;

  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies){
    Species* s = (*itSpecies);
    if (s->creatures.size() >= threshold){
      cout << "\tSpecies "<< s->id <<" with "<< s->creatures.size() << " creatures. ";
      int generation = 0;
      Species* cur = s;
      while (cur != NULL){
        generation += cur->distanceToAncestor;
        cur = cur->ancestor;
      }
      cout << "Generation "<< generation <<", ancestor is ";
      if (s->ancestor != NULL) cout<<s->ancestor->id;
      else                     cout<<"ROOT";
      cout<<". ";
      for (unsigned int j=0;j<s->dna.size();j++) cout<<s->dna[j]->growthDirection<<" ";
      cout<<endl;
    }
  }

  cout << "----------" << endl;

  if (wasRunning) start();
}

void checkConsistency(){
  bool wasRunning = running;
  if (wasRunning) stop();

  world.checkConsistency();

  if (wasRunning) start();
}

void keyboard(unsigned char key, int mouseX, int mouseY) {
  switch ( key ) {
    case 's': // Summary
      printSummary();
      break;
    case 'n': // Next
      if (!running){
        world.lifecycle();
        updateUI();
      }
      break;
    case ' ': // pause/play
      if (running) {
        stop();
      }else{
        start();
      }
      break;
    case 'f': // Fullscreen
      if (fullscreen){
        glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutPositionWindow(0,0);
        fullscreen = false;
      }else{
        glutFullScreen();
        fullscreen = true;
      }
      break;
    case 27: // exit
      exitWorld();
      break;
    default:
      cout<<"Unbound key: "<<key<<endl;
      break;
  }
}

void specialKeyboard(int key, int mouseX, int mouseY) {
  switch ( key ) {
    case GLUT_KEY_F1 : // consistency check
      checkConsistency();
      break;
    default:
      cout<<"Unbound key: "<<key<<endl;
      break;
  }
}

int main(int argc, char **argv) {
  if (LOCALITY_ENABLED){
    if (WORLD_LENGTH/(2*NB_THREADS) < (MAX_CELLS-1)*2){
      cout<<"ERROR: Incompatible number of threads of the world size/max cells combination:"<<endl;
      cout<<"World length="<<WORLD_LENGTH<<", Nb Threads="<<NB_THREADS<<", maxCells="<<MAX_CELLS<<endl;
      cout<<"Thread slices length="<<(WORLD_LENGTH/(2*NB_THREADS))<<", Creature overreach="<<((MAX_CELLS-1)*2)<<endl;
      exit(9);
    }
  }

  glutInit(&argc, argv);
  Rendering::initialize(drawWorld, drawDominantSpecies);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyboard);

  world.minimumEnergyPerCell = .5;

  int nbSpeciesToCreate = WORLD_LENGTH/5;

  world.infest(nbSpeciesToCreate,5);
  updateWorldCubes();

  start();
  glutMainLoop();

  return 0;
}
