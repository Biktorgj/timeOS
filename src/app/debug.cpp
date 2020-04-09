/*
* keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "debug.h"

Debug::Debug() {

}

void Debug::drawDebug(Adafruit_ST7789 *tft, System *sys) {
  DateTimeArray datetime = sys->getCurrentTime();
  TouchEvent thisEvent = sys->getTouchEvent();
  tft->setTextColor(0xF309, 0x0000);
  tft->setTextSize(1);
  tft->setCursor(0, 40);
  tft->print("Touch: X: ");
  tft->println(thisEvent.x);
  tft->print("Touch: Y: ");
  tft->println(thisEvent.y);
  tft->print("Touch: Act: ");
  tft->println(thisEvent.action);
  tft->print("Touch: Gest: ");
  tft->println(thisEvent.gesture);
  if (thisEvent.action == 2) {
    switch (thisEvent.gesture) {
      case 1:
      tft->println("Swipe Up");
      break;
      case 2:
      tft->println("Swipe Down");
      break;
      case 3:
      tft->println("Swipe Right");
      break;
      case 4:
      tft->println("Swipe Left");
      break;
      default:
      tft->print("Unknown action: ");
      tft->println(thisEvent.action);

      break;
    }
  } else {
    tft->println(thisEvent.gesture);
    tft->println(thisEvent.action);
  }
  tft->setCursor(0, 100);
  tft->println("Current time from RTC:");
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
}
