/*
 * power.cpp: Handle battery reporting functions
*/
#include "Arduino.h"
#include "gpio.h"
#include "power.h"

Power::Power() {
  pinMode(BATT_VCC, INPUT);
  pinMode(BATT_CHG, INPUT);

}
float Power::getBatteryVoltage() {
  _currentVoltage = (analogRead(BATT_VCC) * 2000) / (1024/3.3);
  _currentPercentage = map(round(_currentVoltage/10), 342, 450, 0,100);//(((_currentVoltage/1000) - 3.55)*100);
  return _currentVoltage;
}


unsigned int Power::getBatteryPercentage() {
  getBatteryVoltage();
  return _currentPercentage;

}

bool Power::isCharging() {
  return !digitalRead(BATT_CHG);
}

void Power::suspend() {
  // sleep
}
