#ifndef COMMON_H
#define COMMON_H

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;

const bool GROW_OR_DIE = true;
const bool VARIABLE_GROWTH = false;
const bool CONSTRAINT_BALANCING = true;
// For reproduction and mutation
// potentially way faster
const bool LOCALITY_ENABLED = true;

const int WORLD_LENGTH = 200;
const int MAX_CELLS = 30;
const float REPRODUCTION_RATE = 0.1;
const float MUTATION_RATE = 0.01;
const float MIN_ENERGY_PER_CELL = .7;

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
