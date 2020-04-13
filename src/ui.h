/* Base UI */
#include "system.h"
#include "hal.h"

#ifndef _UI_
#define _UI_

struct UIObject {
  bool enabled;
  String text;
  uint16_t front_color;
  uint16_t bg_color;
  int16_t digit_0;
  int16_t digit_1;
  int16_t digit_2;
};

class UI
{
  public:
    UI();
    void attHal(HAL *Hal);
    void restoreObjects();
    void renderTopLeft(bool, UIObject);
    void renderTopRight(bool, UIObject);
    void renderMainBlock(bool, UIObject);
    void renderMainBlockNumberInt(bool, UIObject);
    void renderMainBlockNumbers(bool, UIObject);
    void renderBottomLeftBtn(bool, UIObject);
    void renderBottomRightBtn(bool, UIObject);
    void renderBottomFullButton(bool, UIObject);
    void drawDigit(int, int, int, uint16_t);
    void renderDigit(int, int, int, int, uint16_t);

  private:
    HAL *hal;
    bool clear_screen;
    UIObject resetObject(UIObject object);
    UIObject currentTopLeftObj;
    UIObject previousTopLeftObj;
    UIObject currentTopRightObj;
    UIObject previousTopRightObj;
    UIObject prevMainBlockNumber;
    UIObject prevLeftBtn;
    UIObject curLeftBtn;
    UIObject prevRightBtn;
    UIObject curRightBtn;
    UIObject curFullBtn;
    UIObject prevFullBtn;
};

#endif
