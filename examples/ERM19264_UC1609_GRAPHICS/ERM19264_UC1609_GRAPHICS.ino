
// Example file name : ERM19264_UC1609_GRAPHICS.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of graphic functions from included graphics library.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ******************************


#include <ERM19264_UC1609.h> // Include the library

#define mylcdheight 64
#define mylcdwidth  192
#define VbiasPOT 0x54 // Contrast 0x00 to 0xFF , 49h default , User adjust

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD, RST, CS);

// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0xE5, 0); // Fill the screen with a a pattern , "just for splashscreen effect"
  delay(1500);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
}


// *********** MAIN LOOP ******************
void loop()
{

  // Define a full screen buffer
  uint8_t  textBuffer[(mylcdwidth * (mylcdheight / 8)) + 1]; // 192 X (64/8) + 1 = 1537

  // Declare a struct
  MultiBuffer wholescreen;

  // Define the struct varibles
  wholescreen.screenbitmap = (uint8_t*) &textBuffer;
  wholescreen.width = 192; // bitmap x size, default
  wholescreen.height = 64; // bitmap y size, default
  wholescreen.xoffset = 0; // x offset, default
  wholescreen.yoffset = 0; // y offset, default

  // Call a function to display graphics pass it the struct
  DisplayGraphics(&wholescreen);

}
// *********** END OF MAIN ***********

// Function to display Graphics.
void  DisplayGraphics(MultiBuffer* targetBuffer)
{
  //Q1 |Q2
  //-----
  //Q3 | Q4
  //
  bool colour = 1;
  mylcd.ActiveBuffer =  targetBuffer;   // Set the buffer struct object
  mylcd.LCDclearBuffer(); // Clear the buffer
  while (1)
  {
    colour = !colour;

    // Draw the X
    mylcd.drawLine(96,  0, 96, 64, FOREGROUND);
    mylcd.drawFastVLine(94, 0, 64, FOREGROUND);
    mylcd.drawFastHLine(0, 32, 192, FOREGROUND);

    //Q1
    mylcd.fillRect(0, 10, 20, 20, colour);
    mylcd.fillCircle(40, 20, 10, FOREGROUND);
    mylcd.fillTriangle(60, 30, 70, 10, 80, 30, !colour);
    //Q2
    mylcd.drawRect(100, 10, 20, 20, FOREGROUND);
    mylcd.drawCircle(140, 20, 10, colour);
    mylcd.drawTriangle(160, 30, 170, 10, 180, 30, FOREGROUND);
    //Q3
    mylcd.fillRoundRect(0, 40, 40, 20, 10, !colour);
    mylcd.fillRoundRect(45, 40, 40, 20, 10, colour);
    //Q4
    char i;
    for (i = 0; i < 10; i ++)
    {
      mylcd.drawRect(100 + i, 40 + i, 80 - i * 2, 20 - i * 2, FOREGROUND);
      mylcd.LCDupdate();
      delay(50);
    }
    mylcd.LCDclearBuffer();
  }
}
