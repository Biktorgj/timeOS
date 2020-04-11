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

#include "driver/gpio.h"
#include "driver/lcd.h"
#include "driver/BLESerial.h"
#include "driver/power.h"
#include "driver/vibra.h"
#include "driver/touch.h"
#include "driver/key.h"
#include "driver/hrm.h"

class HAL
{
  public:
    HAL();
    void init();
    void forward();
    void pollBLE();
    void loopback();
    void spam();
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
