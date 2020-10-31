
// Example file name : ERM19264_UC1609_SINGLEBUFFER.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of Single buffer mode. 
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option SINGLE_BUFFER must be selected and only this option. 
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ****************************** 

#include <ERM19264_UC1609.h>

#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. USER adjust. 

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD, RST, CS); 
 

int count = 0;

// ************* SETUP ***************
void setup() 
{
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  delay(1000);
}

// ************** MAIN LOOP ***********
void loop() 
{
  DisplayText();
}

// ************** END OF MAIN ***********

void DisplayText() 
{
   uint8_t  screenBuffer[1537];  // create a full screen buffer (192 * 64/8) + 1
   mylcd.buffer = (uint8_t*) &screenBuffer;  // Assign the pointer to the buffer
   mylcd.LCDclearBuffer(); // Clear the buffer
   mylcd.setTextSize(2);
   mylcd.setTextColor(FOREGROUND);
   while(1)
   {
    mylcd.setCursor(0, 24);
    mylcd.print(F("Single Buffer"));
    mylcd.setCursor(0,40);
    mylcd.print(count);
    mylcd.LCDupdate();   // Writes to the buffer 
    delay(1000);
    mylcd.LCDclearBuffer();
    count= count + 10;  
   }
}
