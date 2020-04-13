/*
 * hal: Unifies all the hardware into one main class
*/
#include "hal.h"
#include "theme.h"

BLESerial BLESerial(BLE_REQ, BLE_RDY, BLE_RST);
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); //for display with CS pin
Vibra Vibrator;
Key Key;
Touch Touch;
Power Power;
HeartRateMonitor HeartRateMonitor;
LCD Lcd;
// Adafruit_FlashTransport_SPI flashTransport(FLASH_CE, &SPI);
// Adafruit_SPIFlash Flash(&flashTransport);

HAL::HAL() {
  pinMode(SIDE_BTN_OUT, OUTPUT);
  digitalWrite(SIDE_BTN_OUT, HIGH);
  pinMode(SIDE_BTN_IN, INPUT);
//  pinMode(FLASH_CE, OUTPUT);
  // Setup interrupt and reporting
}
#define HWMODE

void HAL::init() {
  lcd = &Lcd;
  lcd->setBrightness(1);
  display = &tft;
  display->init(240, 240);
  display->setSPISpeed(16000000);
  display->setRotation(2); // Turn the display around
  display->fillScreen(WHITE); // Paint it black
  display->setCursor(0,30);
  display->setTextSize(2);
  display->setTextColor(BLACK, WHITE);
  display->println("Hi!");
  display->println("[VIB]: Ready");
  vibra = &Vibrator;
  display->println("[KEY]: Ready");
  key = &Key;
  display->println("[TOUCH]: Ready");
  touch = &Touch;
  display->println("[PWR]: Ready");
  power = &Power;
  display->println("[HRM]: Ready");
  hrm = &HeartRateMonitor;

//  flash = &Flash;

  // custom services and characteristics can be added as well
  bluetooth = &BLESerial;
  bluetooth->setLocalName("pineTime");

  Serial.begin(115200);
  bluetooth->begin();

  vibra->init();

//  flash->begin();
//  delay(2000);

  hrm->enable();
  hrm->disable();
}

void HAL::pollBLE() {
  bluetooth->poll();
}

// forward received from Serial to BLESerial and vice versa
void HAL::forward() {
  if (bluetooth && Serial) {
    int byte;
    while ((byte = BLESerial.read()) > 0) Serial.write((char)byte);
    while ((byte = Serial.read()) > 0) BLESerial.write((char)byte);
  }
}

// echo all received data back
void HAL::loopback() {
  if (BLESerial) {
    int byte;
    while ((byte = BLESerial.read()) > 0) BLESerial.write(byte);
  }
}

// periodically sent time stamps
void HAL::spam() {
  if (BLESerial) {
    BLESerial.print(millis());
    BLESerial.println(" tick-tacks!");
    delay(1000);
  }
}
void HAL::setupInterrupt() {

}
