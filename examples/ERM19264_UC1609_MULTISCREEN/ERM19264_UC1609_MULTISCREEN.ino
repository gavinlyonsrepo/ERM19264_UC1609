/*!
	@file ERM19264_UC1609_MULTISCREEN.ino
	@brief Example file for ERM19264_UC1609 library, showing use of mulitple screens(two) sharing one buffer.  
	@author Gavin Lyons
	@note
		-# GPIO is for arduino UNO for other tested MCU see readme.
		-# This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino  example.
     <https://github.com/gavinlyonsrepo/ERM19264_UC1609>
	@test
		-# 501 Shared buffer, Multiple screens (2 off) spilt into left and right , sharing one buffer
		-# 502 FPS test frame per second 
*/

// two shared screens sharing one buffer
//  ______________________
// | Left side | Right side|
// |           |           |
// |          |            |
// |__________|____________|
//

#include <ERM19264_UC1609.h>
// LCD setup 
// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

#define LCDCONTRAST 0x49 // contrast: Range 0-0xFE, optional, default 0x49
#define LCDRAMADDRCTRL 0x02  // RAM address control: Range 0-0x07, optional, default 0x02
#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192
#define HALFSCREEN ((MYLCDWIDTH * (MYLCDHEIGHT/8))/2) // 1536/2 bytes = 768 bytes

// define a buffer to cover whole screen 
uint8_t  halfScreenBuffer[HALFSCREEN]; 

ERM19264_UC1609  mylcd(MYLCDWIDTH , MYLCDHEIGHT, CD, RST, CS); // create LCD object 

// Instantiate  a screen object, in this case to the left side of screen
// (buffer, width/2, height, x_offset, y-offset)
ERM19264_UC1609_Screen leftSideScreen(halfScreenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT, 0, 0); 
// Instantiate  a screen object, in this case the right side of screen
// (buffer, width/2, height, x_offset, y-offset/2)
ERM19264_UC1609_Screen rightSideScreen(halfScreenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT, MYLCDWIDTH/2, 0); 

// vars to control test timing. 
static long previousMillis  = 0;
uint16_t count  = 0;
uint16_t seconds  = 0;
bool colour = false;

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(LCDCONTRAST, LCDRAMADDRCTRL); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);
}

// *********** MAIN LOOP ******************
void loop() {
  static long framerate = 0;

  mylcd.setTextColor(FOREGROUND);
  mylcd.setTextSize(1);

  
  while(1)
  {
    display_Left(framerate, count);
    display_Right();
    framerate++;
    count++;
  }
}
// *********** END OF MAIN ***********

// Function to display left hand side buffer
void display_Left(long currentFramerate, int count)
{
  mylcd.ActiveBuffer = &leftSideScreen; // set active buffer to left side
  mylcd.LCDclearBuffer();
  mylcd.setCursor(0, 0);
  mylcd.print(F("Left Screen:"));

  mylcd.setCursor(0, 10);
  mylcd.print(F("96 * 64/8 = 768"));

  mylcd.setCursor(0, 20);
  mylcd.print(seconds);

  mylcd.setCursor(0, 30);
  mylcd.print(count);
  // Values to count frame rate per second
  static long lastFramerate = 0;
  static uint16_t fps;
  unsigned long currentMillis = millis();

  if (currentMillis  - previousMillis  >= 1000) {
    fps = currentFramerate - lastFramerate;
    lastFramerate = currentFramerate ;
    previousMillis  = currentMillis;
    colour =  !colour;
    seconds++;
  }

  mylcd.setCursor(0, 40);
  mylcd.print(fps);

  mylcd.setCursor(0, 50);
  mylcd.print(mylcd.LCDLibVerNumGet());
  mylcd.drawFastVLine(92, 0, 63, FOREGROUND);
  mylcd.LCDupdate();
}

// Function to display right hand side buffer
void display_Right()
{
  mylcd.ActiveBuffer = &rightSideScreen; // set active buffer to right side
  mylcd.LCDclearBuffer();
  mylcd.setCursor(0, 0);
  mylcd.print(F("Right Screen:"));

  mylcd.fillRect(0, 10, 20, 20, colour);
  mylcd.fillCircle(40, 20, 10, FOREGROUND);
  mylcd.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
  mylcd.drawRoundRect(10, 40, 60, 20, 10, FOREGROUND);

  mylcd.LCDupdate();
}
