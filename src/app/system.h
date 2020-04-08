/*
 * System class
 *  This puts all the watch toghether
 *
 *
 *
 */

 #ifndef _SYSTEMCL_
 #define _SYSTEMCL_

 #include <nrf.h>
 #include <Adafruit_ST7789.h>

 #define RTC NRF_RTC0
 #define RTC_IRQ RTC0_IRQn

 #define APP_MODE_INIT 0x00
 #define APP_MODE_CLOCK 0x01
 #define APP_MODE_ALARM 0x02
 #define APP_MODE_TIMER 0x03
 #define APP_MODE_CHRONO 0x04
 #define APP_MODE_NOTIF 0x05
 #define APP_MODE_HRM 0x06
 #define APP_MODE_STEPS 0x07
 #define APP_MODE_DEBUG 0x99

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



struct DateTimeArray {
  uint8_t dd;
  uint8_t mm;
  uint8_t yy;
  uint8_t hh;
  uint8_t ii;
  uint8_t ss;
};

class System
{
  public:
    System();
    DateTimeArray getCurrentTime();
    uint8_t getCurrentApp();
    bool setCurrentApp(uint8_t);
    void resetStandbyTime();
    bool getLCDState();
    void setLCDState(bool);
    void resetLCD();
    void updateStandbyTime(void);
    bool isTimeToSleep();
    void resetState(Adafruit_ST7789 *tft);
    void appChanged();
    uint8_t getPreviousApp();

  private:
    unsigned int current_app;
    unsigned int previous_app;
    unsigned int available_apps;
    unsigned long display_start_time;
    unsigned long display_end_time;
    unsigned long prevTick;
    bool lcd_state;
    unsigned int lcd_standby_seconds;
    DateTimeArray timestorage;
    int hh;
    int mm;
    int ss;
    Adafruit_ST7789 *disp;

    /*
     *
     *  Power mode:
     *  0: Running
     *  1: Display off, running
     *  2: Powersave mode, accel on
     *  3: Deep sleep, accel off
     *
     */
    unsigned long power_mode;
    void startRTC(void);
};

// This must be in one line

#endif
