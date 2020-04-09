/* Base clock */
#include <Adafruit_ST7789.h>
#include "system.h"
#include "HRS3300lib.h"

#ifndef _HEARTRATE_
#define _HEARTRATE_
class HeartRate
{
  public:
    HeartRate();
    void render(Adafruit_ST7789 *tft, System *sys);
  private:
    bool _reading;
    uint8_t _lastread;
    unsigned long _lastrefresh;
    unsigned long _last_begin;
    HRS3300lib HRS3300;

};

#endif
