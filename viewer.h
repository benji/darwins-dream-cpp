#ifndef VIEWER_H
#define VIEWER_H

#include "rendering.h"
#include "common.h"

struct Cube {
  public:
    int x,y,z;
    float r,g,b;
};

class Viewer {
  public:
    static void initialize();
    static Cube* createCube(int x, int y, int z, Species* s);
    static void drawCubes(vector<Cube*>* cubes);
    static void cleanupCubes(vector<Cube*>* cubes);
    static void drawWorld();
    static void drawDominantSpecies();
    static void updateWorldCubes();
    static void updateDominantSpeciesCubes(bool showBiggestCreatureOrTypical);
    static void addSpeciesCubes(vector<Cube*>* destination, Species* s);
    static void cleanup();
};

#endif
