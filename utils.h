#ifndef UTILS_H
#define UTILS_H

void initSRand();
double randDouble();
int randInt(int max); // [0;max[

int posToIndex(int length, int x, int y);
void indexToPos(int* pos, int length, int index);

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );

#endif
