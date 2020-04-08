#include <nrf.h>

#define RTC NRF_RTC0
#define RTC_IRQ RTC0_IRQn

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
