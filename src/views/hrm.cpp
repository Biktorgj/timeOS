/*
* keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "hrm.h"
#include "../theme.h"

HeartRate::HeartRate(System *System, HAL * Hal) {
  sys = System;
  hal = Hal;
  _reading = false;
  _lastread = 255;
  _lastrefresh = millis();
}

void HeartRate::render() {
  TouchEvent thisEvent = sys->getTouchEvent();
  hal->display->setTextSize(2);
  hal->display->setTextColor(0x00CF, 0x0000);
  hal->display->setCursor(0, 40);
  hal->display->println("Heartrate Monitor");

  hal->display->fillRect(0, 200, 240, 240, 0xC000);
  if (!_reading) {
    hal->display->setTextSize(2);
    hal->display->setTextColor(0xFFFF, 0xC000);
    hal->display->setCursor(30, 210);
    hal->display->println("Start reading");
    if (!thisEvent.dispatched && thisEvent.y >= 180) {
      _last_begin = millis();
      hal->hrm->enable();
      _reading = true;
    }
  } else {
    if (millis() - _lastrefresh > 40) {
      _lastrefresh = millis();
      _lastread = hal->hrm->getHR();
      hal->display->setTextColor(0x00CF, 0x0000);
      hal->display->setCursor(40, 140);
      switch (_lastread) {
        case 255:
        hal->display->println("Reading...");
        // nothing hrs data gets only every 25 calls answered
        break;
        case 254:
        hal->display->println("No pulse"); // Sensor not touched put finger or wrist on it
        break;
        case 253:
        hal->display->println("Waiting.."); // Not enough data to calculate Heartrate please wait
        break;
        default:
        hal->display->print(_lastread); //Got a heartrate print it to Serial
        hal->display->println(" BPM"); //Got a heartrate print it to Serial
        hal->hrm->disable();
        _reading = false;
        break;
      }
    }
    hal->display->setTextSize(2);
    hal->display->setTextColor(0xFFFF, 0xC000);
    hal->display->setCursor(30, 210);
    hal->display->println("Stop reading");
    if (!thisEvent.dispatched && thisEvent.y >= 180) {
      hal->display->setTextColor(0x00CF, 0x0000);
      hal->display->setCursor(40, 140);
      hal->display->println("Stopped!       "); // Not enough data to calculate Heartrate please wait
      hal->hrm->disable();
      _reading = false;
    }
  }
  if (millis() - _last_begin > 60000) {
    hal->hrm->disable();
    _reading = false;
    hal->display->setTextColor(0xC000, 0x0000);
    hal->display->setCursor(40, 140);
    hal->display->println("Timed out!");
  }
}

void HeartRate::resetSensorLib() {
  hal->hrm->enable();
  hal->hrm->disable();
}
