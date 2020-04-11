/*
 * touch.cpp: Handle touchscreen reset, wakeup, suspend and report
*/

#include "Arduino.h"
#include "touch.h"
#include <Wire.h>

/*
 * ACTIONS:
 *  0: Touch, Gesture 5: Touch down
 *  2: Gesture
 *      -> UP: 1
 *      -> DOWN: 2
 *      -> Swipe right: 3
 *      -> Swipe left: 4
 */
Touch::Touch() {
  pinMode(TP_RESET, OUTPUT);
  pinMode(TP_INT, INPUT);
}
void Touch::init() {
  digitalWrite(TP_RESET, LOW);
  delay(20);
  digitalWrite(TP_RESET, HIGH);
}

void Touch::suspend() {
 // Not yet
}
void Touch::wake() {
  // Not yet
}
void Touch::read() {
  int i;
  uint8_t buf[9]; /* 3 bytes "header" and 6 bytes touch info */
  Wire.requestFrom(TP_I2C_ADDRESS,9);
  for (i = 0; i < 9; i++) {
    buf[i] = Wire.read();
  }
   params.gesture = buf[1];
   params.action = buf[3] >> 6;
   params.x = (buf[3] & 0x0f) << 8 | buf[4];
   params.y = (buf[5] & 0x0f) << 8 | buf[6];
   params.dispatched = false;
}

void Touch::writeByte(byte reg, byte value) {
  Wire.beginTransmission(TP_I2C_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}
uint8_t Touch::readByte(byte reg) {
  uint8_t value;
  Wire.beginTransmission(TP_I2C_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(TP_I2C_ADDRESS,1);
  value = Wire.read();
  return value;
}
