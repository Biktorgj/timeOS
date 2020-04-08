/* Base clock */
#include <Adafruit_ST7789.h>
class Debug
{
  public:
    Debug();
    void drawDebug(Adafruit_ST7789 tft);
  private:
    int hh;
    int mm;
    int ss;
 
};
