#include <nrf.h>

/*
 *  RTC0 is used by Softdevice for bluetooth handling
 *  RTC1 is used by the Arduino core for delay()
 *  So we try to use RTC2 as it seems to be free, and it works :)
 */
#define RTC NRF_RTC2
#define RTC_IRQ RTC2_IRQn


// Include the HAL
#include "src/hal.h"

// Include system
#include "src/system.h"
// Include the base theme
#include "src/theme.h"

// Languages here
#include "src/views/clock.h"
#include "src/views/hrm.h"

typedef struct RTCTimerData {
  int hh;
  int mm;
  int ss;
};
RTCTimerData currentTime;


HAL hal;
System sys(&hal);
Clock clock(&sys, &hal);
HeartRate heartrate(&sys, &hal);
bool clockBooted = false;

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

  hal.display->setCursor(80,100);
  hal.display->setTextSize(2);
  hal.display->setTextColor(BLACK, WHITE);
  hal.display->println("Hello!");
  hal.display->fillScreen(WHITE); // Paint it black
  sys.resetState();
  hal.display->fillScreen(BLACK);
  // Attach Interrupts
  attachInterrupt(digitalPinToInterrupt(SIDE_BTN_IN), buttonInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(TP_INT), touchInterrupt, RISING);
  clockBooted = true;
  startRTC(); // If I start the RTC bluetooth goes to trash

}

void loop() {
  hal.bluetooth->poll();
  hal.forward();
  //  clock.route();
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
  if (sys.isAppChanged()) {
        hal.display->fillScreen(BLACK);
        sys.reportAppChanged();
  }
  if (sys.getLCDState()) {
    switch (sys.getCurrentApp()) {
      case 0:
        clock.route();
        break;
      case 1:
        heartrate.render();
        break;
      default:
      hal.display->println("APP ERR");
      break;
    }
  }



  if (sys.getLCDState()) {
    sys.updateStandbyTime();
  }
}
void rtcTick() {
  sys.notifyClockTchange(currentTime.hh, currentTime.mm, currentTime.ss);
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
