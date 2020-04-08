/* Accel, HRM, Touch panel I2C */

#define BMA400_INTERRUPT 8

#include "bma400.h"
BlueDot_BMA400 bma400 = BlueDot_BMA400();
/*
   i2C Devices: 0x15: Touchscreen
                0x18: ???
                0x44: ???
*/

bool accel_ready = false;
int setupAccel() {
  pinMode(BMA400_INTERRUPT, INPUT);
  int retcode = 0x00;
  bma400.parameter.I2CAddress = 0x18;                  //Choose I2C Address
  bma400.parameter.powerMode = 0x01;                   //Choose power mode
  bma400.parameter.outputDataRate = 0x08;             //Choose measurement range
  //bma400.parameter.oversamplingRate = 0x00;             //Choose measurement range
  retcode = bma400.init();

    accel_ready = true;
     bma400.readData();
  return retcode;
}


float getX() {
  bma400.readData();
      return bma400.parameter.acc_x;
}
float getY() {
  bma400.readData();
        return bma400.parameter.acc_y;
}
float getZ() {
  bma400.readData();
      return bma400.parameter.acc_z;

}


void scanI2C(Adafruit_ST7789 tft)
{
  tft.fillScreen(0xFFFF);
  byte error, address;
  int nDevices;
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(0xFFFF, 0x0000);
  tft.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    tft.setCursor(0, address + 30);
    if (error == 0)
    {
      tft.println("Device: 0x");
      tft.setCursor(200, address + 30);
      tft.print(address, HEX);


      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

}
