
// Example file name : ERM19264_UC1609_HELLO.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of hello world
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ******************************

#include "ERM19264_UC1609.h"

// LCD setup defines 
#define LCDcontrast 0x49 
#define mylcdheight 64
#define mylcdwidth  192
// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD, RST, CS); // instantiate object

void setup() 
{
  mylcd.LCDbegin(LCDcontrast); // initialize the OLED
  mylcd.LCDFillScreen(0x00, 0);
  mylcd.setTextColor(FOREGROUND);
  mylcd.setFontNum(1);
}

void loop()
{

  // define a buffer to cover whole screen 
  uint8_t  screenBuffer[mylcdwidth * (mylcdheight/8)]; //1536 bytes

  // Declare a multi buffer struct
  MultiBuffer mybuffer;

  // Intialise struct 
  mybuffer.screenbitmap = (uint8_t*) &screenBuffer; // point it to the buffer
  mybuffer.width = mylcdwidth ;
  mybuffer.height = mylcdheight;
  mybuffer.xoffset = 0;
  mybuffer.yoffset = 0;

  // Assign address of struct to be the active buffer pointer 
  mylcd.ActiveBuffer = &mybuffer;

  mylcd.LCDclearBuffer();   // Clear active buffer 
  
  while (1)
  {
    mylcd.setCursor(20, 20);
    mylcd.print(F("Hello world"));
    mylcd.LCDupdate();  // Update screen , write active buffer to screen 
    delay(5000);
  }
}
