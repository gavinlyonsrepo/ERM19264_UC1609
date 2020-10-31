/*
* Project Name: ERM19264_UC1609
* File: ERM19264_UC1609.h
* Description: ERM19264 LCD driven by UC1609C controller header file
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
*/

#ifndef ERM19264_UC1609_H
#define ERM19264_UC1609_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


//***********************************************
// ** USER BUFFER OPTION SECTION **********
// Pick one buffer option and ONE  option only 
#define MULTI_BUFFER // (1) Note default
//#define SINGLE_BUFFER // (2) 
//#define NO_BUFFER   // (3) 
// **********************************************
// **********************************************


#ifdef NO_BUFFER
   #include "custom_graphics_font.h"
#else
   #include "custom_graphics.h"
#endif

#include <SPI.h>

// Display Pixel colours   definition
// (1): white on blue,  FG = white BG = blue
// ERM19264SBS-4 LCD Display White on Blue
// (2): black on white, FG = black BG = white
// ERM19264FS-4 LCD Display Black on White
// (3):white on black,  FG = white, BG = black
// ERM19264DNS-4LCD Display White on Black
#define FOREGROUND  0
#define BACKGROUND 1
#define INVERSE 2

// UC1909 Read registers
#define UC1609_GET_STATUS 0x01 // Not used v1.0

// UC1909 Write registers
#define UC1609_SYSTEM_RESET 0xE2 // Not used v1.0

#define UC1609_POWER_CONTROL 0x2F 
#define UC1609_PC_SET 0x06 // PC[2:0] 110 Internal V LCD (7x charge pump) + 10b: 1.4mA

#define UC1609_ADDRESS_CONTROL 0x88 // set RAM address control
#define UC1609_ADDRESS_SET 0x02 // Set AC [2:0] Program registers  for RAM address control.

#define UC1609_SET_PAGEADD 0xB0 // Page address Set PA[3:0]
#define UC1609_SET_COLADD_LSB 0x00 // Column Address Set CA [3:0]
#define UC1609_SET_COLADD_MSB 0x10 // Column Address Set CA [7:4]

#define UC1609_TEMP_COMP_REG 0x27 // Temperature Compensation Register
#define UC1609_TEMP_COMP_SET 0x00 // TC[1:0] = 00b= -0.00%/ C

#define UC1609_FRAMERATE_REG 0xA0 // Frame rate
#define UC1609_FRAMERATE_SET 0x01  // Set Frame Rate LC [4:3] 01b: 95 fps

#define UC1609_BIAS_RATIO 0xE8 // Bias Ratio. The ratio between V-LCD and V-D .
#define UC1609_BIAS_RATIO_SET 0x03 //  Set BR[1:0] = 11 (set to 9 default)

#define UC1609_GN_PM 0x81 // Set V BIAS Potentiometer to fine tune V-D and V-LCD  (double-byte command)
#define UC1609_DEFAULT_GN_PM 0x49 // default only used if user does not specify Vbias

#define UC1609_LCD_CONTROL 0xC0 // Rotate map control
#define UC1609_DISPLAY_ON 0xAE // enables display
#define UC1609_ALL_PIXEL_ON 0xA4 // sets on all Pixels on
#define UC1609_INVERSE_DISPLAY 0xA6 // inverts display
#define UC1609_SCROLL 0x40 // scrolls , Set the scroll line number. 0-64

// Rotate
#define UC1609_ROTATION_FLIP_TWO 0x06
#define UC1609_ROTATION_NORMAL 0x04
#define UC1609_ROTATION_FLIP_ONE 0x02
#define UC1609_ROTATION_FLIP_THREE 0x00

// Delays
#define UC1609_RESET_DELAY 150 // mS delay
#define UC1609_RESET_DELAY2   1 // mS delay
#define UC1609_INIT_DELAY 100   //  mS delay
#define UC1609_INIT_DELAY2 3 // mS delay

// No buffer font
#define UC1609_ASCII_OFFSET 0x20 //0x20, ASCII character for Space
#define UC1609_FONTPADDING  send_data(0x00)
#define UC1609_FONTWIDTH 5

// GPIO
#define UC1609_CS_SetHigh digitalWrite(_LCD_CS, HIGH)
#define UC1609_CS_SetLow digitalWrite(_LCD_CS, LOW)
#define UC1609_CD_SetHigh digitalWrite(_LCD_CD, HIGH)
#define UC1609_CD_SetLow digitalWrite(_LCD_CD, LOW)
#define UC1609_RST_SetHigh digitalWrite(_LCD_RST, HIGH)
#define UC1609_RST_SetLow digitalWrite(_LCD_RST, LOW)

// SPI
#define SPI_FREQ 8000000 // Mhz
//#define SPI_FREQ 1000000 // Mhz, can be used on fast devices
#define UC1609_HIGHFREQ_DELAY 0 // Can be used in software SPI for high freq MCU

// Display  Size
const uint8_t LCD_WIDTH = 192;
const uint8_t LCD_HEIGHT = 64;

#ifdef MULTI_BUFFER

struct MultiBuffer
{
  uint8_t* screenbitmap; // pointer to buffer
  uint8_t width=LCD_WIDTH;  // bitmap x size
  uint8_t height=LCD_HEIGHT ; // bitmap y size
  int16_t xoffset = 0; // x offset
  int16_t yoffset = 0; // y offset
  int8_t data = 0;  
};
#endif

//class 
#ifdef NO_BUFFER
class ERM19264_UC1609 {
#else
class ERM19264_UC1609 : public custom_graphics {
#endif
  public:
     // Contructor 1 Software SPI with explicit SCLK and SDIN
    ERM19264_UC1609(int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din);
    // Constructor 2 Hardware SPI
    ERM19264_UC1609(int8_t cd, int8_t rst, int8_t cs);

    ~ERM19264_UC1609(){};

#ifdef MULTI_BUFFER
   MultiBuffer* ActiveBuffer;
#endif

#ifdef SINGLE_BUFFER
    uint8_t* buffer;
    uint8_t bufferWidth = LCD_WIDTH;
    uint8_t bufferHeight = LCD_HEIGHT;
#endif

// Functions not needed for no_buffer mode 
#ifndef NO_BUFFER
    virtual void drawPixel(int16_t x, int16_t y, uint16_t colour) override;
     void LCDupdate(void);
     void LCDclearBuffer(void);
     void LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);
#else  // Functions only needed for one of  buffer mode's
     void  LCDNoBufferGotoXY(uint8_t column , uint8_t page);
     void  LCDNoBufferChar(char character);
     void  LCDNoBufferString(const char *characters);
#endif
    
    void LCDbegin(uint8_t VbiasPot = UC1609_DEFAULT_GN_PM );
    void LCDinit(void);
    void LCDEnable(uint8_t on);
    void LCDFillScreen(uint8_t pixel, uint8_t mircodelay);
    void LCDFillPage(uint8_t pixels);
    void LCDrotate(uint8_t rotatevalue);
    void invertDisplay(uint8_t on);
    void LCD_allpixelsOn(uint8_t bits);
    void LCDscroll(uint8_t bits);
    void LCDReset(void);
    void LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data);
           
  private:

    void send_data(uint8_t data);
    void send_command(uint8_t command, uint8_t value);
    bool isHardwareSPI(void);
    void CustomshiftOut(uint8_t bitOrder, uint8_t val);

    int8_t _LCD_CS;
    int8_t _LCD_CD;
    int8_t _LCD_RST;
    int8_t _LCD_SCLK; // Software SPI only
    int8_t _LCD_DIN;  // Software SPI only
    uint8_t _VbiasPOT; // Contrast default 0x49 datasheet 00-FE
};

#endif
