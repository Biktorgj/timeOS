/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "debug.h"

Debug::Debug() {
  hh = 0;
  mm = 0;
  ss = 0;
}

void Debug::drawDebug(Adafruit_ST7789 tft) {
      tft.setCursor(0, 150);
      tft.println("Debug Mode");
}
