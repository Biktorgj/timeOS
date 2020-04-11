#ifndef _GPIO_
#define _GPIO_

// i2C addresses used by devices
#define TP_I2C_ADDRESS 0x15
#define BMA_I2C_ADDRESS 0x18
#define HRM_I2C_ADDRESS 0x44


/* Battery and charger */
#define BATT_VCC 31
#define BATT_CHG 12

/* Side Key */
#define SIDE_BTN_IN 13 // input
#define SIDE_BTN_OUT 15 // output

/* BLE Pins, unneeded really as we don't use them here */
#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

/* DC Motor pin */
#define VIBRA 16

/* Touchpanel */
#define TP_RESET 10
#define TP_INT 28 // input


/* Heart rate monitor test pin, not sure if needed for anything */
#define HRM_TEST 30 // input

/* LCD Display pins */
#define TFT_CS   25
#define TFT_RST  26
#define TFT_DC   18
#define TFT_MOSI 3  // Data out
#define TFT_SCLK 2  // Clock out

/* Backlight pins */
#define TFT_BL_LOW 14
#define TFT_BL_MID 22
#define TFT_BL_HI 23
#define TFT_DETECT 9 // Input





#endif
