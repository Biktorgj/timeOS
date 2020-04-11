/*
* hrm.cpp: HRS3300 wrapper
*
* Just a wrapper for
*   https://github.com/atc1441/HRS3300-Arduino-Library
*  All credits to  Aaron Christophel
*  I just wrapped his library here for commodity
*/

#include "Arduino.h"
#include "hrm.h"

HeartRateMonitor::HeartRateMonitor() { }

void HeartRateMonitor::enable() {
  HRS3300.begin();
}

void HeartRateMonitor::disable() {
  HRS3300.end();
}

uint8_t HeartRateMonitor::getHR() {
  return HRS3300.getHR();
}
