/*!
  @file ERM19264_UC1609_GRAPHICS.ino
  @brief Example file for ERM19264_UC1609 library, showing use of graphic functions from included graphics class.
  @note  URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
  @author Gavin Lyons
  @details
    -# (1) GPIO is for arduino UNO for other tested MCU see readme.
    -# (2) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
   @test
		-# Test graphics 201
*/

#include <ERM19264_UC1609.h> // Include the library

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
#define FULLSCREEN (MYLCDWIDTH * (MYLCDHEIGHT/8)) // 1536 bytes

// define a buffer to cover whole screen 
uint8_t  screenBuffer[FULLSCREEN]; 
// instantiate an LCD object
ERM19264_UC1609  mylcd(MYLCDWIDTH , MYLCDHEIGHT , CD, RST, CS); 
// Instantiate  a screen object, in this case to cover whole screen
ERM19264_UC1609_Screen fullScreen(screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0); 


// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(LCDCONTRAST, LCDRAMADDRCTRL); // initialize the LCD
  mylcd.LCDFillScreen(0xE5, 0); // Fill the screen with a  pattern , just for splashscreen effect.
  delay(1500);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
  mylcd.ActiveBuffer =  &fullScreen;   // Assign address of screen object to be the "active buffer" pointer 
  mylcd.LCDclearBuffer(); // Clear the buffer
}


// *********** MAIN LOOP ******************
void loop()
{
  DisplayGraphics(); 
  mylcd.LCDPowerDown();
  while(1){delay(100);}; //wait here forever
}
// *********** END OF MAIN ***********

// Function to display Graphics.
void  DisplayGraphics()
{
  // Q1 |Q2
  //-----
  //Q3 | Q4
  bool colour = 1;
  uint8_t count = 0;
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
  mylcd.setTextSize(1);
  
  
  while (count < 50)
  {
    colour = !colour;
    count++;
    //print count 
    mylcd.setCursor(0, 0);
    mylcd.print(count);
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
