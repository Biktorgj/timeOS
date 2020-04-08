/*
 * power.cpp: Handle battery reporting functions
*/

#include "Arduino.h"
#include "touch.h"

Touch::Touch() {
  pinMode(TP_RESET, OUTPUT);
  pinMode(TP_INT, INPUT);
  
}
void Touch::init() {
  digitalWrite(TP_RESET, LOW);
  delay(20);
  digitalWrite(TP_RESET, HIGH);
}
