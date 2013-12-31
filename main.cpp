#include <iostream>
#include <stdio.h>
#include <list>
#include <thread>
#include <ctime>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

using namespace std;

#include "rendering.h"
#include "World.h"
#include "Species.h"
#include "Creature.h"
#include "Cell.h"
#include "Clocks.h"
#include "utils.h"

struct Cube {
  public:
    int x,y,z;
    float r,g,b;
};

World world(100, 100, 0.1, 0.001);

bool running = false;
thread* playLifeThread;
long START = time(0);

long UPDATE_UI_EVERY_CYCLES = 100;
long lastRenderingCycle = -1;
vector<Cube*>* cubes = new vector<Cube*>();
vector<Cube*>* nextCubes = NULL;

bool DEBUG = false;
bool OUT_SUMMARY = false;

Clocks CLOCKS;
int CLOCK_LIFECYCLE = 10;
int CLOCK_DEATH = 0;
int CLOCK_REPRODUCTION = 1;
int CLOCK_GROWTH = 2;
int CLOCK_SUNSHINE = 4;

void cleanupCubes(){
  vector<Cube*>::iterator itCube;
  for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
    Cube* cube = (*itCube);
    delete cube;
  }
  cubes->clear();
  delete cubes;
}

void drawWorld(){
  if (nextCubes != NULL){ //swap requested
    cleanupCubes();
    cubes = nextCubes;
    nextCubes = NULL;
  }

  vector<Cube*>::iterator itCube;
  for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
    Cube* cube = (*itCube);
    Rendering::drawCube( cube->x, cube->y, cube->z, cube->r, cube->g, cube->b );
  }
}

void updateCubes(){
  if (nextCubes != NULL) return;

  vector<Cube*>* tmpCubes = new vector<Cube*>();

  list<Species*>::iterator itSpecies;
  list<Creature*>::iterator itCreature;
  vector<Cell*>::iterator itCell;
  
  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    for (itCreature = s->creatures.begin(); itCreature != s->creatures.end(); ++itCreature) {
      Creature* c = (*itCreature);
      for (itCell = c->cells.begin(); itCell != c->cells.end(); ++itCell) {
        Cell* cell = (*itCell);
        Cube* c = new Cube();
        c->x = cell->x;
        c->y = cell->y;
        c->z = cell->z;
        c->r = s->r;
        c->g = s->g;
        c->b = s->b;
        tmpCubes->push_back(c);
      }
    }
  }

  nextCubes = tmpCubes;
}

void updateUI(){
  if (world.cycle == lastRenderingCycle) return;

  updateCubes();

  string msg4("Sunshine");
  CLOCKS.status(CLOCK_SUNSHINE, msg4);
  CLOCKS.reset(CLOCK_SUNSHINE);

  string msg("Death");
  CLOCKS.status(CLOCK_DEATH, msg);
  CLOCKS.reset(CLOCK_DEATH);

  string msg2("Reproduction");
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
  cleanupCubes();
  cout << "Exiting." << endl;
  exit(0);
}

void printSummary(){
  stop();

  cout << "Summary:" << endl;
  list<Species*>::iterator itSpecies = world.species.begin();
  int i = 0;
  while (i<=5 && itSpecies != world.species.end()){
    Species* s = (*itSpecies);
    cout << "\tSpecies "<< s->id <<" with "<< s->creatures.size() << " creatures." << endl;
    ++itSpecies;
  }
  cout << "----------" << endl;

  start();
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
    case 27: // exit
      exitWorld();
      break;
    default:
      cout<<"Unbound key: "<<key<<endl;
      break;
  }
}

int main(int argc, char **argv) {
  world.infest(5,5);
  updateCubes();

  start();

  glutInit(&argc, argv);
  Rendering::initialize(drawWorld, keyboard);
  glutMainLoop();

  return 0;
}
