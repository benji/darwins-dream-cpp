#include "Cell.h"
#include "common.h"

Cell::Cell(int x, int y, int z):x(x),y(y),z(z){
  //world.registry.register(*this);
}

Cell::~Cell(){
  cout<<"cell died"<<endl;
}
