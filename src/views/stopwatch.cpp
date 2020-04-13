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

  UIObject currentTimeUI;
  UIObject stopWatchTimerUIObj;

  DateTimeArray  currentTime = sys->getCurrentTime();
  currentTimeUI.digit_0 = currentTime.hh;
  currentTimeUI.digit_1 = currentTime.ii;
  currentTimeUI.digit_2 = currentTime.ss;
  currentTimeUI.front_color = 0x0CC0;
  sys->ui->renderTopLeft(true, currentTimeUI);
  sys->ui->renderTopRight(false, currentTimeUI);
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
  stopWatchTimerUIObj.digit_0 = (stopwatchMs/60000);
  stopWatchTimerUIObj.digit_1 = (stopwatchMs/1000%60);
  stopWatchTimerUIObj.digit_2 = stopwatchMs%100;
  if (stopWatchActive) {
    stopWatchTimerUIObj.front_color = RED;
  } else {
    stopWatchTimerUIObj.front_color = PRIMARY;
  }
  sys->ui->renderMainBlockNumbers(true, stopWatchTimerUIObj);

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
