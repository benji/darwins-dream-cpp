#ifndef SPECIES_H
#define SPECIES_H

#include "common.h"
#include "Creature.h"

using namespace std;

class Creature;

class DNA {
  public:
    int cellIdx;
    int growthDirection;

    DNA(DNA* from);
};

class Species {
  public:
    float r,g,b;
    vector<DNA*> dna;
    list<Creature*> creatures;
    long id;
    Species* ancestor;
    int distanceToAncestor;
    mutex creaturesCollectionMutex;

    // statistics
    long deathsByLackOfEnergy;
    long deathsByLackOfBalance;
    long deathsByOverGrowth;
    long deathsByOldAge;
    long deathsByCantGrow;
    bool noRoomForNewSpecies;

    Species(Species* originalSpecies);
    void setColor(float r, float g, float b);
    void kill(Creature* c);
    int killOldAndWeakCreatures();
    Creature* reproduce(int x, int y);
    bool generateDna();
    void clearDna();
    ~Species();
};

#endif
