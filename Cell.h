#ifndef CELL_H
#define CELL_H

#include <list>
#include <string>

using namespace std;

class Cell {
  public:
    int x,y,z;
    string name;
    Cell(int x, int y, int z);
};

#endif
