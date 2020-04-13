/*
 * syste.cpp: Handle battery reporting functions
*/

#include "Arduino.h"
#include "system.h"

UI Ui;

typedef struct rtctimerdata {
  int hh;
  int mm;
  int ss;
};
rtctimerdata curtime;

System::System(HAL *Hal) {
  hal = Hal;
  Ui.attHal(Hal);
  ui = &Ui;
}
void System::notifyClockTchange(int hh, int mm, int ss) {
  timestorage.hh = hh;
  timestorage.ii = mm;
  timestorage.ss = ss;
}
DateTimeArray System::getCurrentTime() {
  return timestorage;
}
uint8_t System::getCurrentApp() {
  return current_app;
}
uint8_t System::getPreviousApp() {
  return previous_app;
}
void System::reportAppChanged() {
  previous_app = current_app;
  ui->restoreObjects();

}
bool System::isAppChanged() {
  if (previous_app == current_app) {
      return false;
  }
  return true;
}

bool System::setCurrentApp(uint8_t appID) {
  if (appID >= 0 && appID < available_apps &&
      appID != current_app) {
    previous_app = current_app;
    current_app = appID;
    return false; // success
  }

  return true; // failed
}
bool System::isPendingTouchEvent() {
  return !touch_event.dispatched;
}
TouchEvent System::getTouchEvent() {
  TouchEvent out = touch_event;
  touch_event.dispatched = true;
  return out;
}
void System::notifyTouchEvent(TouchEvent thisEvent) {
  touch_event = thisEvent;
}


void System::resetStandbyTime() {
  display_start_time = millis();
  display_end_time = millis();
}
void System::updateStandbyTime() {
  display_end_time = millis();
}
bool System::getLCDState() {
  return lcd_state;
}
void System::setLCDState(bool state) {
  lcd_state = state;
}
bool System::isTimeToSleep() {
  if (display_end_time - display_start_time >= (lcd_standby_seconds * 1000)) {
    return true;
  }
  return false;
}
void System::resetState() {
  hal->display->println("[SYS]: Ready");
  display_start_time = millis();
  display_end_time = millis();
  prevTick = millis();
  previous_app = 0;
  current_app = 0;
  available_apps = 4;
  lcd_state = true;

  timestorage.hh = 0;
  timestorage.ii = 0;
  timestorage.ss = 0;
  timestorage.dd = 1;
  timestorage.mm = 1;
  timestorage.yy = 1970;
  lcd_standby_seconds = 20;
}
