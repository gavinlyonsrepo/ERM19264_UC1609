/*!
  @file ERM19264_UC1609_MISC.ino
  @author Gavin Lyons
  @brief Example file for ERM19264_UC1609 library, showing use of various functions

  @test
    -# 301 All Pixels on
    -# 302 Invert screen
    -# 303 Rotate screen using LCD rotate command.
    -# 304 Scroll Screen
    -# 305 Enable and disable Screen 
    -# 306 Rotate screen using rotate buffer. 
    -# 307 Power down (LCD OFF)

 
  @details
    -# (1) GPIO is for arduino UNO for other tested MCU see readme
    -# (2) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
    -# (3) URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
*/

#include <ERM19264_UC1609.h>  // Include the library

// LCD setup 
// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

#define LCDCONTRAST 0x49  // contrast: Range 0-0xFE, optional, default 0x49
#define LCDRAMADDRCTRL 0x02  // RAM address control: Range 0-0x07, optional, default 0x02
#define MYLCDHEIGHT 64
#define MYLCDWIDTH  192
#define FULLSCREEN (MYLCDWIDTH * (MYLCDHEIGHT/8)) // 1536 bytes

// define a buffer to cover whole screen 
uint8_t  screenBuffer[FULLSCREEN]; 
// instantiate an LCD object
ERM19264_UC1609  mylcd(MYLCDWIDTH , MYLCDHEIGHT , CD, RST, CS); 
// Instantiate  a screen object, in this case to cover whole screen
ERM19264_UC1609_Screen fullScreen(screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0); 

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(LCDCONTRAST, LCDRAMADDRCTRL);          // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);      // Clears screen
  mylcd.ActiveBuffer = &fullScreen;  // Assign address of screen object to be the "active buffer" pointer
}

// *********** MAIN LOOP ******************
void loop() {
  mylcd.LCDclearBuffer();  // Clear active buffer

  // Set text parameters
  mylcd.setTextColor(FOREGROUND);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);

  // Test 301 LCD all pixels on
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

  // Test 302 inverse
  mylcd.setCursor(20, 30);
  mylcd.print("inverse test  ");
  mylcd.LCDupdate();
  mylcd.LCDInvertDisplay(0);  // Normal
  delay(2000);
  mylcd.LCDInvertDisplay(1);  // Inverted
  delay(4000);
  mylcd.LCDInvertDisplay(0);


  // Test 303 LCD rotate command
  mylcd.LCDclearBuffer();
  mylcd.setCursor(20, 30);
  mylcd.print("rotate cmd test");
  mylcd.LCDupdate();
  delay(2000);
  mylcd.LCDrotate(UC1609_ROTATION_FLIP_ONE);
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDrotate(UC1609_ROTATION_NORMAL);
  mylcd.LCDupdate();
  delay(5000);


  // Test 304 LCD scroll
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

  //Test 305 LCD enable and disable
  mylcd.LCDclearBuffer();
  mylcd.setCursor(0, 30);
  mylcd.print("LCD Disable test");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDEnable(0);
  delay(5000);
  mylcd.LCDEnable(1);
  mylcd.LCDclearBuffer();

  // Test 306 rotate buffer test
  
  mylcd.LCDclearBuffer();
  mylcd.setCursor(5, 30);
  mylcd.print("rotate Buffer test");
  mylcd.LCDupdate();
  delay(2000);
  mylcd.LCDclearBuffer();

  mylcd.setRotation(LCD_Degrees_0);
  mylcd.setCursor(5, 5);
  mylcd.print("rotate 0");
  mylcd.setCursor(5, 50);
  mylcd.print("bottom");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDclearBuffer();

  mylcd.setRotation(LCD_Degrees_90);
  mylcd.setCursor(5, 5);
  mylcd.print("rotate 90");
  mylcd.setCursor(5, 180);
  mylcd.print("bottom");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDclearBuffer();

  mylcd.setRotation(LCD_Degrees_180);
  mylcd.setCursor(5, 5);
  mylcd.print("rotate 180");
  mylcd.setCursor(5, 50);
  mylcd.print("bottom");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDclearBuffer();

  mylcd.setRotation(LCD_Degrees_270);
  mylcd.setCursor(5, 5);
  mylcd.print("rotate    270");
  mylcd.setCursor(5, 180);
  mylcd.print("bottom");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.setRotation(LCD_Degrees_0); //back to normal rotation
  mylcd.LCDclearBuffer();

  // Test 307 Powerdown
  mylcd.setCursor(15, 25);
  mylcd.print("End Tests");
  mylcd.setCursor(5, 35);
  mylcd.print("Power down in 5");
  mylcd.LCDupdate();
  delay(5000);
  mylcd.LCDPowerDown();
  
  while (1) {delay(10);}  // tests over, loop here forever
}

// *********** END OF MAIN ***********
