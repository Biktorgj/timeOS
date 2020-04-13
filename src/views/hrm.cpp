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

  UIObject currentTimeUI, actionButton, hrmUIObj;
  DateTimeArray  currentTime = sys->getCurrentTime();
  currentTimeUI.digit_0 = currentTime.hh;
  currentTimeUI.digit_1 = currentTime.ii;
  currentTimeUI.digit_2 = currentTime.ss;
  currentTimeUI.front_color = 0x0CC0;
  sys->ui->renderTopLeft(true, currentTimeUI);
  sys->ui->renderTopRight(false, currentTimeUI);

  hal->display->setTextSize(2);
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->setCursor(0, 40);
  hal->display->println("HRM");
  if (!_reading) {
    actionButton.front_color = WHITE;
    actionButton.bg_color = PRIMARY;
    actionButton.text = "START";
    sys->ui->renderBottomFullButton(true, actionButton);
    if (!thisEvent.dispatched && thisEvent.y >= 180) {
      _last_begin = millis();
      hal->hrm->enable();
      _reading = true;
    }
  } else {
    actionButton.front_color = WHITE;
    actionButton.bg_color = DANGER;
    actionButton.text = "STOP";
    sys->ui->renderBottomFullButton(true, actionButton);
    if (millis() - _lastrefresh > 40) {
      _lastrefresh = millis();
      _lastread = hal->hrm->getHR();
      hal->display->setTextColor(PRIMARY, BGCOLOR);
      hal->display->setCursor(40, 160);
      if (_lastread < 253) {
        hrmUIObj.digit_0 = _lastread / 100;
        hrmUIObj.digit_1 = _lastread % 100;
        hrmUIObj.front_color = SUCCESS;
      } else {
        hrmUIObj.digit_0 = 0;
        hrmUIObj.digit_1 = 0;
        hrmUIObj.front_color = WARNING;
      }
      sys->ui->renderMainBlockNumberInt(false, hrmUIObj);
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
        hal->display->println(" BPM         "); //Got a heartrate print it to Serial
        hal->hrm->disable();
        _reading = false;
        break;
      }
    }
    if (!thisEvent.dispatched && thisEvent.y >= 180) {
      hal->display->setTextColor(PRIMARY, BGCOLOR);
      hal->display->setCursor(40, 140);
      hal->display->println("Stopped!       "); // Not enough data to calculate Heartrate please wait
      hal->hrm->disable();
      _reading = false;
    }
  }
  if (millis() - _last_begin > 60000) {
    hal->hrm->disable();
    _reading = false;
    hal->display->setTextColor(SECONDARY, BGCOLOR);
    hal->display->setCursor(40, 140);
    hal->display->println("Timed out!");
  }
}

void HeartRate::resetSensorLib() {
  hal->hrm->enable();
  hal->hrm->disable();
}
