#include <stdio.h>
#include <list>
#include <iostream>

#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#include <GL/gl.h>
#include <GL/glut.h>

#include "rendering.cpp"
#include "World.h"
#include "Species.h"
#include "Creature.h"
#include "Cell.h"

#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
  World world;
  world.infest(5,5);

  double n = (unsigned)time(0);
  cout << n << endl;
  srand(n);
  for (int i=0;i<10;i++){
    cout << (rand()%10) << endl;
  }

	glutInit(&argc, argv);
	initialize();
	glutMainLoop();

	return 0;
}
