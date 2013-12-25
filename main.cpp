#include <iostream>
#include <stdio.h>
#include <list>
#include <thread>
#include <ctime>
#include <GL/gl.h>
#include <GL/glut.h>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

using namespace std;

#include "utils.h"
#include "rendering.h"
#include "World.h"
#include "Species.h"
#include "Creature.h"
#include "Cell.h"

struct Cube {
  public:
    int x,y,z;
    float r,g,b;
};

World world(10, 0.2, 0.05);
long UPDATE_UI_EVERY_CYCLES = 1000;
bool running = false;
vector<Cube*>* cubes = new vector<Cube*>();
vector<Cube*>* nextCubes = NULL;
long START = time(0);

void drawWorld(){
  if (nextCubes != NULL){ //swap requested
    // cleanup
    vector<Cube*>::iterator itCube;
    for (itCube = cubes->begin(); itCube != cubes->end(); ++itCube) {
      Cube* cube = (*itCube);
      delete cube;
    }
    cubes->clear();

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
  while(running){
    world.lifecycle();
    updateCubes();
    if (world.cycle % UPDATE_UI_EVERY_CYCLES == 0){
      long elapsed = time(0)-START;
      if (elapsed >0){
        float cyclesPerSecs = world.cycle/elapsed;
        cout<<"Elapsed "<<elapsed<<" seconds, "<<cyclesPerSecs<<" cycles/s."<<endl;
      }
    }
  }
}

void cleanupWorld(){
  running = false;
  cout << "Cleaning up" << endl;
}

int main(int argc, char **argv) {
  world.infest(5,5);

  glutInit(&argc, argv);
  Rendering::initialize(drawWorld, cleanupWorld);

  std::thread renderingThread(glutMainLoop);
  std::thread playLifeThread(playLife);

  renderingThread.join();
  playLifeThread.join();

  return 0;
}
