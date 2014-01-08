#include "common.h"
#include "Cell.h"
#include "World.h"

Cell::Cell(Creature* creature, int x, int y, int z):creature(creature),x(x),y(y),z(z),energy(0){
  world.registry.registerCell(this);
}

Cell::~Cell(){
  world.registry.unregisterCell(this);
}
