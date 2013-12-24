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


World world(10, 0.2, 0.05);
long RENDER_EVERY_CYCLES = 1;
bool running = false;

void drawWorld(){
  std::list<Species>::const_iterator itSpecies;
  std::list<Creature>::const_iterator itCreature;
  std::vector<Cell>::const_iterator itCell;
  
  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species s = *itSpecies;
    for (itCreature = s.creatures.begin(); itCreature != s.creatures.end(); ++itCreature) {
      Creature c = *itCreature;
      for (itCell = c.cells.begin(); itCell != c.cells.end(); ++itCell) {
        Cell cell = *itCell;
        Rendering::drawCube( cell.x, cell.y, cell.z, s.r, s.g, s.b );
      }
    }
  }
}

void playLife(){
  running = true;
  while(running){
    world.lifecycle();
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
