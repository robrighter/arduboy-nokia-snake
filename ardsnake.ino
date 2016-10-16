#include "Arduboy.h"
#include "matrixscreen.h"
Arduboy arduboy;

const byte JOYSTICK_DIRECTION_NONE = 5;
const byte JOYSTICK_DIRECTION_UP = 2;
const byte JOYSTICK_DIRECTION_DOWN = 4;
const byte JOYSTICK_DIRECTION_LEFT = 1;
const byte JOYSTICK_DIRECTION_RIGHT = 3;
const byte JOYSTICK_DIRECTION_CENTER = 0;

byte lastDirection = JOYSTICK_DIRECTION_NONE;
byte countDownDelay = 15;
byte snakeLevel = 1;
byte snakeLength = 2;
int8_t xSnake[128] = {1,0,-1};
int8_t ySnake[128] = {0,0,-1};
byte currentSnakeDirection = JOYSTICK_DIRECTION_RIGHT;
int8_t applePosition[2] = {2,5};
boolean shouldGrow = false;


void resetTheGame(){
  snakeLevel = 1;
  snakeLength = 2;
  xSnake[0] = 1;
  xSnake[1] = 0;
  xSnake[2] = -1;
  ySnake[0] = 0;
  ySnake[1] = 0;
  ySnake[2] = -1;
  currentSnakeDirection = JOYSTICK_DIRECTION_RIGHT;
  applePosition[0] = 2;
  applePosition[1] = 5;
  shouldGrow = false;
}

byte getJoystickDirection(){
  if(arduboy.pressed(LEFT_BUTTON)){
    return JOYSTICK_DIRECTION_LEFT;
  }
  if(arduboy.pressed(RIGHT_BUTTON)){
    return JOYSTICK_DIRECTION_RIGHT;
  }
  if(arduboy.pressed(DOWN_BUTTON)){
    return JOYSTICK_DIRECTION_DOWN;
  }
  if(arduboy.pressed(UP_BUTTON)){
    return JOYSTICK_DIRECTION_UP;
  }
  if(arduboy.pressed(B_BUTTON)){
    return JOYSTICK_DIRECTION_CENTER;  
  }
  return JOYSTICK_DIRECTION_NONE;
}



boolean moveTheSnake(byte direction, boolean grow){
  //find the tail (-1,-1)
  int8_t offset = 0;
  while(xSnake[offset]>-1){
    offset++;
  }
  int8_t xTail = offset + (grow ? 1 : 0);
  int8_t yTail = offset + (grow ? 1 : 0);
  
  while(offset>-1){
    xSnake[offset+1] = xSnake[offset]; 
    ySnake[offset+1] = ySnake[offset]; 
    offset--;
  }

  xSnake[xTail] = -1;
  ySnake[yTail] = -1;

  if(direction == JOYSTICK_DIRECTION_UP){
      ySnake[0]--;
      if(ySnake[0]<0){
        //hit the end of the screen
        return true; 
      }
  }
  if(direction == JOYSTICK_DIRECTION_DOWN){
      ySnake[0]++;
      if(ySnake[0]>(Y_WIDTH-1)){
        //hit the end of the screen
        return true; 
      }  
  }
  if(direction == JOYSTICK_DIRECTION_LEFT){
      xSnake[0]--;
      if(xSnake[0]<0){
        //hit the end of the screen
        return true; 
      }  
  }
  if(direction == JOYSTICK_DIRECTION_RIGHT){
    xSnake[0]++;
    if(xSnake[0]>(X_WIDTH-1)){
      //hit the end of the screen
      return true; 
    }   
  }
  
  return false;
}

boolean isCoordInSnake(int8_t x, int8_t y, byte startFrom=0){
  while(xSnake[startFrom] != -1){
    if((xSnake[startFrom] == x) && (ySnake[startFrom] == y)){
      return true;  
    }
    startFrom++;
  }
  return false;
}

boolean doesSnakeOverlap(){
  byte i = 0;
  while(xSnake[i] != -1){
    if(isCoordInSnake(xSnake[i], ySnake[i], i+1)){
      return true;
    }
    i++;
  }
  return false;
}

void setNewApplePosition(byte direction){
  byte possibleLocationsForTheApple = snakeLength - (X_WIDTH * Y_WIDTH);
  long choosenLocation = random(possibleLocationsForTheApple);
  int8_t x;
  int8_t y;
  //iterate through the possible locations until we get to the choosen location
  int i=0;
  for(y=(Y_WIDTH-1); y>-1; y--){
    for(x=0; x<X_WIDTH; x++){
      
      if(!isCoordInSnake(x, y)){
        i++;
        if(i==choosenLocation){
          applePosition[0] = x;
          applePosition[1] = y;
          return;  
        }  
      }
    }  
  }
  //should never get here
}

byte readNewSnakeDirection(){
  for(byte i=0; i<(countDownDelay - snakeLevel ); i++){
    byte readDirection = getJoystickDirection();
    if((readDirection != JOYSTICK_DIRECTION_NONE) && (readDirection != JOYSTICK_DIRECTION_CENTER)){
      currentSnakeDirection = readDirection;  
    }
    delay(10);
  }
  return currentSnakeDirection;
}

void drawSnake(){
  for(int8_t i=0; xSnake[i]>-1; i++){
     setPixel(xSnake[i],ySnake[i], true); 
  }
}

void drawTheApple(){
  setPixel(applePosition[0],applePosition[1], true);
}

void snakeGameOver(){
  
  while(true){
    if(arduboy.nextFrame()){
      arduboy.setCursor(27, 20);
      arduboy.print("Game Over :-(");
      arduboy.display();  
    }
    
    if(arduboy.pressed(B_BUTTON) || arduboy.pressed(A_BUTTON)){
      //reset the game and return
      resetTheGame();
      return;  
    }
  }
}



void intro()
{
  for(int i = -8; i < 28; i = i + 2)
  {
    arduboy.clear();
    arduboy.setCursor(26, i);
    arduboy.print("NOKIA SNAKE!!!");
    arduboy.display();
  }

  arduboy.tunes.tone(987, 160);
  delay(160);
  arduboy.tunes.tone(1318, 400);
  delay(2000);
}

void drawMatrixScreen(){
  for(int x=0; x<25; x++){
    for(int y=0; y<11; y++){
      arduboy.fillRect((5*x)+2, 5*y, 4, 4, 1);   
    }
  }
  
}



void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.display();
  intro();
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  delay(90);
  boolean isGameOverEvent = false;
  isGameOverEvent = moveTheSnake(currentSnakeDirection, shouldGrow);
  if(shouldGrow){
    //last time we ate the apple so lets move it to a new location
    setNewApplePosition(currentSnakeDirection);
  }

  clearMatrix();
  arduboy.clear();
  drawSnake();
  drawTheApple();
  drawMatrix();
  arduboy.setCursor(2, 57);
  char displaystring[20];
  sprintf(displaystring, "SNAKE LENGTH: %d", snakeLength);
  arduboy.print(displaystring);
  arduboy.display();
  
  //See if the snake hit itself (look for duplicate coords)
  if(!isGameOverEvent){
    isGameOverEvent = doesSnakeOverlap();
  }
  
  if(isGameOverEvent){
    snakeGameOver();
    return;
  }

  //handle eating the apple
  if( isCoordInSnake(applePosition[0], applePosition[1]) ){
    //The snake ate the apple
    shouldGrow = true;
    snakeLength++;
  }
  else{
    shouldGrow = false;
  }

  readNewSnakeDirection();

}
