/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "clock.h"

Clock::Clock() {
  hh = 0;
  mm = 0;
  ss = 0;
}

void Clock::drawClock(Adafruit_ST7789 tft,int hh, int mm, int ss) {
      tft.setCursor(0, 150);
      //tft.println("00:00:00");
      tft.print(hh);
      tft.print(":");
      tft.print(mm);
      tft.print(":");
      tft.print(ss);
}
