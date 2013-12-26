#include "utils.h"

#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

bool SRAND_INITIALIZED = false;

void initSRand(){
  if (!SRAND_INITIALIZED) {
    srand(time(0));
    SRAND_INITIALIZED = true;
  }
}

double randDouble(){
  initSRand();
  return 1.*(rand() % 100000) / 100000.;
}

// [0;max[
int randInt(int max){
  if (max <= 0){
    cerr << "randInt: Invalid max: "<<max << endl;
    exit(-1);
  }
  initSRand();
  return rand() % max;
}

int posToIndex(int length, int x, int y){
  return x+length*y;
}

void indexToPos(int* pos, int length, int index){
  pos[0] = index % length;
  pos[1] = index / length;
}

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ){
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}
