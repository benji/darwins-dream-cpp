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


World world(10);
long RENDER_EVERY_CYCLES = 1;

void drawWorld(){
  std::list<Species>::const_iterator itSpecies;
  std::list<Creature>::const_iterator itCreature;
  std::list<Cell>::const_iterator itCell;
  
  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species s = *itSpecies;
    for (itCreature = s.creatures.begin(); itCreature != s.creatures.end(); ++itCreature) {
      Creature c = *itCreature;
      for (itCell = c.cells.begin(); itCell != c.cells.end(); ++itCell) {
        Cell cell = *itCell;
        Rendering::drawCube( cell.x, cell.y, cell.z );
      }
    }
  }
}

int main(int argc, char **argv) {
  world.infest(5,5);

  glutInit(&argc, argv);
  Rendering::initialize(drawWorld);

  std::thread renderingThread(glutMainLoop);
  renderingThread.join(); 

  return 0;
}
