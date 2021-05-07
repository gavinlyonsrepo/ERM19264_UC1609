
// Example file name : ERM19264_UC1609_TEXTino
// Description:
// Test file for ERM19264_UC1609 library, showing use of TEXT  mode .
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option MULTI_BUFFER must be selected and only this option.
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// (4) In order to use extended ASCII font (127 '}') comment out #define UC_FONT_MOD_TWO in the file <custom_graphics_font.h> TEST 6
// ******************************

// A series of tests to display the text mode
// Test 1 Font size 3 string
// Test 2 font size 2 string
// Test 3 font size 1 string inverted
// Test 4 draw a single character font size 4
// Test 5 print ASCII  font 0-127
// Test 6 print ASCII font 128-255, see notes 4 at top of file

#include <ERM19264_UC1609.h>
#include <ERM19264_graphics_font.h> // include just to see font mod. UC_FONT_MOD_TWO for test 6

#define mylcdheight 64
#define mylcdwidth  192
#define VbiasPOT 0x49 // contrast 00 to FF , default 0x49 , user adjust

#define DisplayDelay1 5000 
#define DisplayDelay2 0
 
// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD, RST, CS); // instantiate object


// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
}

// ************** MAIN LOOP ***********
void loop()
{

  // Define a full screen buffer
  uint8_t  textBuffer[(mylcdwidth * (mylcdheight / 8)) + 1]; // 192 X (64/8) + 1 = 1537
  MultiBuffer window;
  window.screenbitmap = (uint8_t*) &textBuffer;

  // Call a function to display text
  DisplayText(&window);

}
// ************** END OF MAIN ***********


void DisplayText(MultiBuffer* targetBuffer)
{

  mylcd.setTextWrap(true);
  mylcd.ActiveBuffer =  targetBuffer;
  mylcd.LCDclearBuffer(); // Clear the buffer
  while (1)
  {
    // Test 1
    mylcd.setTextColor(FOREGROUND);
    mylcd.setTextSize(3);
    mylcd.setCursor(0, 0);
    mylcd.print(F("HelloWorld"));

    // Test 2
    mylcd.setTextSize(2);
    mylcd.setCursor(0, 30);
    mylcd.print(F("1234567890"));

    // Test 3
    mylcd.setTextSize(1);
    mylcd.setTextColor(BACKGROUND, FOREGROUND);
    mylcd.setCursor(0, 50);
    mylcd.print(F("HelloWorld"));

    // Test 4
    mylcd.drawChar(150, 25 , 'H', FOREGROUND, BACKGROUND, 4);

    mylcd.LCDupdate();  // Write to the buffer
    delay(DisplayDelay1);
    mylcd.LCDclearBuffer();

    // Test 5
    mylcd.setCursor(0, 0);
    mylcd.setTextColor(FOREGROUND);
    mylcd.setTextSize(1);
    mylcd.print(F("ASCII font 0-127"));
    mylcd.setCursor(0, 15);
    // Print first 127 chars of font
    for (char i = 0; i < 126; i++)
    {
      if (i == '\n' || i == '\r') continue;
      mylcd.print(i);
      delay(DisplayDelay2);
    }
    mylcd.LCDupdate();  // Write to the buffer
    delay(DisplayDelay1);
    mylcd.LCDclearBuffer();

#ifndef UC_FONT_MOD_TWO
    mylcd.setCursor(0, 0);
    mylcd.setTextColor(FOREGROUND);
    mylcd.setTextSize(1);
    mylcd.print(F("ASCII font 128-255"));
    
    uint8_t x = 0;
    uint8_t y = 15;
    mylcd.setCursor(x, y);
    
    for (uint8_t i = 128; i < 255; i++)
    {
      if (x > 180)
      {
        x = 0;
        y += 9;
      }
      mylcd.drawChar(x, y , i, FOREGROUND, BACKGROUND, 1);
      x += 7;
      delay(DisplayDelay2);
    }
    
    mylcd.LCDupdate();  // Write to the buffer
    delay(DisplayDelay1);
    mylcd.LCDclearBuffer();
#endif
  } // while
} // end
