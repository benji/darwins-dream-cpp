#include <GL/glut.h>

#include "rendering.h"
#include "viewer.h"
#include "World.h"
#include "Species.h"
#include "Creature.h"
#include "Cell.h"
#include "Clocks.h"
#include "utils.h"
#include "common.h"

using namespace std;

World world(WORLD_LENGTH, MAX_CELLS, 0.1, 0.01);

bool running = false;
thread* playLifeThread;
bool fullscreen = false;
bool showBiggestCreatureOrTypical = true;

long lastRenderingCycle = -1;

Clocks CLOCKS;
Viewer viewer;

void updateUI(){
  if (world.cycle == lastRenderingCycle) return;

  viewer.updateWorldCubes();
  viewer.updateDominantSpeciesCubes(showBiggestCreatureOrTypical);

  // Stats
  list<Species*>::iterator itSpecies;
  long nbSpecies = 0,nbCreatures = 0, nbSpeciesWithMoreThanOneCreature=0;
  for (itSpecies = world.species.begin(); itSpecies != world.species.end(); ++itSpecies) {
    Species* s = (*itSpecies);
    nbCreatures += s->creatures.size();
    if (s->creatures.size() > 1) ++nbSpeciesWithMoreThanOneCreature;
    ++nbSpecies;
  }
  cout << "Ending cycle "<<world.cycle<<" with "<<nbSpecies<<" species("<<nbSpeciesWithMoreThanOneCreature<<") and "<<nbCreatures<<" creatures."<<endl;

  // Clocks
  CLOCKS.printAndReset(CLOCK_SUNSHINE, "Sunshine");
  CLOCKS.printAndReset(CLOCK_DEATH, "Death");
  CLOCKS.printAndReset(CLOCK_REPRODUCTION, "Reproduction / Mutation");
  CLOCKS.printAndReset(CLOCK_GROWTH, "Growth");

  float elapsed = CLOCKS.timers[CLOCK_LIFECYCLE]->elapsed/1000000.;
  if (elapsed >0){
    cout<<"Elapsed "<<elapsed<<" seconds, "<< (world.cycle/elapsed) <<" cycles/s."<<endl;
  }
  cout<<endl;

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
    updateUI();
    
    delete playLifeThread;
  }
}

void exitWorld(){
  stop();
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
    case '=':
      cout<<"Switching creature view to ";
      if (showBiggestCreatureOrTypical) cout<<"Biggest Creature" <<endl;
      else cout<<"Typical DNA" <<endl;
      showBiggestCreatureOrTypical = !showBiggestCreatureOrTypical;
      break;
    case 27: // exit
    case 'q':
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
    int sliceWidth = WORLD_LENGTH/(2*NB_THREADS);
    int overreach = LOCALITY_RADIUS*2;
    if (sliceWidth < overreach){
      cout<<"ERROR: Incompatible number of threads of the world size/max cells/locality radius combination:"<<endl;
      cout<<"World length="<<WORLD_LENGTH<<", Nb Threads="<<NB_THREADS<<", maxCells="<<MAX_CELLS<<", locality radius="<< LOCALITY_RADIUS <<endl;
      cout<<"Thread slices width="<<sliceWidth<<", Creature overreach="<<overreach<<endl;
      exit(9);
    }
  }

  glutInit(&argc, argv);
  viewer.initialize();
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyboard);

  int nbSpeciesToCreate = WORLD_LENGTH/5;

  world.infest(nbSpeciesToCreate,5);
  viewer.updateWorldCubes();

  start();
  glutMainLoop();

  return 0;
}
