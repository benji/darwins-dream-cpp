#ifndef SPECIES_H
#define SPECIES_H

#include <list>
#include <string>
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
    vector<DNA*> dna;
    string name;
    float test1;
    float test2;
    float test3;
    list<Creature*> creatures;
    Species();
    void setColor(float r, float g, float b);
    void killOldCreatures();
    void reproduce(int x, int y);
};

#endif
