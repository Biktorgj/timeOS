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
#include "src/driver/rtc.h"
// Application code
#include "src/app/runtime.h"
#include "src/app/app.h"

#include "src/app/debug.h"
#include "src/app/clock.h"

//app_settings appSettings;
#define HRSPowerPin 26
#include "HRS3300lib.h"
HRS3300lib HRS3300;

#include <nrf.h>

#define RTC NRF_RTC0
#define RTC_IRQ RTC0_IRQn
int interrupt = 0;

int refreshTime;
// Initialize classes
PowerMGR PowerMGR;
BMA421 BMA421;
Touch Touch;
Clock Clock;
Debug Debug;
Input Input;

unsigned int current = 0;
int tMode = 0;
int mode = 0; // 0: suspend, 1: debug, 2: clock
int timedout = 0;
int secs = 0;

int bmainit_rec = 0x00;
app_runtime appRuntime;
/*
Delays:
*/
const unsigned long DELAY_TIME = 1000; // mS == 1sec

static unsigned irq_counter = 0;
boolean LD = false;


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
void setAppDefault() {
  appRuntime.display_start_time = millis();
  appRuntime.display_end_time = millis();
  appRuntime.current_app = 0;
  appRuntime.hh = 0;
  appRuntime.mm = 0;
  appRuntime.ss = 0;

}
void setup(void) {
  //  bmainit_rec = setupAccel();
  appRuntime.prevTick = millis();
  displayOn();
  timedout = 0;
  setupVibrator();
  // setBacklightLevel(5);
  setupLCD();

  Touch.init();
  //  setupAccel();
  // SET TEST MODE FOR HRM
  pinMode(30u, INPUT);
  //  digitalWrite(30u, HIGH);
  refreshTime = millis();

  //flash.begin();
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), buttonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), touchInterrupt, RISING);
  // Test interrupt from the accel / gyro
  // attachInterrupt(digitalPinToInterrupt(BMA400_INTERRUPT), touchInterrupt, RISING);
  HRS3300.begin();
  HRS3300.end();

  /* RTC */
  // Configure RTC
  RTC->TASKS_STOP = 1;
  RTC->PRESCALER = 31; //1024Hz frequency
  RTC->CC[0] = RTC->COUNTER + (1 * 1024);
  RTC->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
  RTC->INTENSET = RTC_INTENSET_COMPARE0_Msk;
  RTC->TASKS_START = 1;
  RTC->EVENTS_COMPARE[0] = 0;

  // Enable interrupt
  NVIC_SetPriority(RTC_IRQ, 15);
  NVIC_ClearPendingIRQ(RTC_IRQ);
  NVIC_EnableIRQ(RTC_IRQ);


}
void buttonInterrupt() {
  tMode = !tMode;
  appRuntime.display_start_time = millis();
  appRuntime.display_end_time = millis();
  if (timedout) {
    timedout = 0;
    displayOn();
  } else {
    timedout = 1;
    displayOff();
  }
  delay(200);
}
void touchInterrupt() {
  appRuntime.display_start_time = millis();
  appRuntime.display_end_time = millis();
  Touch.read();

  if (timedout) {
    timedout = 0;
    appRuntime.current_app = 0;
    displayOn();
  } else {
    if (Touch.params.action == 2 && Touch.params.gesture == 2) {
      switchApp(true);
    } else if (Touch.params.action == 2 && Touch.params.gesture == 1) {
      switchApp(false);
    }
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

void switchApp(bool next) {
  if (next && appRuntime.current_app < 2) {
    appRuntime.current_app++;
  } else if (next) {
    appRuntime.current_app = 0;
  } else if (!next && appRuntime.current_app > 0) {
    appRuntime.current_app--;
  } else {
    appRuntime.current_app = 0;
  }
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
  if (!timedout) {

    //    BMA421.readData();
    //Touch.read();
    switch (appRuntime.current_app) {
      case 0:
      cleanArea(0, 0, 6, 6);
      Clock.drawClock(tft, appRuntime.hh, appRuntime.mm, appRuntime.ss);
      break;
      case 1:
      cleanArea(0, 0, 6, 6);
      Debug.drawDebug(tft);
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
      break;
    }
    //  hrmTest();
    //   delay(1000);

  } /*else {
    scanI2C(tft);
  }*/
  //tft.drawTriangle(50, 50, 120, 180, 120, 120, PRIMARY_COLOR);
  //  tft.drawCircle(200, 200, 30, PRIMARY_COLOR);

  if (!timedout) {
    appRuntime.display_end_time = millis();
  }
  if ((appRuntime.display_end_time - appRuntime.display_start_time) >= 40000) {
    timedout = 1;
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


/**
* Reset events and read back on nRF52
* http://infocenter.nordicsemi.com/pdf/nRF52_Series_Migration_v1.0.pdf
*/
#if __CORTEX_M == 0x04
#define NRF5_RESET_EVENT(event)                                                 \
event = 0;                                                                   \
(void)event
#else
#define NRF5_RESET_EVENT(event) event = 0
#endif

// This must be in one line
extern "C" {
  void RTC0_IRQHandler(void) {
    NRF5_RESET_EVENT(RTC->EVENTS_COMPARE[0]);
    //unsigned long currentTick = millis();
    //if (currentTick - appRuntime.prevTick >= 500) {
    //  appRuntime.prevTick = currentTick;
      appRuntime.ss++;
      if (appRuntime.ss >= 60) {
        appRuntime.mm++;
        appRuntime.ss = 0;
        if (appRuntime.mm >= 60) {
          appRuntime.mm = 0;
          appRuntime.hh++;
        }
      }
    //}
    RTC->TASKS_CLEAR = 1;
  }
}
