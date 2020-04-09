/*
* keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "hrm.h"
HeartRate::HeartRate() {
  _reading = false;
  _lastread = 255;
  _lastrefresh = millis();
}

void HeartRate::render(Adafruit_ST7789 *tft, System *sys) {
  TouchEvent thisEvent = sys->getTouchEvent();
  tft->setTextSize(2);
  tft->setTextColor(0x00CF, 0x0000);
  tft->setCursor(0, 40);
  tft->println("Heartrate Monitor");

  tft->fillRect(0, 200, 240, 240, 0xC000);
  if (!_reading) {
    tft->setTextSize(2);
    tft->setTextColor(0xFFFF, 0xC000);
    tft->setCursor(30, 210);
    tft->println("Start reading");
    if (!thisEvent.dispatched && thisEvent.y >= 180) {
      _last_begin = millis();
      HRS3300.begin();
      _reading = true;
    }
  } else {
    if (millis() - _lastrefresh > 40) {
      _lastrefresh = millis();
      _lastread = HRS3300.getHR();
      tft->setTextColor(0x00CF, 0x0000);
      tft->setCursor(40, 140);
      switch (_lastread) {
        case 255:
        tft->println("Reading...");
        // nothing hrs data gets only every 25 calls answered
        break;
        case 254:
        tft->println("No pulse"); // Sensor not touched put finger or wrist on it
        break;
        case 253:
        tft->println("Waiting.."); // Not enough data to calculate Heartrate please wait
        break;
        default:
        tft->print(_lastread); //Got a heartrate print it to Serial
        tft->println(" BPM"); //Got a heartrate print it to Serial
        HRS3300.end();
        _reading = false;
        break;
      }
    }
    tft->setTextSize(2);
    tft->setTextColor(0xFFFF, 0xC000);
    tft->setCursor(30, 210);
    tft->println("Stop reading");
    if (!thisEvent.dispatched && thisEvent.y >= 180) {
      HRS3300.end();
      _reading = false;
    }
  }
  if (millis() - _last_begin > 60000) {
    HRS3300.end();
    _reading = false;
    tft->setTextColor(0xC000, 0x0000);
    tft->setCursor(40, 140);
    tft->println("Timed out!");
  }
}
