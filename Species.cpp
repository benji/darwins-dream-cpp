#include "Species.h"
#include <iostream>

Species::Species(){
  cout << "new species!" << endl;
}

void Species::setColor(float r, float g, float b){
  this->r=r; this->g=g; this->b=b;
}
