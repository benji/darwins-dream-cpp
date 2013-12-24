#ifndef SPECIES_H
#define SPECIES_H

#include <list>
#include "Creature.h"

using namespace std;

class Creature;

class Species {
  public:
    list<Creature> creatures;
    Species();
};

#endif
