/*!
  @file ERM19264_UC1609_MISC.ino
  @author Gavin Lyons
  @brief Example file for ERM19264_UC1609 library, showing use of various functions

  @test
    -# (1) All Pixels on
    -# (2) Invert screen
    -# (3) Rotate screen
    -# (4) Scroll Screen
    -# (5) Enable and disable Screen 
    -# (6) Power down (LCD OFF)

 
  @details
    -# (1) GPIO is for arduino UNO for other tested MCU see readme
    -# (2) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
    -# (3) URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
*/

#include <ERM19264_UC1609.h>  // Include the library
// LCD Setup
#define VbiasPOT 0x49  // Contrast 0x00 to 0xFF , 49h default , User adjust
#define MYLCDHEIGHT 64
#define MYLCDWIDTH 192
// define a buffer to cover whole screen
uint8_t screenBuffer[MYLCDWIDTH * (MYLCDHEIGHT / 8)];  // 1536 bytes

// GPIO 5-wire SPI interface
#define CD 10  // GPIO pin number pick any you want
#define RST 9  // GPIO pin number pick any you want
#define CS 8   // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

// instantiate an LCD object
ERM19264_UC1609 mylcd(CD, RST, CS);
// Instantiate  a screen object, in this case to cover whole screen
ERM19264_UC1609_Screen fullScreen(screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0);

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(VbiasPOT);          // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);      // Clears screen
  mylcd.ActiveBuffer = &fullScreen;  // Assign address of screen object to be the "active buffer" pointer
}

// *********** MAIN LOOP ******************
void loop() {
  mylcd.LCDclearBuffer();  // Clear active buffer

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
  mylcd.LCDallpixelsOn(1);
  delay(2000);
  mylcd.LCDallpixelsOn(0);
  delay(2000);

  // Test 2 inverse
  mylcd.setCursor(20, 30);
  mylcd.print("inverse test  ");
  mylcd.LCDupdate();
  mylcd.invertDisplay(0);  // Normal
  delay(2000);
  mylcd.invertDisplay(1);  // Inverted
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
  for (uint8_t i = 0; i < 62; i++) {
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
  
  while (1) {delay(1);}  // tests over, loop here forever
}

// *********** END OF MAIN ***********
