#ifndef SPECIES_H
#define SPECIES_H

#include <list>
#include "Creature.h"

using namespace std;

class Creature;

class DNA {
  public:
    float probas[6];
    DNA();
};

class Species {
  public:
    float r,g,b;
    DNA dna;
    list<Creature> creatures;
    Species();
    void setColor(float r, float g, float b);
};

#endif
