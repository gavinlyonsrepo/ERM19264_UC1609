
// Example file name : ERM19264_UC1609_SWSPI.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of software SPI  mode
// only difference for user is the constructor used when you instantiate the object and pin definations
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected and only this option.
// (3) This is for software  SPI for hardware SPI see all other examples.
// ******************************

#include <ERM19264_UC1609.h>

#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192
#define VbiasPOT 0x49 // contrast range 0x00 to 0xFF, 0x49 is datasheet default, user adjust.

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
#define SCLK 13 // GPIO pin number pick any you want
#define DIN  11 // GPIO pin number pick any you want

ERM19264_UC1609  mylcd(CD, RST, CS, SCLK, DIN ); // instantiate object pick any GPIO you want

// define a buffer to cover whole screen 
uint8_t  screenBuffer[(MYLCDWIDTH * (MYLCDHEIGHT/8))+1]; // 1536 +1 bytes

long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch

// ************* SETUP ***************
void setup() {
  delay(50);
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
  startTime = millis();

}

// ************** MAIN LOOP ***********
void loop() {

  MultiBuffer myStruct; // Declare a multi buffer struct
  // Intialise that struct (&struct, buffer, w, h, x-offset, y-offset)
  mylcd.LCDinitBufferStruct(&myStruct, screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0); 

  // Call a function to display text
  DisplayText(&myStruct);
}

// ************** END OF MAIN ***********

void DisplayText(MultiBuffer* targetBuffer)
{
  int count = 0;
  mylcd.setTextColor(FOREGROUND);
  mylcd.setTextSize(1);
  mylcd.ActiveBuffer =  targetBuffer;
  mylcd.LCDclearBuffer(); // Clear the buffer
  while (1)
  {
    mylcd.LCDclearBuffer();
    mylcd.setCursor(20, 30);
    mylcd.print(F("Software SPI speed test"));
    mylcd.setCursor(20, 50);
    if (count < 1000)
    {
      mylcd.print(count);
    } else
    {
      elapsedTime =   millis() - startTime;
      mylcd.print("Time Taken:");
      mylcd.print(elapsedTime / 1000L);
    }
    delay(1);
    mylcd.LCDupdate();  // Write to the buffer
    if (count == 1000)
    {
      while (1) {
        delay(1000);
      }; //test finished
    }
    count ++;
  }
}
