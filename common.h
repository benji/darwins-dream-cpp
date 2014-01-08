#ifndef COMMON_H
#define COMMON_H

#define GROW_OR_DIE true
#define VARIABLE_GROWTH false
#define WORLD_LENGTH 400
#define MAX_CELLS 30

// For reproduction and mutation
// potentially way faster
#define LOCALITY_ENABLED true
const int LOCALITY_RADIUS = 8;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;
const int NB_THREADS = 3; // Tweak this according to your hardware

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "utils.h"

using namespace std;

class World;
class Species;
class Creature;
class Cell;
class CellRegistry;
class Timer;
class Clocks;

extern World world;
extern bool DEBUG;
extern bool OUT_SUMMARY;

extern Clocks CLOCKS;
extern int CLOCK_LIFECYCLE;
extern int CLOCK_DEATH;
extern int CLOCK_REPRODUCTION;
extern int CLOCK_GROWTH;
extern int CLOCK_SUNSHINE;

#endif
