#include "common.h"
#include "Cell.h"
#include "World.h"

Cell::Cell(int x, int y, int z, bool registerCell):x(x),y(y),z(z){
  if (registerCell){
    world.registry.registerCell(this);
  }
}

Cell::~Cell(){
  world.registry.unregisterCell(this);
}
