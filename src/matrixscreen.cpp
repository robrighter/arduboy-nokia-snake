#include "matrixscreen.h"

extern boolean screenMatrix[X_WIDTH][Y_WIDTH] = { 
 {0},
};

void drawPixel(byte x,byte y){
  arduboy.fillRect((5*x)+2, 5*y, 4, 4, 1);
}


void clearMatrix(){
  for(int x=0;x<X_WIDTH;x++){
    for(int y=0;y<Y_WIDTH;y++){
      screenMatrix[x][y] = false;
    }
  }
}

void drawMatrix(){
  arduboy.drawRect(1, 0, X_WIDTH*5, Y_WIDTH*5, 1);
  for(int x=0;x<X_WIDTH;x++){
    for(int y=0;y<Y_WIDTH;y++){
      if(screenMatrix[x][y] == true){
        drawPixel(x,y);  
      }
    }
  }
}

void setPixel(byte x,byte y, bool on){
  screenMatrix[x][y] = on;
}

void scrollText(String toscroll, boolean endOnJoystickMove) {
  //do nothing for now
}

