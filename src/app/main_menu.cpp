/*
 * keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "main_menu.h"
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))
MainMenu::MainMenu() {
}

void MainMenu::render(Adafruit_ST7789 *tft, System *sys) {
  String menuitems[] = { "Alarm", "Timer", "Stopwatch", "Heartrate", "Activity", "Settings" };
  int x = 50;
  int y = 20;
  int i = 0;
  tft->setTextSize(3);
  tft->setTextColor(0x00CF, 0x0000);
  for (i = 0; i < NUMITEMS(menuitems); i++) {
    tft->setCursor(x, y);
    tft->println(menuitems[i]);
    y+=30;

  }


}
