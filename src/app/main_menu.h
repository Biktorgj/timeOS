/* Base clock */
#include <Adafruit_ST7789.h>
#include "system.h"

#ifndef _MAINMENU_
#define _MAINMENU_
class MainMenu
{
  public:
    MainMenu();
    void render(Adafruit_ST7789 *tft, System *sys);
  private:

};

#endif
