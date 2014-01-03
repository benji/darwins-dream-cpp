#ifndef SPECIES_H
#define SPECIES_H

#include "common.h"
#include "Creature.h"

using namespace std;

class Creature;

class DNA {
  public:
    int growthDirection;
    float probas[6];
    DNA(DNA* from);
};

class Species {
  public:
    float r,g,b;
    vector<DNA*> dna;
    list<Creature*> creatures;
    long id;

    Species(Species* originalSpecies);
    void setColor(float r, float g, float b);
    DNA* getDNA(int idx);
    int killOldAndWeakCreatures();
    Creature* reproduce(int x, int y);
    ~Species();
};

#endif
