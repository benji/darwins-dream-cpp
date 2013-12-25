#include <iostream>
#include <stdio.h>
#include <list>
#include <thread>
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
long RENDER_EVERY_CYCLES = 1;
bool running = false;
std::vector<Cube*> cubes;
bool lockCubes = false;

void drawWorld(){
  if (lockCubes == false){
    lockCubes = true;
    std::vector<Cube*>::iterator itCube;
    for (itCube = cubes.begin(); itCube != cubes.end(); ++itCube) {
      Cube* cube = (*itCube);
      Rendering::drawCube( cube->x, cube->y, cube->z, cube->r, cube->g, cube->b );
    }
    lockCubes = false;
  }
}

void playLife(){
  running = true;
  while(running){
    world.lifecycle();
    
    if (lockCubes == false){
      lockCubes = true;

      std::list<Species*>::iterator itSpecies;
      std::list<Creature*>::iterator itCreature;
      std::vector<Cell*>::iterator itCell;

      cubes.clear();
      
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
            cubes.push_back(c);
          }
        }
      }
      lockCubes = false;
    }
  }
}

void cleanupWorld(){
  running = false;
  cout << "Cleaning up" << endl;
}

int main(int argc, char **argv) {
  world.infest(1,1);

  glutInit(&argc, argv);
  Rendering::initialize(drawWorld, cleanupWorld);

  std::thread renderingThread(glutMainLoop);
  std::thread playLifeThread(playLife);

  renderingThread.join();
  playLifeThread.join();

  return 0;
}
