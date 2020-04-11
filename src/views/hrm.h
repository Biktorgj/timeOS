/* Base clock */
#include "../system.h"
#include "../hal.h"
#include "HRS3300lib.h"

#ifndef _HEARTRATE_
#define _HEARTRATE_
class HeartRate
{
  public:
    HeartRate(System *System, HAL *Hal);
    void render();
    void resetSensorLib();
  private:
    bool _reading;
    uint8_t _lastread;
    unsigned long _lastrefresh;
    unsigned long _last_begin;
    HRS3300lib HRS3300;
    System *sys;
    HAL *hal;
};

#endif
