/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "clock.h"

Clock::Clock() {

}

void Clock::drawClock(Adafruit_ST7789 *tft,System *sys) {
  DateTimeArray datetime = sys->getCurrentTime();
    tft->setTextSize(2);

      tft->setCursor(60, 100);
      tft->setTextColor(0xFFFF, 0x0000);

      // Date
      if (datetime.dd < 10) {
        tft->print("0");
      }
      tft->print(datetime.dd);
      tft->print("/");
      if (datetime.mm < 10) {
        tft->print("0");
      }
      tft->print(datetime.mm);
      tft->print("/");
      if (datetime.yy < 10) {
        tft->print("000");
      }
      tft->println(datetime.yy);

      // Time
      tft->setCursor(60, 120);
      if (datetime.hh < 10) {
        tft->print("0");
      }
      tft->print(datetime.hh);
      tft->print(":");
      if (datetime.ii < 10) {
        tft->print("0");
      }
      tft->print(datetime.ii);
      tft->print(":");
      if (datetime.ss < 10) {
        tft->print("0");
      }
      tft->println(datetime.ss);
      tft->setCursor(60, 140);

      tft->println("CLOCK");
}
