/*
* keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "stopwatch.h"
#include "../theme.h"
Stopwatch::Stopwatch(System *System, HAL * Hal) {
  sys = System;
  hal = Hal;
  curdig0 = curdig1 = curdig2 = curdig3  = 0;
  prevdig0 = prevdig1 = prevdig2 = prevdig3  = 0;

}

void Stopwatch::route() {
  TouchEvent thisEvent = sys->getTouchEvent();
  if (!thisEvent.dispatched) {
    stopWatchActive = !stopWatchActive;
  }
  //   if(buttonUp.wasPressed()) stopwatchActive = !stopwatchActive;
  //   if(buttonDown.wasPressed()){
  //     stopwatchTimer = millis();
  //     stopwatchMs = 0;
  //   }

  if(!stopWatchActive) stopwatchTimer = millis() - stopwatchMs;
  stopwatchMs = millis()-stopwatchTimer;

  // hal->display->fillRect(20, 80, 100, 80, 0x0000);
  //2+44
  curdig0 = ((stopwatchMs/60000)/10);
  curdig1 = ((stopwatchMs/60000)%10);
  curdig2 = (stopwatchMs/1000%60)/10;
  curdig3 =  (stopwatchMs/1000%60)%10;
  if (curdig0 != prevdig0) {
    drawDigit( 66, 80, prevdig0, WHITE);
  }
  drawDigit( 66, 80, curdig0, PRIMARY);
  if (curdig1 != prevdig1) {
    drawDigit(88, 80, prevdig1, WHITE);
  }
  drawDigit(88, 80, curdig1, PRIMARY);
  if (curdig2 != prevdig2) {
    drawDigit(116, 80, prevdig2, WHITE);
  }
  drawDigit(116, 80, curdig2, PRIMARY);
  if (curdig3 != prevdig3) {
    drawDigit(138, 80, prevdig3, WHITE);
  }
  drawDigit(138, 80, curdig3, PRIMARY);
  prevdig0 = ((stopwatchMs/60000)/10);
  prevdig1 = ((stopwatchMs/60000)%10);
  prevdig2 = (stopwatchMs/1000%60)/10;
  prevdig3 =  (stopwatchMs/1000%60)%10;
  hal->display->setCursor(118, 130);
  hal->display->setTextColor(BLACK, WHITE);
  hal->display->setTextSize(2);
  if(stopwatchMs%1000 < 10) hal->display->print(0);
  if(stopwatchMs%1000 < 100) hal->display->print(0);
  hal->display->print(stopwatchMs%1000); hal->display->print("ms");
  hal->display->fillRect(110, 90, 4, 4, PRIMARY);
  hal->display->fillRect(110, 106, 4, 4, PRIMARY);
  /*
  if(buttonMid.wasPressed()){
  stopwatchActive = false;
  stopwatchMs = 0;
  currentPage = 9; //switch back to menu
  buttonMid.read(); //make sure wasPressed is not activated again
}*/
}


void Stopwatch::drawDigit(int posX, int posY, int digit, uint16_t col){
  switch (digit){
    case 0:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY, 7, 40, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 1:
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    break;
    case 2:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX, posY+24, 7, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 3:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 4:
    hal->display->fillRect(posX, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    break;
    case 5:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY+24, 7, 8, col);
    hal->display->fillRect(posX, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 6:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY+24, 7, 8, col);
    hal->display->fillRect(posX, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 7:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    break;
    case 8:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 9:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
  }
}
