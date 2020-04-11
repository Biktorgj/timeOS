/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "clock.h"
#include "../theme.h"
Clock::Clock(System *System, HAL * Hal) {
  curPath = 0; // Default path is 0 (show clock), 1 would be watchface select
  sys = System;
  hal = Hal;
  prevTime = sys->getCurrentTime();

}

void Clock::route() {
  switch (curPath) {
    case 0:
      renderClockView();
      break;
    default:
      break;
  }
}

void Clock::renderClockView(void) {
  DateTimeArray  currentTime = sys->getCurrentTime();

   renderDigitalClock(currentTime);
   renderAnalogClock(true, prevTime);
   renderAnalogClock(false, currentTime);

   prevTime = currentTime;
   renderSteps();
   renderBattery();
}

void Clock::renderDigitalClock(DateTimeArray datetime) {
    hal->display->setTextSize(2);

      hal->display->setCursor(60, 20);
      hal->display->setTextColor(0xFFFF, 0x0000);

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
      hal->display->setCursor(60, 140);
}
void Clock::renderAnalogClock(bool inv, DateTimeArray datetime) {
  int z,x2,x3,y2,y3;
  float angle;
  // Size total: 240, 240,
  // Size to render: 20 - 180
  // Now draw the clock face
   hal->display->drawCircle(120, 120, 80, WHITE);
   hal->display->drawCircle(120, 120, 2, 0x00CF);
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
void Clock::renderBattery() {
  int batt = hal->power->getBatteryPercentage();
  hal->display->setTextSize(2);
  hal->display->setCursor(180, 0);
  hal->display->setTextColor(0x0CF0, 0x0000);
  hal->display->print(batt);
  hal->display->print("%");
}
void Clock::renderSteps() {
  int steps = 0; // Comes from the HAL, but cant seem to make the accel work
  hal->display->setTextSize(2);
  hal->display->setCursor(0, 220);
  hal->display->setTextColor(0x0CF0, 0x0000);
  hal->display->println("0 Steps");
}
