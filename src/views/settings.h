/* Base clock */
#include "../system.h"
#include "../hal.h"
#include "HRS3300lib.h"

#ifndef _SETTINGS_
#define _SETTINGS_
class Settings
{
  public:
    Settings(System *System, HAL *Hal);
    void router();


  private:
    System *sys;
    HAL *hal;
    int scroll_position;
    int active_section;

    // Menu sections
    void renderMain(TouchEvent);
    void renderDisplay(TouchEvent);
    void renderBluetooth(TouchEvent);
    void renderSensors(TouchEvent);
    void renderReset(TouchEvent);
    bool inRange(int,int,int,int, int, int);
    uint8_t handleMenuChange();
};

#endif

/*
ROOT Menu
  -> Display
      -> Brightness [1-5]
      -> Timeout Secs (5,10,15,20,30,60)
  -> Bluetooth
      -> On-Off
      -> Paired devices
  -> Body sensors
      -> Continuous HR (Off On)
      -> Step Counting (Off On)
  -> Reboot
  -> Reset Settings
*/
