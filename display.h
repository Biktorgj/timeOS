/* Display pinout */

#define TFT_CS   25
#define TFT_RST  26
#define TFT_DC   18
#define TFT_MOSI 3  // Data out
#define TFT_SCLK 2  // Clock out

#define TFT_BL_LOW 14
#define TFT_BL_MID 22
#define TFT_BL_HI 23
#define TFT_DETECT 9 // Input

#define HWMODE

#ifdef HWMODE
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); //for display with CS pin
#else
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST); //for display with CS pin
#endif

#define PRIMARY_COLOR    0x0000
#define BLUE     0x001F
#define ACCENT_COLOR      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define BG_COLOR    0xFFFF

// Color definitions
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int backlightLevel = 0;
int getBacklightLevel() {
  return backlightLevel;
}
void setBacklightLevel(int level) {
  switch (level) {
    case 0:
      pinMode(TFT_BL_LOW, OUTPUT);
      digitalWrite(TFT_BL_LOW, HIGH);
      break;
    case 1:
      pinMode(TFT_BL_MID, OUTPUT);
      digitalWrite(TFT_BL_MID, HIGH);
      break;
     default:
      pinMode(TFT_BL_HI, OUTPUT);
      digitalWrite(TFT_BL_HI, HIGH);
      break;
  }
  delay(10);
}

void displayOff() {
  pinMode(TFT_BL_LOW, INPUT);
    pinMode(TFT_BL_MID, INPUT);
      pinMode(TFT_BL_MID, INPUT);
      delay(10);
  digitalWrite(TFT_BL_LOW, HIGH);
  digitalWrite(TFT_BL_MID, HIGH);
  digitalWrite(TFT_BL_HI, HIGH);
}

void displayOn() {
    pinMode(TFT_BL_LOW, OUTPUT);
    pinMode(TFT_BL_MID, OUTPUT);
      pinMode(TFT_BL_MID, OUTPUT);
      delay(10);
  digitalWrite(TFT_BL_LOW, LOW);
  digitalWrite(TFT_BL_MID, LOW);
  digitalWrite(TFT_BL_HI, LOW);
}
