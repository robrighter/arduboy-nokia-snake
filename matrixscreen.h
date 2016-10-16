#ifndef MATRIXSCREEN_H
#define MATRIXSCREEN_H
#include "Arduboy.h"
#include <stdlib.h>

#define X_WIDTH 25
#define Y_WIDTH 11

extern boolean screenMatrix[X_WIDTH][Y_WIDTH];
extern Arduboy arduboy;


void drawMatrix();
void clearMatrix();
void setPixel(byte x,byte y, bool on);

#endif
