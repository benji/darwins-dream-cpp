#ifndef WORLD_H
#define WORLD_H

#include "common.h"
#include "Species.h"
#include "CellsRegistry.h"

class Species;

class World {
  public:
    list<Species*> species;
    long cycle;
    int length, maxCells;
    float reproductionRate, mutationRate, minimumEnergyPerCell;
    CellsRegistry registry;

    World(int length, int maxCells, float reproductionRate, float mutationRate);
    void infest(int nbSpecies, int nbCreaturesPerSpecies);
    void lifecycle();
    void checkConsistency();
    Species* createSpecies(Species* originalSpecies);
    Creature* reproduce(Species* s, Creature* parent);
    Species* evolve(Species* s, Creature* c);
    vector<Creature*> collectCreaturesCopy();
    vector<Species*> collectSpeciesCopy();
    void prepareSpeciesForDelete(Species* s);
    ~World();
};

#endif
