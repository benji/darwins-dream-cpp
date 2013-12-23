#include <iostream>
#include <stdio.h>
#include <list>
#include <GL/gl.h>
#include <GL/glut.h>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

using namespace std;

#include "utils.cpp"
#include "rendering.cpp"
#include "World.h"
#include "Species.h"
#include "Creature.h"
#include "Cell.h"


World world(10);

void drawWorld(){
  std::list<Species>::const_iterator itSpecies;
  std::list<Creature>::const_iterator itCreature;
  std::list<Cell>::const_iterator itCell;
  
  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species s = *itSpecies;
    for (itCreature = s.creatures.begin(); itCreature != s.creatures.end(); ++itCreature) {
      Creature c = *itCreature;
      drawCube( c.x, c.y, 0 );
      for (itCell = c.cells.begin(); itCell != c.cells.end(); ++itCell) {
      }
    }
  }
}

int main(int argc, char **argv) {
  world.infest(5,5);

  for (int i=0;i<10;i++){
    cout << randDouble() << endl;
  }

  glutInit(&argc, argv);
  initialize(drawWorld);
  glutMainLoop();

  return 0;
}
