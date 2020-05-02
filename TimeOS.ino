#include <nrf.h>

/*
*  RTC0 is used by Softdevice for bluetooth handling
*  RTC1 is used by the Arduino core for delay()
*  So we try to use RTC2 as it seems to be free, and it works :)
*/
#define RTC NRF_RTC2
#define RTC_IRQ RTC2_IRQn

#include "src/hal.h" // Include the HAL
#include "src/system.h" // Include system
#include "src/theme.h" // Include the base theme

// Languages here
#include "src/views/watchface.h"
#include "src/views/stopwatch.h"
#include "src/views/hrm.h"
#include "src/views/settings.h"

typedef struct RTCTimerData {
  int hh;
  int mm;
  int ss;
};
RTCTimerData currentTime;


HAL hal;
System sys(&hal);
WatchFace watchface(&sys, &hal);
Stopwatch stopwatch(&sys, &hal);
HeartRate heartrate(&sys, &hal);
Settings settings(&sys, &hal);
bool clockBooted = false;
bool alreadyRefreshing = false;

#ifdef MEAUSARE_FRAMERATE
uint16_t beginRefresh;
uint16_t endRefresh;
#endif
void startRTC() {
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

void setup() {
  hal.init();

  sys.resetState();
  hal.display->fillScreen(BGCOLOR); // Paint it PRIMARY
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), buttonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), touchInterrupt, RISING);
  clockBooted = true;
  startRTC(); // If I start the RTC bluetooth goes to trash

}

void loop() {
  hal.bluetooth->poll();
  hal.forward();
  handleCurrentView();
  //  watchface.route();
  // loopback();
  // spam();
  if (sys.isTimeToSleep()) {
    sys.setLCDState(false);
    hal.lcd->setBrightness(0);
    // hal.suspendDevices();
    __WFI();
  }
}

void buttonInterrupt() {
  sys.resetStandbyTime();
  if (!sys.getLCDState()) {
    sys.setLCDState(true);
    sys.setCurrentApp(0);
    handleCurrentView();
    hal.lcd->setBrightness(3);
  } else {
    sys.setLCDState(false);
    hal.lcd->setBrightness(0);
  }
  delay(200);
}


void touchInterrupt() {
  bool result;
  TouchEvent thisEvent;
  hal.touch->read();
  thisEvent.x = hal.touch->params.x;
  thisEvent.y = hal.touch->params.y;
  thisEvent.action = hal.touch->params.action;
  thisEvent.gesture = hal.touch->params.gesture;
  thisEvent.dispatched = false;
  sys.resetStandbyTime();
  if (!sys.getLCDState()) {
    sys.setLCDState(true);
    sys.setCurrentApp(0);
    hal.lcd->setBrightness(3);
    handleCurrentView();

  } else {
    if (hal.touch->params.action == 2 && hal.touch->params.gesture == 3) {
      result = sys.setCurrentApp(sys.getCurrentApp()+1);
      thisEvent.dispatched = true;
    } else if (hal.touch->params.action == 2 && hal.touch->params.gesture == 4) {
      result = sys.setCurrentApp(sys.getCurrentApp()-1);
      thisEvent.dispatched = true;
    }
    sys.notifyTouchEvent(thisEvent);
    handleCurrentView();
  }
}


void handleCurrentView() {
  if (sys.isAppChanged() && !alreadyRefreshing) {
    // dont trip over, signal that a refresh is in progress in case
    // the rtc attempts to print something else on screen
    alreadyRefreshing = true;
    hal.display->fillScreen(BGCOLOR);
    sys.reportAppChanged();
    alreadyRefreshing = false;
  }
  if (sys.getLCDState() && !alreadyRefreshing) {
    #ifdef MEAUSARE_FRAMERATE
      beginRefresh = millis();
    #endif
    alreadyRefreshing = true;
    switch (sys.getCurrentApp()) {
      case 0:
        watchface.route();
        break;
      case 1:
        stopwatch.route();
        break;
      case 2:
        heartrate.render();
        break;
      case 3:
        settings.router();
        break;
      default:
        hal.display->println("APP ERR");
        break;
    }
    #ifdef MEAUSARE_FRAMERATE
      endRefresh = millis();
      hal.display->setCursor(80,180);
      hal.display->setTextSize(2);
      hal.display->setTextColor(PRIMARY, BGCOLOR);
      hal.display->print(endRefresh - beginRefresh);
      hal.display->print(" ms   ");
    #endif
    alreadyRefreshing = false;
  }



  if (sys.getLCDState()) {
    sys.updateStandbyTime();
  }
}
void rtcTick() {
  sys.notifyClockChange(currentTime.hh, currentTime.mm, currentTime.ss);
  if (clockBooted) {
    handleCurrentView();
  }
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

extern "C" {
  void RTC2_IRQHandler(void) {
    NRF5_RESET_EVENT(RTC->EVENTS_COMPARE[0]);
    currentTime.ss++;
    if (currentTime.ss >= 60) {
      currentTime.mm++;
      currentTime.ss = 0;
      if (currentTime.mm >= 60) {
        currentTime.mm = 0;
        currentTime.hh++;
      }
    }
    rtcTick();
    RTC->TASKS_CLEAR = 1;
  }
}
