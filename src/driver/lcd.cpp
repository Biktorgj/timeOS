/*
* power.cpp: Handle battery reporting functions
*/
#include "Arduino.h"
#include "gpio.h"
#include "LCD.h"

LCD::LCD() {
  pinMode(TFT_BL_LOW, INPUT);
  pinMode(TFT_BL_MID, INPUT);
  pinMode(TFT_BL_HI, INPUT);
}
void LCD::setBrightness(uint8_t level) {
  pinMode(TFT_BL_LOW, INPUT);
  pinMode(TFT_BL_MID, INPUT);
  pinMode(TFT_BL_HI, INPUT);
  delay(10);
  switch (level) {
    case 0: // Screen off
      break;
    case 1:
      pinMode(TFT_BL_LOW, OUTPUT);
      digitalWrite(TFT_BL_LOW, LOW);
      break;
    case 2:
      pinMode(TFT_BL_MID, OUTPUT);
      delay(10);
      digitalWrite(TFT_BL_MID, LOW);
      break;
    case 3:
      pinMode(TFT_BL_LOW, OUTPUT);
      pinMode(TFT_BL_MID, OUTPUT);
      delay(10);
      digitalWrite(TFT_BL_LOW, LOW);
      digitalWrite(TFT_BL_MID, LOW);
      break;
    case 4:
      pinMode(TFT_BL_HI, OUTPUT);
      digitalWrite(TFT_BL_HI, LOW);
      break;
    case 5:
      pinMode(TFT_BL_LOW, OUTPUT);
      pinMode(TFT_BL_HI, OUTPUT);
      delay(10);
      digitalWrite(TFT_BL_LOW, LOW);
      digitalWrite(TFT_BL_HI, LOW);
      break;
    case 6:
      pinMode(TFT_BL_LOW, OUTPUT);
      pinMode(TFT_BL_MID, OUTPUT);
      pinMode(TFT_BL_HI, OUTPUT);
      delay(10);
      digitalWrite(TFT_BL_LOW, LOW);
      digitalWrite(TFT_BL_MID, LOW);
      digitalWrite(TFT_BL_HI, LOW);
      break;
    default:
      pinMode(TFT_BL_HI, OUTPUT);
      digitalWrite(TFT_BL_HI, HIGH);
      break;
  }
  delay(10);
}

void LCD::saveBrightness(uint8_t val) {
  _savedBrightness = val;
  setBrightness(val);
}

uint8_t LCD::getBrightness() {
  return _savedBrightness;
}
