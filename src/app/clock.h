/* Base clock */
#include <Adafruit_ST7789.h>
#include "system.h"

class Clock
{
  public:
    Clock();
    void drawClock(Adafruit_ST7789 *tft, System *sys);
  private:


};
