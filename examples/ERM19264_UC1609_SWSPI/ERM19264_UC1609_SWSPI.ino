
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

#define mylcdheight 64
#define mylcdwidth  192
#define VbiasPOT 0x49 // contrast range 0x00 to 0xFF, 0x49 is datasheet default, user adjust.

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
#define SCLK 4 // GPIO pin number pick any you want
#define DIN  5 // GPIO pin number pick any you want

ERM19264_UC1609  mylcd(CD, RST, CS, SCLK, DIN ); // instantiate object pick any GPIO you want

long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch

// ************* SETUP ***************
void setup() {
  delay(50);
  Serial.begin(9600);
  Serial.println("up");
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
  startTime = millis();

}


// ************** MAIN LOOP ***********
void loop() {
  // Define a full screen buffer
  uint8_t  textBuffer[(mylcdwidth * (mylcdheight / 8)) + 1]; // 192 X (64/8) + 1 = 1537

  MultiBuffer window;
  window.screenbitmap = (uint8_t*) &textBuffer;
  window.width = mylcdwidth;
  window.height = mylcdheight;

  // Call a function to display text
  DisplayText(&window);
}

// ************** END OF MAIN ***********

void DisplayText(MultiBuffer* screen)
{
  int count = 0;
  mylcd.setTextColor(FOREGROUND);
  mylcd.setTextSize(1);
  mylcd.ActiveBuffer =  screen;
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
