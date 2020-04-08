#include <String.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)

#include <SPI.h> // SPI
#include <Wire.h> // I2C




//#include "i2c.h"
#include "display.h"
#include "touch.h"
#include "keys.h"
#include "power.h"
#include "vibra.h"
#include "clock.h"
#include "bma421.h"


#define HRSPowerPin 26
#include "HRS3300lib.h"
HRS3300lib HRS3300;

int refreshTime;
//#include <RTC.h>

PowerMGR PowerMGR;
BMA421 BMA421;
Touch Touch;
Clock Clock;
Input Input;
float p = 3.1415926;
unsigned int current = 0;
int tMode = 0;
int mode = 0; // 0: suspend, 1: debug, 2: clock
int timedout = 0;
int secs = 0;
unsigned long startTime;
unsigned long endTime;
int bmainit_rec = 0x00;
/*
   Delays:
*/
const unsigned long DELAY_TIME = 1000; // mS == 1sec


void setupGPIO() {
  pinMode(TFT_BL_LOW, OUTPUT);
  pinMode(TFT_BL_MID, OUTPUT);
  pinMode(TFT_BL_HI, OUTPUT);
  delay(10);
  digitalWrite(TFT_BL_LOW, LOW);
  digitalWrite(TFT_BL_MID, LOW);
  digitalWrite(TFT_BL_HI, LOW);
}
void setupAccel() {
  uint8_t res;
  BMA421.parameter.I2CAddress = 0x18;                  //Choose I2C Address
  res = BMA421.init();
  BMA421.writeConfigFile();
  BMA421.enableSensorFeatures();
}
void setupLCD() {
  tft.init(240, 240);           // Init ST7789 240x240
  tft.fillScreen(BLACK);
  // tft.setFont(&FreeSans9pt7b);

}

void setup(void) {
  //  bmainit_rec = setupAccel();
  startTime = millis();
  endTime = millis();
  timedout = 0;
  setupVibrator();
  setupGPIO();
  setBacklightLevel(0);
  setupLCD();

  Touch.init();
  setupAccel();
  // SET TEST MODE FOR HRM
  pinMode(30u, INPUT);
  //  digitalWrite(30u, HIGH);
  refreshTime = millis();

  //flash.begin();
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), handleInputButton, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), handleTouchWake, RISING);
  // Test interrupt from the accel / gyro
 // attachInterrupt(digitalPinToInterrupt(BMA400_INTERRUPT), handleTouchWake, RISING);
 // HRS3300.begin();
}
void handleInputButton() {
  tMode = !tMode;
  startTime = millis();
  endTime = millis();
  if (timedout) {
    timedout = 0;
    displayOn();
  } else {
    timedout = 1;
    displayOff();
  }
  delay(100);
}
void handleTouchWake() {
  startTime = millis();
  endTime = millis();
  Touch.read();

  if (timedout) {
 
    timedout = 0;
    displayOn();
  }
}

void drawBattery() {
  unsigned int voltage = PowerMGR.getBatteryVoltage();
  unsigned int percent = PowerMGR.getBatteryPercentage();
  String volts = String(voltage) + "mV" ;
  String perc = String(percent) + "%";
  cleanArea(0, 0, 3, 1);
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println(volts);
  cleanArea(5, 0, 3, 1);
  tft.setCursor(180, 0);
  tft.setTextColor(RED);
  tft.println(perc);
}

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

void loop() {
  // tft.fillScreen(BLACK);

  float x = 0, y = 0, z = 0;
  if (!timedout) {
    drawBattery();
    tft.setTextSize(2);
    tft.setCursor(0, 60);
    tft.setTextColor(BLUE);
    cleanArea(0, 1, 5, 8);
    BMA421.readData();
    //Touch.read();

    if (PowerMGR.isCharging()) {
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

    } else {
      // tft.println("NOT Charging");
    }

  //  hrmTest();
 //   delay(1000);

  } /*else {
    scanI2C(tft);
  }*/
  //tft.drawTriangle(50, 50, 120, 180, 120, 120, PRIMARY_COLOR);
  //  tft.drawCircle(200, 200, 30, PRIMARY_COLOR);
  if (!timedout) {
    endTime = millis();
  }
  if ((endTime - startTime) >= 40000) {
    timedout = 1;
    displayOff();
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
