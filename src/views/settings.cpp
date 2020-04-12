/*
* keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "settings.h"
#include "../theme.h"

Settings::Settings(System *System, HAL * Hal) {
  sys = System;
  hal = Hal;
  active_section = 0;
  scroll_position = 0;
}
void Settings::router() {
  TouchEvent thisEvent = sys->getTouchEvent();
  hal->display->setCursor(30, 20);
  hal->display->fillRect(30, 20, 100, 32, BGCOLOR);
  hal->display->print(thisEvent.x);
  hal->display->print(" - ");
  hal->display->print(thisEvent.y);
  switch (active_section) {
    case 0:
      renderMain(thisEvent);
      break;
    case 1:
      renderDisplay(thisEvent);
      break;
    case 2:
      renderBluetooth(thisEvent);
      break;
    case 3:
      renderSensors(thisEvent);
      break;
    case 4:
      renderReset(thisEvent);
      break;
    default:
      active_section = 0; //if you overflow just go back...
      break;
  }
}
uint8_t handleMenuChange() {
  return 0;
}
void Settings::renderMain(TouchEvent event) {

  if (!event.dispatched) {
    if (inRange(50,0,100,240, event.x, event.y)) {
      active_section = 1;
    } else if(inRange(101,0,150,240, event.x, event.y)) {
      active_section = 2;
    } else if(inRange(151,0,200,240, event.x, event.y)) {
      active_section = 3;
    } else if(inRange(200,0,240,240, event.x, event.y)) {
      active_section = 4;
    }
  }
  hal->display->setTextSize(2);
  hal->display->setTextColor(PRIMARY, BGCOLOR);

  hal->display->setCursor(30, 0);
  hal->display->println("Settings");



  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->setCursor(0, 50);
  hal->display->println("[ ] Display");

  hal->display->setCursor(0, 100);
  hal->display->println("[ ] Bluetooth");

  hal->display->setCursor(0, 150);
  hal->display->println("[ ] Sensors");

  hal->display->setCursor(0, 200);
  hal->display->println("[ ] Reset");
}
void Settings::renderDisplay(TouchEvent event) {
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->setCursor(0, 50);
  hal->display->println("[ ] Display SETTINGS");
}
void Settings::renderBluetooth(TouchEvent event) {
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->setCursor(0, 50);
  hal->display->println("[ ] BLUETOOTH SETTINGS");
}
void Settings::renderSensors(TouchEvent event) {
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->setCursor(0, 50);
  hal->display->println("[ ] SENSOR SETTINGS");
}
void Settings::renderReset(TouchEvent event) {
  hal->display->setTextColor(PRIMARY, BGCOLOR);
  hal->display->setCursor(0, 50);
  hal->display->println("[ ] RESET SETTINGS");
 }

bool Settings::inRange(int ymin, int xmin, int ymax, int xmax, int curx, int cury) {
  if (curx > xmin &&
      cury > ymin &&
      curx < xmax &&
      cury < ymax ) {
    return true;
  } else {
    return false;
  }
}
