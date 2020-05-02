/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "watchface.h"
#include "../theme.h"
WatchFace::WatchFace(System *System, HAL * Hal) {
  curPath = 0; // Default path is 0 (show clock), 1 would be watchface select
  sys = System;
  hal = Hal;
}

void WatchFace::route() {
  switch (curPath) {
    case 0:
      renderWatchFaceView();
      break;
    default:
      break;
  }
}

void WatchFace::renderWatchFaceView(void) {
  DateTimeArray  currentTime = sys->getCurrentTime();

//   renderDigitalWatchFace(currentTime);
  // renderAnalogWatchFace(true, prevTime);
   //renderAnalogWatchFace(false, currentTime);
   UIObject currentTimeUI;
   currentTimeUI.digit_0 = currentTime.hh;
   currentTimeUI.digit_1 = currentTime.ii;
   currentTimeUI.digit_2 = currentTime.ss;
   currentTimeUI.front_color = 0x0CC0;
   sys->ui->renderTopLeft(true, currentTimeUI);
   sys->ui->renderTopRight(true, currentTimeUI);
   sys->ui->renderMainBlockNumbers(true, currentTimeUI);
   renderSteps();
//   renderBattery();
}


void WatchFace::renderDigitalWatchFace(DateTimeArray datetime) {
  int first_digit = 48;
  int digit_size = 24;
  int ypos = 100;
  UIObject currentTime;
 // Structure should be 00:00:00 to 23:59:59
  // First digit Position: 66, others 66 + (20 * X)
  // position (y): 80
  // 6 Digits total
  //Hours

   hal->display->setTextSize(2);

      hal->display->setCursor(60, 160);
      hal->display->setTextColor(BLACK, WHITE);

      // Date
      if (datetime.dd < 10) {
        hal->display->print("0");
      }
      hal->display->print(datetime.dd);
      hal->display->print("/");
      if (datetime.mm < 10) {
        hal->display->print("0");
      }
      hal->display->print(datetime.mm);
      hal->display->print("/");
      if (datetime.yy < 10) {
        hal->display->print("000");
      }
      hal->display->println(datetime.yy);
/*
      hal->display->setCursor(70, 205);

      // Time
      if (datetime.hh < 10) {
        hal->display->print("0");
      }
      hal->display->print(datetime.hh);
      hal->display->print(":");
      if (datetime.ii < 10) {
        hal->display->print("0");
      }
      hal->display->print(datetime.ii);
      hal->display->print(":");
      if (datetime.ss < 10) {
        hal->display->print("0");
      }
      hal->display->println(datetime.ss);
      hal->display->setCursor(60, 140);*/
}
void WatchFace::renderAnalogWatchFace(bool inv, DateTimeArray datetime) {
  int z,x2,x3,y2,y3;
  float angle;
  // Size total: 240, 240,
  // Size to render: 20 - 180
  // Now draw the clock face
   hal->display->drawCircle(120, 120, 80, WHITE);
   hal->display->drawCircle(120, 120, 2, PRIMARY);
   //
   //hour ticks
   for( int z=0; z < 360;z= z + 30 ){
   //Begin at 0° and stop at 360°
     float angle = z ;
     angle=(angle/57.29577951) ; //Convert degrees to radians
      x2=(120+(sin(angle)*80));
     y2=(120-(cos(angle)*80));
     x3=(120+(sin(angle)*(80-15)));
     y3=(120-(cos(angle)*(80-15)));
     hal->display->drawLine(x2,y2,x3,y3,GREEN);
   }
   // display second hand
    angle = datetime.ss*6 ;
   angle=(angle/57.29577951) ; //Convert degrees to radians
   x3=(120+(sin(angle)*(80)));
   y3=(120-(cos(angle)*(80)));
   if (inv) {
     hal->display->drawLine(120,120,x3,y3,BLACK);
   } else {
     hal->display->drawLine(120,120,x3,y3,WHITE);
   }
   //
   // display minute hand
   angle = datetime.ii * 6 ;
   angle=(angle/57.29577951) ; //Convert degrees to radians
   x3=(120+(sin(angle)*(70)));
   y3=(120-(cos(angle)*(70)));
   if (inv) {
     hal->display->drawLine(120,120,x3,y3,BLACK);
   } else {
     hal->display->drawLine(120,120,x3,y3,WHITE);
   }
   //
   // display hour hand
   angle = datetime.hh * 30 + int((datetime.ii / 12) * 6 )   ;
   angle=(angle/57.29577951) ; //Convert degrees to radians
   x3=(120+(sin(angle)*(30)));
   y3=(120-(cos(angle)*(30)));
   if (inv) {
     hal->display->drawLine(120,120,x3,y3,BLACK);
   } else {
     hal->display->drawLine(120,120,x3,y3,0xF000);
   }
  //
}
void WatchFace::renderBattery() {
  int batt = hal->power->getBatteryPercentage();
  hal->display->setTextSize(2);
  hal->display->setCursor(180, 10);
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->print(batt);
  hal->display->print("%");
}
void WatchFace::renderSteps() {
  int steps = 0; // Comes from the HAL, but cant seem to make the accel work
  hal->display->setTextSize(2);
  hal->display->setCursor(0, 220);
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->println("0 Steps");
}


void WatchFace::renderDigit(int posX, int posY, int digit, int prev_val, uint16_t col){
  if (digit != prev_val) {
    drawDigit(posX, posY, prev_val, BGCOLOR);
  }
  drawDigit(posX, posY, digit, col);

}
void WatchFace::drawDigit(int posX, int posY, int digit, uint16_t col){
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
