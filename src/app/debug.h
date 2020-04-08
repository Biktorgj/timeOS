/* Base clock */
#include <Adafruit_ST7789.h>
#include "system.h"


class Debug
{
  public:
    Debug();
    void drawDebug(Adafruit_ST7789 *tft, System *sys);
  private:

};
