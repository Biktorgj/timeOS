/***************************************************
This is a library for the ST7789 IPS SPI display.

Originally written by Limor Fried/Ladyada for
Adafruit Industries.

Modified by Ananev Ilia
****************************************************/

#include "Graphics.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

#define SPI_MOSI_PIN 3 // MOSI (P0.23)
#define SPI_SCK_PIN 2 // SCK (P0.25)

static const uint8_t PROGMEM
cmd_240x240[] = {                 		// Initialization commands for 7789 screens
  10,                       				// 9 commands in list:
  ST7789_SWRESET,   ST_CMD_DELAY,  		// 1: Software reset, no args, w/delay
  150,                     				// 150 ms delay
  ST7789_SLPOUT ,   ST_CMD_DELAY,  		// 2: Out of sleep mode, no args, w/delay
  255,                    				// 255 = 500 ms delay
  ST7789_COLMOD , 1+ST_CMD_DELAY,  		// 3: Set color mode, 1 arg + delay:
  0x55,                   				// 16-bit color
  10,                     				// 10 ms delay
  ST7789_MADCTL , 1,  					// 4: Memory access ctrl (directions), 1 arg:
  0x00,                   				// Row addr/col addr, bottom to top refresh
  ST7789_CASET  , 4,  					// 5: Column addr set, 4 args, no delay:
  0x00, ST7789_240x240_XSTART,          // XSTART = 0
  (ST7789_TFTWIDTH+ST7789_240x240_XSTART) >> 8,
  (ST7789_TFTWIDTH+ST7789_240x240_XSTART) & 0xFF,   // XEND = 240
  ST7789_RASET  , 4,  					// 6: Row addr set, 4 args, no delay:
  0x00, ST7789_240x240_YSTART,          // YSTART = 0
  (ST7789_TFTHEIGHT+ST7789_240x240_YSTART) >> 8,
  (ST7789_TFTHEIGHT+ST7789_240x240_YSTART) & 0xFF,	// YEND = 240
  ST7789_INVON ,   ST_CMD_DELAY,  		// 7: Inversion ON
  10,
  ST7789_NORON  ,   ST_CMD_DELAY,  		// 8: Normal display on, no args, w/delay
  10,                     				// 10 ms delay
  ST7789_DISPON ,   ST_CMD_DELAY,  		// 9: Main screen turn on, no args, w/delay
  255 };                  				// 255 = 500 ms delay

  inline uint16_t swapcolor(uint16_t x) {
    return (x << 11) | (x & 0x07E0) | (x >> 11);
  }

  static SPISettings mySPISettings;


  #define SPI_BEGIN_TRANSACTION()    if (_hwSPI)    SPI.beginTransaction(mySPISettings)
  #define SPI_END_TRANSACTION()      if (_hwSPI)    SPI.endTransaction()

  // Constructor when using software SPI.  All output pins are configurable.
  Graphics::Graphics(int8_t dc, int8_t rst, int8_t sid, int8_t sclk, int8_t cs)
  : Adafruit_GFX(ST7789_TFTWIDTH, ST7789_TFTHEIGHT)
  {
    _cs   = cs;
    _dc   = dc;
    _sid  = sid;
    _sclk = sclk;
    _rst  = rst;
    _hwSPI = false;
    if(dc == -1) _SPI9bit = true;
    else _SPI9bit = false;
  }

  // Constructor when using hardware SPI.  Faster, but must use SPI pins
  // specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
  Graphics::Graphics(int8_t dc, int8_t rst, int8_t cs)
  : Adafruit_GFX(ST7789_TFTWIDTH, ST7789_TFTHEIGHT) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _hwSPI = true;
    _SPI9bit = false;
    _sid  = _sclk = -1;
  }
  inline void Graphics::spiwrite(uint8_t c)
  {

      // Fast SPI bitbang swiped from LPD8806 library
      for(uint8_t bit = 0x80; bit; bit >>= 1) {
        digitalWrite(_sclk, LOW);
        if(c & bit) digitalWrite(_sid, HIGH);
        else        digitalWrite(_sid, LOW);
        digitalWrite(_sclk, HIGH);
      }

  }

  void Graphics::writecommand(uint8_t c) {
    DC_LOW();
    spiwrite(c);
  }

  void Graphics::writedata(uint8_t c) {
    DC_HIGH();
    spiwrite(c);
  }

  // Companion code to the above tables.  Reads and issues
  // a series of LCD commands stored in PROGMEM byte array.
  void Graphics::displayInit(const uint8_t *addr) {

    uint8_t  numCommands, numArgs;
    uint16_t ms;
    //<-----------------------------------------------------------------------------------------
    DC_HIGH();
    #if defined(USE_FAST_IO)
    *clkport |=  clkpinmask;
    #else
    digitalWrite(_sclk, HIGH);
    #endif
    //<-----------------------------------------------------------------------------------------

    numCommands = pgm_read_byte(addr++);   // Number of commands to follow
    while(numCommands--) {                 // For each command...
      writecommand(pgm_read_byte(addr++)); //   Read, issue command
      numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
      ms       = numArgs & ST_CMD_DELAY;   //   If hibit set, delay follows args
      numArgs &= ~ST_CMD_DELAY;            //   Mask out delay bit
      while(numArgs--) {                   //   For each argument...
        writedata(pgm_read_byte(addr++));  //     Read, issue argument
      }

      if(ms) {
        ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
        if(ms == 255) ms = 500;     // If 255, delay for 500 ms
        delay(ms);
      }
    }
  }


  // Initialization code common to all ST7789 displays
  void Graphics::commonInit(const uint8_t *cmdList) {
    _ystart = _xstart = 0;
    _colstart  = _rowstart = 0; // May be overridden in init func

    pinMode(_dc, OUTPUT);
    if(_cs) {
      pinMode(_cs, OUTPUT);
    }

    if(_hwSPI) { // Using hardware SPI
      SPI.begin();
      mySPISettings = SPISettings(16000000, MSBFIRST, SPI_MODE3);
     
    } else {
      pinMode(_sclk, OUTPUT);
      pinMode(_sid , OUTPUT);
      digitalWrite(_sclk, LOW);
      digitalWrite(_sid, LOW);
    }

    // toggle RST low to reset; CS low so it'll listen to us

    if (_rst != -1) {
      pinMode(_rst, OUTPUT);
      digitalWrite(_rst, HIGH);
      delay(50);
      digitalWrite(_rst, LOW);
      delay(50);
      digitalWrite(_rst, HIGH);
      delay(50);
    }

    if(cmdList)
    displayInit(cmdList);
  }

  void Graphics::setRotation(uint8_t m) {

    writecommand(ST7789_MADCTL);
    rotation = m % 4; // can't be higher than 3
    switch (rotation) {
      case 0:
      writedata(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);

      _xstart = _colstart;
      _ystart = _rowstart;
      break;
      case 1:
      writedata(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);

      _ystart = _colstart;
      _xstart = _rowstart;
      break;
      case 2:
      writedata(ST7789_MADCTL_RGB);

      _xstart = _colstart;
      _ystart = _rowstart;
      break;

      case 3:
      writedata(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);

      _ystart = _colstart;
      _xstart = _rowstart;
      break;
    }
  }

  void Graphics::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
    uint16_t y1) {

      uint16_t x_start = x0 + _xstart, x_end = x1 + _xstart;
      uint16_t y_start = y0 + _ystart, y_end = y1 + _ystart;


      writecommand(ST7789_CASET); // Column addr set
      writedata(x_start >> 8);
      writedata(x_start & 0xFF);     // XSTART
      writedata(x_end >> 8);
      writedata(x_end & 0xFF);     // XEND

      writecommand(ST7789_RASET); // Row addr set
      writedata(y_start >> 8);
      writedata(y_start & 0xFF);     // YSTART
      writedata(y_end >> 8);
      writedata(y_end & 0xFF);     // YEND

      writecommand(ST7789_RAMWR); // write to RAM
    }

    void Graphics::pushColor(uint16_t color) {

      DC_HIGH();


      spiwrite(color >> 8);
      spiwrite(color);



    }

    void Graphics::drawPixel(int16_t x, int16_t y, uint16_t color) {

      if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

      setAddrWindow(x,y,x+1,y+1);


      DC_HIGH();


      spiwrite(color >> 8);
      spiwrite(color);



    }

    void Graphics::drawFastVLine(int16_t x, int16_t y, int16_t h,
      uint16_t color) {

        // Rudimentary clipping
        if((x >= _width) || (y >= _height)) return;
        if((y+h-1) >= _height) h = _height-y;
        setAddrWindow(x, y, x, y+h-1);

        uint8_t hi = color >> 8, lo = color;


        DC_HIGH();


        while (h--) {
          spiwrite(hi);
          spiwrite(lo);
        }



      }

      void Graphics::drawFastHLine(int16_t x, int16_t y, int16_t w,
        uint16_t color) {

          // Rudimentary clipping
          if((x >= _width) || (y >= _height)) return;
          if((x+w-1) >= _width)  w = _width-x;
          setAddrWindow(x, y, x+w-1, y);

          uint8_t hi = color >> 8, lo = color;


          DC_HIGH();


          while (w--) {
            spiwrite(hi);
            spiwrite(lo);
          }



        }

        void Graphics::fillScreen(uint16_t color) {
          fillRect(0, 0,  _width, _height, color);
        }

        // fill a rectangle
        void Graphics::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
          uint16_t color) {

            // rudimentary clipping (drawChar w/big text requires this)
            if((x >= _width) || (y >= _height)) return;
            if((x + w - 1) >= _width)  w = _width  - x;
            if((y + h - 1) >= _height) h = _height - y;

            setAddrWindow(x, y, x+w-1, y+h-1);

            uint8_t hi = color >> 8, lo = color;



            DC_HIGH();

            for(y=h; y>0; y--) {
              for(x=w; x>0; x--) {
                spiwrite(hi);
                spiwrite(lo);
              }
            }


          }

          // Pass 8-bit (each) R,G,B, get back 16-bit packed color
          uint16_t Graphics::Color565(uint8_t r, uint8_t g, uint8_t b) {
            return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
          }

          void Graphics::invertDisplay(boolean i) {
            writecommand(i ? ST7789_INVON : ST7789_INVOFF);
          }

          /******** low level bit twiddling **********/

          inline void Graphics::DC_HIGH(void) {
            _DCbit = true;
            digitalWrite(_dc, HIGH);
          }

          inline void Graphics::DC_LOW(void) {
            _DCbit = false;
            digitalWrite(_dc, LOW);
          }

          void Graphics::init(uint16_t width, uint16_t height) {
            commonInit(NULL);

            _colstart = ST7789_240x240_XSTART;
            _rowstart = ST7789_240x240_YSTART;
            _height = 240;
            _width = 240;

            displayInit(cmd_240x240);

            setRotation(2);
          }
