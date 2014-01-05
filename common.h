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

#define GROW_OR_DIE true
#define VARIABLE_GROWTH false
#define WORLD_LENGTH 30
