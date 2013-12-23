#ifndef CREATURE_H
#define CREATURE_H

#include <list>
#include "Cell.h"

using namespace std;

class Creature {
  public:
    list<Cell> cells;
    Creature();
};

#endif
