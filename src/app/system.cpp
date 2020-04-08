/*
 * syste.cpp: Handle battery reporting functions
*/

#include "Arduino.h"
#include "system.h"
// #include "../driver/rtc.h"
typedef struct rtctimerdata {
  int hh;
  int mm;
  int ss;

};
rtctimerdata curtime;

System::System() {
  resetState();
  startRTC();
}

DateTimeArray System::getCurrentTime() {
  timestorage.hh = curtime.hh;
  timestorage.ii = curtime.mm;
  timestorage.ss = curtime.ss;
  return timestorage;
}
uint8_t System::getCurrentApp() {
  return current_app;
}
bool System::setCurrentApp(uint8_t appID) {
  if (appID >= 0 && appID < available_apps) {
    current_app = appID;
    return false; // success
  }

  return true; // failed
}
void System::resetStandbyTime() {
  display_start_time = millis();
  display_end_time = millis();
}
void System::updateStandbyTime() {
  display_end_time = millis();
}
bool System::getLCDState() {
  return lcd_state;
}
void System::setLCDState(bool state) {
  lcd_state = state;
}
bool System::isTimeToSleep() {
  if (display_end_time - display_start_time >= (lcd_standby_seconds * 1000)) {
    return true;
  }
  return false;
}
void System::resetState(void) {
  display_start_time = millis();
  display_end_time = millis();
  prevTick = millis();
  current_app = 0;
  available_apps = 3;
  hh = 0;
  mm = 0;
  ss = 0;
  lcd_state = true;

  timestorage.hh = 0;
  timestorage.ii = 0;
  timestorage.ss = 0;
  timestorage.dd = 0;
  timestorage.mm = 0;
  timestorage.yy = 0;
  lcd_standby_seconds = 20;

}

void System::startRTC() {
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

extern "C" {
  void RTC0_IRQHandler(void) {
    NRF5_RESET_EVENT(RTC->EVENTS_COMPARE[0]);
    //unsigned long currentTick = millis();
    //if (currentTick - appRuntime.prevTick >= 500) {
    //  appRuntime.prevTick = currentTick;
      curtime.ss++;
      if (curtime.ss >= 60) {
        curtime.mm++;
        curtime.ss = 0;
        if (curtime.mm >= 60) {
          curtime.mm = 0;
          curtime.hh++;
        }
      }
    //}
    RTC->TASKS_CLEAR = 1;
  }
}
