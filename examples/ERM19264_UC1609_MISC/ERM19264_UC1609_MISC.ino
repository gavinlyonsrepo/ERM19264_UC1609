
// Example file name : ERM19264_UC1609_Misc.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of various functions
//  (1) All Pixels on
//  (2) Invert screen
//  (3) Rotate screen
//  (4) Scroll Screen
//  (5) Enable and disable Screen 
//  (6) Power down (LCD OFF)
//
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION at top of file
// , option MULTI_BUFFER must be selected
// and only this option. It is on by default.
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ******************************

#include <ERM19264_UC1609.h> // Include the library

#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. USER adjust
#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD , RST, CS);

// define a buffer to cover whole screen 
uint8_t  screenBuffer[MYLCDWIDTH * (MYLCDHEIGHT/8)+1]; // 1536+1 bytes

// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(VbiasPOT);             // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // Clears screen
}

// *********** MAIN LOOP ******************
void loop()
{
  MultiBuffer myStruct; // Declare a multi buffer struct
  mylcd.LCDinitBufferStruct(&myStruct, screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0);  // Intialise that struct (&struct,buffer,w,h,x,y)
  mylcd.ActiveBuffer = &myStruct;   // Assign address of struct to be the "active buffer" pointer 
  mylcd.LCDclearBuffer();   // Clear active buffer 

  // Set text parameters
  mylcd.setTextColor(FOREGROUND);
  mylcd.setTextSize(2);

  // Test 1 LCD all pixels on
    mylcd.setCursor(20, 30);
    mylcd.print("All Pixels on");
    mylcd.LCDupdate();
    delay(4000);
    mylcd.LCDclearBuffer();
    mylcd.LCDupdate();
    mylcd.LCD_allpixelsOn(1);
    delay(2000);
    mylcd.LCD_allpixelsOn(0);
    delay(2000);

    // Test 2 inverse
    mylcd.setCursor(20, 30);
    mylcd.print("inverse test  ");
    mylcd.LCDupdate();
    mylcd.invertDisplay(0); // Normal
    delay(2000);
    mylcd.invertDisplay(1); // Inverted
    delay(4000);
    mylcd.invertDisplay(0);


    // Test3 LCD rotate
    mylcd.LCDclearBuffer();
    mylcd.setCursor(20, 30);
    mylcd.print("rotate test");
    mylcd.LCDupdate();
    delay(2000);
    mylcd.LCDrotate(UC1609_ROTATION_FLIP_ONE);
    mylcd.LCDupdate();
    delay(5000);
    mylcd.LCDrotate(UC1609_ROTATION_FLIP_TWO);
    mylcd.LCDupdate();
    delay(5000);
    mylcd.LCDrotate(UC1609_ROTATION_FLIP_THREE);
    mylcd.LCDupdate();
    delay(5000);
    mylcd.LCDrotate(UC1609_ROTATION_NORMAL);
    mylcd.LCDupdate();
    delay(5000);


  // Test4 LCD scroll
  mylcd.LCDclearBuffer();
  //mylcd.LCDupdate();
  mylcd.setCursor(0, 40);
  mylcd.print("scroll test");
  for (uint8_t i = 0 ; i < 62 ; i ++) 
  {
    mylcd.LCDscroll(i);
    mylcd.LCDupdate();
    delay(50);
  }
  mylcd.LCDscroll(0);

  //Test5 LCD enable and disable
  mylcd.LCDclearBuffer();
  mylcd.setCursor(0, 30);
  mylcd.print("LCD Disable test");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDEnable(0);
  delay(5000);
  mylcd.LCDEnable(1);
  mylcd.LCDclearBuffer();

  // Test 6 Powerdown
  mylcd.setCursor(5, 10);
  mylcd.print("End Tests");
  mylcd.setCursor(5, 35);
  mylcd.print("Power down in 5");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDPowerDown();
  while (1)
  {
    delay(1); // tests over, loop here forever
  }
}

// *********** END OF MAIN ***********
