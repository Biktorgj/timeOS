#include <String.h> 

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)

#include <SPI.h> // SPI
#include <Wire.h> // I2C



#include "i2c.h"
#include "display.h"
#include "touch.h"
#include "keys.h"
#include "power.h"
#include "vibra.h"
#include "clock.h"


//#include <RTC.h>

PowerMGR PowerMGR;
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
void setupLCD() {
  tft.init(240, 240);           // Init ST7789 240x240
  tft.fillScreen(BLACK);
  // tft.setFont(&FreeSans9pt7b);

}

void setup(void) {
  bmainit_rec = setupAccel();
  startTime = millis();
  endTime = millis();
  timedout = 0;
   setupVibrator();
  setupGPIO();
  setBacklightLevel(0);
  setupLCD();
  Touch.init();
  delay(200);


  //flash.begin();
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), handleInputButton, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), handleTouchWake, FALLING);
  // Test interrupt from the accel / gyro
   attachInterrupt(digitalPinToInterrupt(BMA400_INTERRUPT), handleTouchWake, RISING);

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
  if (timedout) {
    startTime = millis();
    endTime = millis();
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

void loop() {
  
  if (!timedout) {
    drawBattery();
    tft.setCursor(0, 60);
    tft.setTextColor(BLUE);
    cleanArea(0, 1, 7, 3);
    if (PowerMGR.isCharging()) {
      tft.println(getX());
      tft.println(getY());
      tft.println(getZ());
      tft.println(bmainit_rec, HEX);
    } else {
      tft.println("NOT Charging");
    }
  } /*else {
    scanI2C(tft);
  }*/
  //tft.drawTriangle(50, 50, 120, 180, 120, 120, PRIMARY_COLOR);
  //  tft.drawCircle(200, 200, 30, PRIMARY_COLOR);
  if (!timedout) {
      endTime = millis();
  }
  if ((endTime - startTime) >= 20000) {
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
