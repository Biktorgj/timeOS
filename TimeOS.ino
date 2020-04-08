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
#include "src/app/app.h"
#include "src/app/debug.h"
#include "src/app/clock.h"

System sys;

//app_settings appSettings;
#define HRSPowerPin 26
#include "HRS3300lib.h"
HRS3300lib HRS3300;



// Initialize classes
PowerMGR PowerMGR;
BMA421 BMA421;
Touch Touch;
Clock Clock;
Debug Debug;
Input Input;

int bmainit_rec = 0x00;

void setupAccel() {
  uint8_t res;
  BMA421.parameter.I2CAddress = 0x18;                  //Choose I2C Address
  res = BMA421.init();
  BMA421.writeConfigFile();
  BMA421.enableSensorFeatures();
}
void setupLCD() {
  tft.init(240, 240);           // Init ST7789 240x240
  tft.setRotation(2);
  tft.fillScreen(BLACK);
  // tft.setFont(&FreeSans9pt7b);

}


void setup(void) {
  // Reset all variables to default
  // Setup devices
  setupVibrator();
  displayOn();
  setupLCD();
  Touch.init();
  // SET TEST MODE FOR HRM
  //  pinMode(30u, INPUT);
  //  digitalWrite(30u, HIGH);

  //  bmainit_rec = setupAccel();
  // setBacklightLevel(5);
  //  setupAccel();

  //flash.begin();

  HRS3300.begin();
  HRS3300.end();


  // Attach interrupts for touchscreen and key
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), buttonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), touchInterrupt, RISING);

  // Test interrupt from the accel / gyro
  // attachInterrupt(digitalPinToInterrupt(BMA400_INTERRUPT), touchInterrupt, RISING);
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
  sys.resetStandbyTime();
  Touch.read();
  if (!sys.getLCDState()) {
    sys.setLCDState(true);
    sys.setCurrentApp(0);
    displayOn();
  } else {
    if (Touch.params.action == 2 && Touch.params.gesture == 2) {
      switchApp(sys.getCurrentApp()+1);
    } else if (Touch.params.action == 2 && Touch.params.gesture == 1) {
      switchApp(sys.getCurrentApp()-1);
    }
  }
}

void drawBattery() {
  unsigned int voltage = PowerMGR.getBatteryVoltage();
  unsigned int percent = PowerMGR.getBatteryPercentage();
  String volts = String(voltage) + "mV" ;
  String perc = String(percent) + "%";
  cleanArea(0, 0, 20, 1);
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println(volts);
  //cleanArea(5, 0, 3, 1);
  tft.setCursor(150, 0);
  tft.setTextColor(GREEN);
  tft.println(perc);
  tft.setTextColor(BLUE);
}

void switchApp(uint8_t appID) {
  bool result = sys.setCurrentApp(appID);
}
/*
void hrmTest() {
cleanArea(0, 1, 5, 8);
tft.println("HRMTest"); // Sensor not touched put finger or wrist on it
if (millis() - refreshTime > 40) {
refreshTime = millis();
uint8_t algo = HRS3300.getHR();
tft.println("Begin!"); // Sensor not touched put finger or wrist on it

switch (algo) {
case 255:
tft.println("Nothing"); // Sensor not touched put finger or wrist on it
// nothing hrs data gets only every 25 calls answered
break;
case 254:
tft.println("NO_TOUCH"); // Sensor not touched put finger or wrist on it
break;
case 253:
tft.println("DATA_TOO_SHORT"); // Not enough data to calculate Heartrate please wait
break;
default:
tft.println(algo); //Got a heartrate print it to Serial
break;
}
}
tft.println("HRMTest END"); // Sensor not touched put finger or wrist on it
}
*/
void loop() {
  // tft.fillScreen(BLACK);
  float x = 0, y = 0, z = 0;
  if (sys.getLCDState()) {
    drawBattery();

    //    BMA421.readData();
    //Touch.read();
    switch (sys.getCurrentApp()) {
      case 0:
      cleanArea(0, 3, 6, 6);
      Clock.drawClock(&tft, &sys);
      break;
      case 1:
      cleanArea(0, 3, 6, 6);
      Debug.drawDebug(&tft, &sys);
      break;
      case 2:
      drawBattery();
      tft.setTextSize(2);
      tft.setCursor(0, 60);
      tft.setTextColor(BLUE);
      cleanArea(0, 1, 5, 8);
      if (PowerMGR.isCharging()) {
        tft.println("Charging");
      } else {
        tft.println("Draining");
      }
      tft.println(BMA421.parameter.raw_acc_x);
      tft.println(BMA421.parameter.raw_acc_y);
      tft.println(BMA421.parameter.raw_acc_z);
      tft.println("Touch:");
      tft.println(Touch.params.x);
      tft.println(Touch.params.y);
      if (Touch.params.action == 2) {
        switch (Touch.params.gesture) {
          case 1:
          tft.println("Swipe Up");
          break;
          case 2:
          tft.println("Swipe Down");
          break;
          case 3:
          tft.println("Swipe Right");
          break;
          case 4:
          tft.println("Swipe Left");
          break;
          default:
          tft.println(Touch.params.action);

          break;
        }
      } else {
        tft.println(Touch.params.gesture);
        tft.println(Touch.params.action);
      }
      break;
      default:
      tft.println("APP ERR");

      break;
    }
    //  hrmTest();
    //   delay(1000);

  } /*else {
    scanI2C(tft);
  }*/
  //tft.drawTriangle(50, 50, 120, 180, 120, 120, PRIMARY_COLOR);
  //  tft.drawCircle(200, 200, 30, PRIMARY_COLOR);

  if (sys.getLCDState()) {
    sys.updateStandbyTime();
  }
  if (sys.isTimeToSleep()) {
    sys.setLCDState(false);
    displayOff();
    __WFI();
  }
  // tft.fillScreen(WHITE);

}

void cleanArea(int xa, int ya, int bl_w, int bl_h) {

  int posX = 0;
  int posY = 0;
  int width = 0;
  int height = 0;
  if (xa > 0) {
    posX = xa * 32; // 32 pixel blocks
  }
  if (ya > 0) {
    posY = ya * 32;
  }
  width = 32 * bl_w;
  height = 32 * bl_h;
  tft.fillRect(posX, posY, width, height, BLACK);

}
