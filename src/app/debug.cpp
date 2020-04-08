/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "debug.h"

Debug::Debug() {

}

void Debug::drawDebug(Adafruit_ST7789 *tft, System *sys) {
  DateTimeArray datetime = sys->getCurrentTime();
      tft->setCursor(60, 100);
      tft->setTextSize(1);
      tft->setTextColor(0xF309);
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

      tft->println("--Debug Mode--");
}
