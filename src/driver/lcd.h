#ifndef _LCD_
#define _LCD_

class LCD
{
  public:
    LCD();
    void setBrightness(uint8_t); // Actually set active brightness
    void saveBrightness(uint8_t); // Store brightness setting
    uint8_t getBrightness(void); // Get brightness setting
  private:
    uint8_t _currentBrightness;
    uint8_t _savedBrightness;
};

#endif
