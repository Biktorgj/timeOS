/*
* keys.cpp: Handle button keypresses
*/

#include "Arduino.h"
#include "ui.h"
#include "theme.h"

UI::UI() {

  restoreObjects();
  clear_screen = false;
}
void UI::attHal(HAL *Hal) {
  hal = Hal;
}
void UI::restoreObjects(void) {
  previousTopLeftObj = resetObject(previousTopLeftObj);
  previousTopRightObj = resetObject(previousTopRightObj);
  prevLeftBtn = resetObject(prevLeftBtn);
  prevRightBtn = resetObject(prevRightBtn);
  prevFullBtn = resetObject(prevFullBtn);
  prevMainBlockNumber = resetObject(prevMainBlockNumber);
}
void UI::renderTopLeft(bool def, UIObject newobj) {
  newobj.text = "B";
  if (newobj.text != prevLeftBtn.text) {
  if (!def) {
    hal->display->fillRect(0, 0, 110, 40, newobj.bg_color);
  } else {
    hal->display->fillRect(0, 0, 110, 40, WHITE);
  }
  hal->display->setCursor(10, 10);
  hal->display->setTextColor(BLUE, BGCOLOR);
  hal->display->print(newobj.text);
}

  prevLeftBtn = newobj;
}
/* Batt variable is called to either show the battery or the clock */
void UI::renderTopRight(bool batt, UIObject newobj) {
  uint8_t batt_bars = hal->power->getBatteryPercentage() / 10;
  newobj.digit_0 = batt_bars; // Store this here to check on the new refresh val
  uint8_t i;
  uint16_t color = SUCCESS;
  if (batt_bars < 3) {
    color = DANGER;
  } else if (batt_bars < 5) {
    color = WARNING;
  }
  if (batt && newobj.digit_0 != prevRightBtn.digit_0) {
    hal->display->drawRect(176,10,60, 20, BLACK);
    hal->display->drawRect(236,16,2, 8, BLACK);
    for (i = 0; i <= batt_bars; i++) {
        hal->display->fillRect(177+(i*6),12, 6, 16, color);
    }
    if (newobj.digit_0 == 100) {
      hal->display->setCursor(120, 10);
    } else {
      hal->display->setCursor(130, 10);
    }
    hal->display->setTextColor(color, BGCOLOR);
    hal->display->print(hal->power->getBatteryPercentage());
    hal->display->print("%");
  } else if (!batt) {
    hal->display->setCursor(130, 10);
    hal->display->setTextColor(PRIMARY, BGCOLOR);
    if (newobj.digit_0 < 10) {
      hal->display->print("0");
    }
    hal->display->print(newobj.digit_0);
    hal->display->print(":");
    if (newobj.digit_1 < 10) {
      hal->display->print("0");
    }
    hal->display->print(newobj.digit_1);
    hal->display->print(":");
    if (newobj.digit_2 < 10) {
      hal->display->print("0");
    }
    hal->display->println(newobj.digit_2);
  }
  prevRightBtn = newobj;
}

void UI::renderMainBlock(bool def, UIObject newobj) {

}

void UI::renderMainBlockNumbers(bool def, UIObject newobj) {
  int first_digit = 48;
  int digit_size = 24;
  int ypos = 100;
  renderDigit( first_digit, ypos, newobj.digit_0/10,prevMainBlockNumber.digit_0/10, newobj.front_color);
  renderDigit( first_digit + (digit_size * 1), ypos, newobj.digit_0%10,prevMainBlockNumber.digit_0%10, newobj.front_color);

  renderDigit( first_digit + (digit_size * 2), ypos, newobj.digit_1/10, prevMainBlockNumber.digit_1/10, newobj.front_color);
  renderDigit( first_digit + (digit_size * 3), ypos, newobj.digit_1%10, prevMainBlockNumber.digit_1%10, newobj.front_color);

  renderDigit( first_digit + (digit_size * 4), ypos, newobj.digit_2/10, prevMainBlockNumber.digit_2/10, newobj.front_color);
  renderDigit( first_digit + (digit_size * 5), ypos, newobj.digit_2%10, prevMainBlockNumber.digit_2%10, newobj.front_color);

  hal->display->fillRect(first_digit + (digit_size * 2) - 3, ypos+10, 2, 2, newobj.front_color);
  hal->display->fillRect(first_digit + (digit_size * 2) - 3, ypos+26, 2, 2, newobj.front_color);
  // Dots between minutes and seconds
  hal->display->fillRect(first_digit + (digit_size * 4) - 3, ypos+10, 2, 2, newobj.front_color);
  hal->display->fillRect(first_digit + (digit_size * 4) - 3, ypos+26, 2, 2, newobj.front_color);
  prevMainBlockNumber = newobj;
}

void UI::renderMainBlockNumberInt(bool def, UIObject newobj) {
  int first_digit = 48;
  int digit_size = 24;
  int ypos = 100;
  renderDigit( first_digit + (digit_size * 1), ypos, newobj.digit_0%10,prevMainBlockNumber.digit_0%10, newobj.front_color);
  renderDigit( first_digit + (digit_size * 2), ypos, newobj.digit_1/10, prevMainBlockNumber.digit_1/10, newobj.front_color);
  renderDigit( first_digit + (digit_size * 3), ypos, newobj.digit_1%10, prevMainBlockNumber.digit_1%10, newobj.front_color);
  prevMainBlockNumber = newobj;
}

void UI::renderBottomLeftBtn(bool def, UIObject newobj) {
  if (newobj.bg_color != prevLeftBtn.bg_color) {
  }
  hal->display->fillRect(0, 200, 110, 240, newobj.bg_color);
  hal->display->setCursor(10, 210);
  hal->display->print(newobj.text);
}

void UI::renderBottomRightBtn(bool def, UIObject newobj) {
  hal->display->fillRect(130, 200, 240, 240, newobj.bg_color);
  hal->display->setCursor(140, 210);
  hal->display->print(newobj.text);
}
void UI::renderBottomFullButton(bool def, UIObject newobj) {
  if (newobj.text != prevFullBtn.text) {
  hal->display->fillRect(0, 200, 240, 240, newobj.bg_color);
  hal->display->setCursor(80, 210);
  hal->display->setTextColor(newobj.front_color);
  hal->display->print(newobj.text);
  }
  prevFullBtn = newobj;
}

UIObject UI::resetObject(UIObject object) {
  object.enabled = false;
  object.text = "";
  object.front_color = PRIMARY;
  object.bg_color = BGCOLOR;
  object.digit_0 = 0;
  object.digit_1 = 0;
  object.digit_2 = 0;
  return object;
}

void UI::renderDigit(int posX, int posY, int digit, int prev_val, uint16_t col){
  if (digit != prev_val) {
    drawDigit(posX, posY, prev_val, WHITE);
  }
  drawDigit(posX, posY, digit, col);

}

void UI::drawDigit(int posX, int posY, int digit, uint16_t col){
  switch (digit){
    case 0:
    hal->display->fillRect(posX, posY, 20, 8, col); // up
    hal->display->fillRect(posX, posY+32, 20, 8, col); // down
    hal->display->fillRect(posX, posY, 7, 40, col); // left
    hal->display->fillRect(posX+13, posY, 7, 40, col); // right
    break;
    case 1:
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    break;
    case 2:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX, posY+24, 7, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 3:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 4:
    hal->display->fillRect(posX, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    break;
    case 5:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY+24, 7, 8, col);
    hal->display->fillRect(posX, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 6:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY+24, 7, 8, col);
    hal->display->fillRect(posX, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 7:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    break;
    case 8:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
    case 9:
    hal->display->fillRect(posX, posY, 20, 8, col);
    hal->display->fillRect(posX, posY, 7, 16, col);
    hal->display->fillRect(posX, posY+16, 20, 8, col);
    hal->display->fillRect(posX+13, posY, 7, 40, col);
    hal->display->fillRect(posX, posY+32, 20, 8, col);
    break;
  }
}
