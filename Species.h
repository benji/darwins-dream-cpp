#ifndef SPECIES_H
#define SPECIES_H

#include <list>
#include "Creature.h"

using namespace std;

class Creature;

class Species {
  public:
    float r,g,b;
    list<Creature> creatures;
    Species();
    void setColor(float r, float g, float b);
};

#endif
