#ifndef UTILS_H
#define UTILS_H

void initSRand();

double randDouble();

// [0;max[
int randInt(int max);

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );


int posToIndex(int length, int x, int y);
void indexToPos(int* pos, int length, int index);

#endif
