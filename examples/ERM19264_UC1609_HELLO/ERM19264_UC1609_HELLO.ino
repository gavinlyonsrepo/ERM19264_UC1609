/*!
  @file ERM19264_UC1609_HELLO.ino
  @brief Example file for ERM19264_UC1609 library, showing display of hello world on LCD
  @note  URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
  @author Gavin Lyons
  @details
    -# (1) GPIO is for arduino UNO for other tested MCU see readme.
    -# (2) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
*/

#include "ERM19264_UC1609.h"


// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

// LCD setup defines 
#define LCDCONTRAST 0x49 // contrast: Range 0-0xFE, optional, default 0x49
#define LCDRAMADDRCTRL 0x02  // RAM address control: Range 0-0x07, optional, default 0x02
#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192

// define a buffer to cover whole screen 
uint8_t  screenBuffer[MYLCDWIDTH * (MYLCDHEIGHT/8)]; // 1536 bytes
// instantiate an LCD object
ERM19264_UC1609  mylcd(CD, RST, CS); 
// Instantiate  a screen object, in this case to cover whole screen
ERM19264_UC1609_Screen fullScreen(screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0); 

void setup() 
{
  mylcd.LCDbegin(LCDCONTRAST,LCDRAMADDRCTRL); // initialize the OLED
  mylcd.LCDFillScreen(0x00, 0); // clear screen 
  mylcd.setTextColor(FOREGROUND); // set text color
  mylcd.setFontNum(UC1609Font_Default); // set font type
  mylcd.ActiveBuffer = &fullScreen;   // Assign address of screen object to be the "active buffer" pointer 
  mylcd.LCDclearBuffer();   // Clear active buffer 
}

void loop()
{ 
    mylcd.setCursor(20, 20);
    mylcd.print(F("Hello world"));
    mylcd.LCDupdate();  // Update screen , write active buffer to screen 
    delay(5000);
}
