#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <algorithm>

#ifndef COMMON_H
#define COMMON_H

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;

const bool GROW_OR_DIE = true;
const bool CONTRAINT_NEED_SUN = true;
const bool CONSTRAINT_BALANCING = true;
const bool LOCALITY_ENABLED = true;
const bool PROGRESSIVE_DNA = false;

const int WORLD_LENGTH = 200;
const int MAX_CELLS = 30;
const float REPRODUCTION_RATE = 0.1;
const float MUTATION_RATE = 0.01;
const float MIN_ENERGY_PER_CELL = .5;
const float SUNLIGHT_LOSS_RATIO = 0.15;
const int BALANCING_THRESHOLD = 6*6;
const int MAX_CREATURE_AGE = 30;
const int LOCALITY_RADIUS = 8;

const int NB_THREADS = 3;

const bool DEBUG = false;
const bool OUT_SUMMARY = false;
const bool LOG_SPECIES_EXTINCTION_CAUSES = false;

const long UPDATE_UI_EVERY_CYCLES = 300;

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
extern Clocks CLOCKS;

const int CLOCK_LIFECYCLE = 10;
const int CLOCK_DEATH = 0;
const int CLOCK_REPRODUCTION = 1;
const int CLOCK_GROWTH = 2;
const int CLOCK_SUNSHINE = 4;

#endif
