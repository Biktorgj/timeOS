/* Base clock */
#include <Adafruit_ST7789.h>
class Clock
{
  public:
    Clock();
    void drawClock(Adafruit_ST7789 tft);
  private:
    int hh;
    int mm;
    int ss;
 
};
