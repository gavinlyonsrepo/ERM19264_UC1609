/*!
	@file ERM19264_UC1609_MULTISCREEN_TWO.ino
	@brief Example file for ERM19264_UC1609 library, showing use of mulitple screens(four) sharing one buffer.  To save data
	@author Gavin Lyons
	@note
		-# GPIO is for arduino UNO for other tested MCU see readme.
		-# This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino  example.
     <https://github.com/gavinlyonsrepo/ERM19264_UC1609>
	@test
		-# Shared buffer two , Multiple screens (4 off) spilt into top left, top right, bottom left, bottom right, sharing one buffer
*/

// Four shared screens sharing one buffer
//   Q1   Q2
//   Q3   Q4

#include <ERM19264_UC1609.h>

#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192
#define LCDCONTRAST 0x49 // contrast: Range 0-0xFE, optional, default 0x49
#define LCDRAMADDRCTRL 0x02  // RAM address control: Range 0-0x07, optional, default 0x02

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

// Define a 1/4 screen sized buffer
uint8_t  fourthScreenBuffer[(MYLCDWIDTH * (MYLCDHEIGHT/8))/4]; // 1536/4 = 384 bytes

ERM19264_UC1609  mylcd(CD, RST, CS); //  Instantiate LCD object , CD, RST, CS

// (buffer, width, height, x_offset, y-offset)
// Instantiate  a screen object, in this case to the top left side of screen Q1
ERM19264_UC1609_Screen topLeftSideScreen(fourthScreenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT/2, 0, 0); 
// Instantiate  a screen object, in this case the top right side of screen Q2
ERM19264_UC1609_Screen topRightSideScreen(fourthScreenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT/2, MYLCDWIDTH/2, 0); 
// Instantiate  a screen object, in this case to the bottom left side of screen Q3
ERM19264_UC1609_Screen botLeftSideScreen(fourthScreenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT/2, 0, MYLCDHEIGHT/2); 
// Instantiate  a screen object, in this case the bottom right side of screen Q4
ERM19264_UC1609_Screen botRightSideScreen(fourthScreenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT/2, MYLCDWIDTH/2, MYLCDHEIGHT/2); 


// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(LCDCONTRAST, LCDRAMADDRCTRL); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);
  mylcd.setTextColor(FOREGROUND);
  mylcd.setTextSize(1);
  mylcd.setFontNum(UC1609Font_Default); // set font type
}

// *********** MAIN LOOP ******************
void loop() {

  mylcd.ActiveBuffer = &topLeftSideScreen;
  mylcd.LCDclearBuffer();
  mylcd.setCursor(5,5);
  mylcd.print("top left Q1");
  mylcd.LCDupdate();

  mylcd.ActiveBuffer = &topRightSideScreen;
  mylcd.LCDclearBuffer();
  mylcd.setCursor(5,5);
  mylcd.print("top right Q2");
  mylcd.LCDupdate();

  mylcd.ActiveBuffer = &botLeftSideScreen;
  mylcd.LCDclearBuffer();
  mylcd.setCursor(5,5);
  mylcd.print("bot left Q3");
  mylcd.LCDupdate();

  mylcd.ActiveBuffer = &botRightSideScreen;
  mylcd.LCDclearBuffer();
  mylcd.setCursor(5,5);
  mylcd.print("bot right Q4");
  mylcd.LCDupdate();

  delay(5000);
}
// *********** END OF MAIN ***********

