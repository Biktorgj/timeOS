#include <String.h>
// Drivers for SPI & I2C
#include <SPI.h> // SPI
#include <Wire.h> // I2C

// External Libraries
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)

// Pinetime drivers
#include "src/driver/display.h"
#include "src/driver/touch.h"
#include "src/driver/keys.h"
#include "src/driver/power.h"
#include "src/driver/vibra.h"
#include "src/driver/bma421.h"

// Application code
#include "src/app/system.h"
#include "src/app/debug.h"
#include "src/app/clock.h"
#include "src/app/main_menu.h"
#include "src/app/hrm.h"
System sys;

// Initialize classes
PowerMGR PowerMGR;
BMA421 BMA421;
Touch Touch;
Clock Clock;
Debug Debug;
Input Input;
HeartRate HeartRate;
MainMenu MainMenu;

void setupAccel() {
  uint8_t res;
  BMA421.parameter.I2CAddress = 0x18; //Choose I2C Address
  res = BMA421.init();
  BMA421.writeConfigFile();
  BMA421.enableSensorFeatures();
}

void setup(void) {
  // Reset all variables to default
  sys.resetState(&tft);
  // Setup devices
  setupVibrator();
  displayOn();
  sys.resetLCD();
  Touch.init();
  HeartRate.resetSensorLib();
  // Attach interrupts for touchscreen and key
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), buttonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), touchInterrupt, RISING);
}
void buttonInterrupt() {
  sys.resetStandbyTime();
  if (!sys.getLCDState()) {
    sys.setLCDState(true);
    sys.setCurrentApp(0);
    displayOn();
  } else {
    sys.setLCDState(false);
    displayOff();
  }
  delay(200);
}


void touchInterrupt() {
  bool result;
  TouchEvent thisEvent;
  sys.resetStandbyTime();
  Touch.read();
  thisEvent.x = Touch.params.x;
  thisEvent.y = Touch.params.y;
  thisEvent.action = Touch.params.action;
  thisEvent.gesture = Touch.params.gesture;
  thisEvent.dispatched = false;
  if (!sys.getLCDState()) {
    sys.setLCDState(true);
    sys.setCurrentApp(0);
    displayOn();
  } else {
    if (Touch.params.action == 2 && Touch.params.gesture == 3) {
      result = sys.setCurrentApp(sys.getCurrentApp()+1);
      thisEvent.dispatched = true;
    } else if (Touch.params.action == 2 && Touch.params.gesture == 4) {
      result = sys.setCurrentApp(sys.getCurrentApp()-1);
      thisEvent.dispatched = true;
    }
  }
  sys.notifyTouchEvent(thisEvent);
}

void drawBattery() {
  unsigned int voltage = PowerMGR.getBatteryVoltage();
  unsigned int percent = PowerMGR.getBatteryPercentage();
  String volts = String(voltage) + "mV" ;
  String perc = String(percent) + "%";
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLACK);
  tft.println(volts);
  tft.setCursor(150, 0);
  tft.setTextColor(GREEN, BLACK);
  tft.println(perc);
  tft.setTextColor(BLUE, BLACK);
}

void loop() {
  if (sys.isAppChanged()) {
        tft.fillScreen(BLACK);
        sys.reportAppChanged();
  }
  if (sys.getLCDState()) {
    switch (sys.getCurrentApp()) {
      case 0:
      drawBattery();
      Clock.drawClock(&tft, &sys);
      break;
      case 1:
      drawBattery();
      HeartRate.render(&tft, &sys);
      break;
      case 2:
      drawBattery();
      MainMenu.render(&tft, &sys);
      break;
      case 3:
      drawBattery();
      Debug.drawDebug(&tft, &sys);
      break;
      case 4:
      drawBattery();
      tft.setTextSize(2);
      tft.setCursor(0, 60);
      tft.setTextColor(BLUE, BLACK);
      if (PowerMGR.isCharging()) {
        tft.println("Charging");
      } else {
        tft.println("Discharging");
      }
      tft.println("Accel:");
      tft.print("X: ");
      tft.print(BMA421.parameter.raw_acc_x);
      tft.print(" Y: ");
      tft.print(BMA421.parameter.raw_acc_y);
      tft.print(" Z: ");
      tft.print(BMA421.parameter.raw_acc_z);


      break;
      default:
      tft.println("APP ERR");
      break;
    }
  }
  if (sys.getLCDState()) {
    sys.updateStandbyTime();
  }
  if (sys.isTimeToSleep()) {
    sys.setLCDState(false);
    displayOff();
    __WFI();
  }
} // end of loop
