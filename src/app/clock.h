/* Base clock */
#include <Adafruit_ST7789.h>

// #include "runtime.h"

class Clock
{
  public:
    Clock();
    void drawClock(Adafruit_ST7789 tft,int hh, int mm, int ss);
  private:
    int hh;
    int mm;
    int ss;

};
