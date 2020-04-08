/*
 * power.cpp: Handle battery reporting functions
*/

#include "Arduino.h"
#include "power.h"

PowerMGR::PowerMGR() {
  pinMode(BATT_VCC, INPUT);
  pinMode(BATT_CHG, INPUT);
  
}
float PowerMGR::getBatteryVoltage() {
  _currentVoltage = (analogRead(BATT_VCC) * 2000) / (1024/3.3);
  _currentPercentage = (((_currentVoltage/1000) - 3.55)*100);
  return _currentVoltage;
}


unsigned int PowerMGR::getBatteryPercentage() {
    return _currentPercentage;

}

bool PowerMGR::isCharging() {
  return !digitalRead(BATT_CHG);
}

bool PowerMGR::gotoSleep() {
  // sleep
}
