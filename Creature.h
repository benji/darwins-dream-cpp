#ifndef CREATURE_H
#define CREATURE_H

#include <list>
#include "Cell.h"

using namespace std;

class Creature {
  public:
    int x,y;
    list<Cell> cells;
    Creature(int x, int y);
};

#endif
