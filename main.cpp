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

World world(100, 10, 0.1, 0.05);
long UPDATE_UI_EVERY_CYCLES = 30;
vector<Cube*>* cubes = new vector<Cube*>();
vector<Cube*>* nextCubes = NULL;
long START = time(0);
bool DEBUG = false;
bool OUT_SUMMARY = false;

bool running = false;
bool exitRequested = false;
thread* playLifeThread;

Clocks CLOCKS;
int CLOCK_DEATH = 0;
int CLOCK_REPRODUCTION = 1;
int CLOCK_GROWTH = 2;
int CLOCK_SUNSHINE = 4;

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
  while (running && world.species.size() > 0){

    if (DEBUG || OUT_SUMMARY) cout << "===== Cycle "<<world.cycle<<" ====="<<endl;
    world.lifecycle();

    if (world.cycle % UPDATE_UI_EVERY_CYCLES == 0){
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

      string msg10("10");
      CLOCKS.status(10, msg10);
      CLOCKS.reset(10);

      string msg11("11");
      CLOCKS.status(11, msg11);
      CLOCKS.reset(11);

      string msg12("12");
      CLOCKS.status(12, msg12);
      CLOCKS.reset(12);

      Cell* c;
      long x;

      CLOCKS.start(13);
      for (int k=0;k<100;k++)
        for (int i=0;i<100;i++)
          for (int j=0;j<100;j++){
            c=world.registry.registryXYZ[i][j][1];
            if (c != NULL){
              x += c->x;
            }
          }
      CLOCKS.pause(13);

      //cout << x <<endl;

      string msg13("13");
      CLOCKS.status(13, msg13);
      CLOCKS.reset(13);

      list<Species*>::iterator itSpecies;
      long nbSpecies = 0,nbCreatures = 0, nbSpeciesWithMoreThanOneCreature=0;
      for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
        Species* s = (*itSpecies);
        nbCreatures += s->creatures.size();
        if (s->creatures.size() > 1) ++nbSpeciesWithMoreThanOneCreature;
        ++nbSpecies;
      }
      cout << "Ending cycle with "<<nbSpecies<<" species("<<nbSpeciesWithMoreThanOneCreature<<") and "<<nbCreatures<<" creatures."<<endl;

      long elapsed = time(0)-START;
      if (elapsed >0){
        float cyclesPerSecs = world.cycle/elapsed;
        cout<<"Elapsed "<<elapsed<<" seconds, "<<cyclesPerSecs<<" cycles/s."<<endl;
      }
    }
    if (DEBUG || OUT_SUMMARY) usleep(1000*500);

  }
}

void start(){
  running = true;
  playLifeThread = new thread(playLife);
}

void stop(){
  running = false;
  playLifeThread->join();
  delete playLifeThread;
}

void exitWorld(){
  if (running) {
    stop();
  }
  // TODO cleanup
  cout << "Exiting." << endl;
  exit(0);
}



void keyboard(unsigned char key, int mouseX, int mouseY) {
  cout<<"Key pressed: "<<key<<endl;
  switch ( key ) {
    case ' ':
      if (running) {
        stop();
      }else{
        start();
      }
      break;
    case 27: // esc
      exitWorld();
      break;
    default:
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
