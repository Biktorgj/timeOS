/*
 * System class
 *  This puts all the watch toghether
 *
 *
 *
 */
 #ifndef _SYSTEMCL_
 #define _SYSTEMCL_
 #include "hal.h"
 #include "ui.h"

struct DateTimeArray {
  uint8_t dd;
  uint8_t mm;
  uint16_t yy;
  uint8_t hh;
  uint8_t ii;
  uint8_t ss;
};

struct Alarm {
  bool repeat[7];
  bool enable;
  uint8_t hh;
  uint8_t ii;
  uint8_t ss;
};

struct TouchEvent {
  int x;
  int y;
  int gesture;
  int action;
  bool dispatched;
};

struct Accel {
  uint16_t x;
  uint16_t y;
  uint16_t z;
};

class System
{
  public:
    System(HAL *Hal);
    DateTimeArray getCurrentTime();
    uint8_t getCurrentApp();
    bool setCurrentApp(uint8_t);
    void resetStandbyTime();
    bool getLCDState();
    void setLCDState(bool);
    void updateStandbyTime(void);
    bool isTimeToSleep();
    void resetState();
    void reportAppChanged();
    bool isAppChanged();
    uint8_t getPreviousApp();
    bool isPendingTouchEvent();
    TouchEvent getTouchEvent();
    void notifyTouchEvent(TouchEvent);
    void notifyClockChange(int, int, int);
    UI *ui;
  private:
    HAL *hal;
    bool hal_ready;
    // Current app tracking
    unsigned int current_app;
    unsigned int previous_app;
    unsigned int available_apps;

    unsigned long display_start_time;
    unsigned long display_end_time;
    unsigned long prevTick;
    bool lcd_state;
    unsigned int lcd_standby_seconds;
    DateTimeArray timestorage;
    TouchEvent touch_event;
    Alarm alarms[5];

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
};

// This must be in one line

#endif
