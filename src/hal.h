/* HAL */

#ifndef _HAL_
#define _HAL_
#include "Arduino.h"
#include <SPI.h>
#include <BLEPeripheral.h>
#include <Wire.h> // I2C
#include <String.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)
// SPI Flash

#include "driver/gpio.h"
#include "driver/lcd.h"
#include "driver/BLESerial.h"
#include "driver/power.h"
#include "driver/vibra.h"
#include "driver/touch.h"
#include "driver/key.h"
#include "driver/hrm.h"
#include "driver/arduino_bma421.h"


struct AccelRaw {
  float x;
  float y;
  float z;
  uint32_t temp;
  uint32_t steps;

};
class HAL
{
  public:
    HAL();
    void init();
    void forward();
    void pollBLE();
    void loopback();
    void updateAccel();
    void spam();
    AccelRaw accelData;
    Vibra *vibra;
    Adafruit_ST7789 *display;
    BLESerial *bluetooth;
    Key *key;
    Touch *touch;
    Power *power;
    HeartRateMonitor *hrm;
    LCD *lcd;
  private:
    void setupInterrupt();
};

#endif
