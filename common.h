#ifndef COMMON_H
#define COMMON_H

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;

#define GROW_OR_DIE true
#define VARIABLE_GROWTH false
#define CONSTRAINT_BALANCING true
// For reproduction and mutation
// potentially way faster
#define LOCALITY_ENABLED true

#define WORLD_LENGTH 200
#define MAX_CELLS 30
// Tweak this according to your hardware
const int NB_THREADS = 3;
const int BALANCING_THRESHOLD = 30;
const int LOCALITY_RADIUS = 8;

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <mutex>

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
