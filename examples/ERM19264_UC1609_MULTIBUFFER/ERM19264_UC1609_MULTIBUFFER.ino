
// Example file name : ERM19264_UC1609_MULTIBUFFER.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of mulitple buffers.  In this case: two, divided vertically.
// The user can also divide horizontally and create as many buffers as they want.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ******************************

#include <ERM19264_UC1609.h>

#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192
#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. USER adjust.

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD, RST, CS); // instate object , CD, RST, CS

// Define a half screen sized buffer
uint8_t  screenBuffer[(MYLCDWIDTH * (MYLCDHEIGHT/8))/2]; // 1536/2 = 768 bytes

// vars  the test control
static long previousMillis  = 0;
uint16_t count  = 0;
uint16_t seconds  = 0;
bool colour = false;

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);
}

// *********** MAIN LOOP ******************
void loop() {
  static long framerate = 0;

  mylcd.setTextColor(FOREGROUND);
  mylcd.setTextSize(1);

  MultiBuffer left_side;  // Declare a multi buffer struct for left side of screen
  // Intialise that struct (&struct, buffer, w, h, x_offset, y-offset)
  mylcd.LCDinitBufferStruct(&left_side, screenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT, 0, 0);  
  
  MultiBuffer right_side;  // Declare a multi buffer struct for right side of screen
  // Intialise that struct (&struct, buffer, w, h, x_offset, y-offset)
  mylcd.LCDinitBufferStruct(&right_side, screenBuffer, MYLCDWIDTH/2, MYLCDHEIGHT, MYLCDWIDTH/2, 0); 
  
  while(1)
  {
    display_Left(&left_side, framerate, count);
    display_Right(&right_side);
    framerate++;
    count++;
  }
}
// *********** END OF MAIN ***********

// Function to display left hand side buffer
void display_Left(MultiBuffer* targetbuffer, long currentFramerate, int count)
{
  mylcd.ActiveBuffer = targetbuffer; // set target buffer object
  mylcd.LCDclearBuffer();
  mylcd.setCursor(0, 0);
  mylcd.print(F("Left Buffer:"));

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
  mylcd.print("V 1.4.0");
  mylcd.drawFastVLine(92, 0, 63, FOREGROUND);
  mylcd.LCDupdate();
}

// Function to display right hand side buffer
void display_Right(MultiBuffer* targetbuffer)
{
  mylcd.ActiveBuffer = targetbuffer; // set target buffer object
  mylcd.LCDclearBuffer();
  mylcd.setCursor(0, 0);
  mylcd.print(F("Right buffer:"));

  mylcd.fillRect(0, 10, 20, 20, colour);
  mylcd.fillCircle(40, 20, 10, FOREGROUND);
  mylcd.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
  mylcd.drawRoundRect(10, 40, 60, 20, 10, FOREGROUND);

  mylcd.LCDupdate();
}
