/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "keys.h"

Input::Input() {
  pinMode(SIDE_BTN_OUT, OUTPUT);
  digitalWrite(SIDE_BTN_OUT, HIGH);
  pinMode(SIDE_BTN_IN, INPUT);
  // Setup interrupt and reporting
}

void Input::setupInterrupt() {
  
}
