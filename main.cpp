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

World world(100, 30, 0.1, 0.05);
long UPDATE_UI_EVERY_CYCLES = 100;
bool running = false;
vector<Cube*>* cubes = new vector<Cube*>();
vector<Cube*>* nextCubes = NULL;
long START = time(0);
bool DEBUG = false;
bool OUT_SUMMARY = false;

Clocks CLOCKS;
int CLOCK_DEATH = 0;
int CLOCK_REPRODUCTION = 1;
int CLOCK_GROWTH = 2;

void drawWorld(){
  if (nextCubes != NULL){ //swap requested
    // cleanup
    vector<Cube*>::iterator itCube;
    for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
      Cube* cube = (*itCube);
      delete cube;
    }
    cubes->clear();
    delete cubes;

    // swap
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


void playLife(){
  running = true;
  while(running && world.species.size() > 0){
    if (DEBUG || OUT_SUMMARY) cout << "===== Cycle "<<world.cycle<<" ====="<<endl;
    world.lifecycle();
    updateCubes();

    if (world.cycle % UPDATE_UI_EVERY_CYCLES == 0){
  
      string msg("Death");
      CLOCKS.status(CLOCK_DEATH, msg);
      string msg2("Reproducion");
      CLOCKS.status(CLOCK_REPRODUCTION, msg2);
      string msg3("Growth");
      CLOCKS.status(CLOCK_GROWTH, msg3);

      CLOCKS.reset(CLOCK_DEATH);
      CLOCKS.reset(CLOCK_REPRODUCTION);
      CLOCKS.reset(CLOCK_GROWTH);

      long elapsed = time(0)-START;
      if (elapsed >0){
        float cyclesPerSecs = world.cycle/elapsed;
        cout<<"Elapsed "<<elapsed<<" seconds, "<<cyclesPerSecs<<" cycles/s."<<endl;
      }
    }
    if (DEBUG || OUT_SUMMARY) usleep(1000*500);
  }
}

void cleanupWorld(){
  running = false;
  cout << "Cleaning up" << endl;
}

int main(int argc, char **argv) {
  world.infest(5,5);
  updateCubes();

  glutInit(&argc, argv);
  Rendering::initialize(drawWorld, cleanupWorld);

  std::thread renderingThread(glutMainLoop);
  std::thread playLifeThread(playLife);

  renderingThread.join();
  playLifeThread.join();

  return 0;
}
