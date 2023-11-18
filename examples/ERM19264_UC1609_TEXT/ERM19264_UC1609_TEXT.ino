/*!
  @file ERM19264_UC1609_TEXT.ino
  @brief Example file for ERM19264_UC1609 library, showing use of Text and Fonts.
  @author Gavin Lyons
  @details
    -# GPIO is for arduino UNO for other tested MCU see readme.
    -# This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
    -# Test 6 In order to use extended ASCII font > 127   #define UC_FONT_MOD_TWO in the file <ERM19264_UC1609_graphics_font>
      must be commented in. It is by default.
     -# In order for tests 9-15 to work fully: the respective font
      must be enabled, see USER FONT OPTION ONE in file <ERM19264_graphics_font.h>.
      If font NOT enabled NOTHING will appear during test on LCD screen.
    -# URL = <https://github.com/gavinlyonsrepo/ERM19264_UC1609>

  @test
    -# Test 1 font size 3 string
    -# Test2 font size 2 string
    -# Test3 font size 1 string inverted
    -# Test4 draw a single character font size 4
    -# Test4b drawtext method
    -# Test5 print ASCII font 0-127
    -# Test6 print ASCII font 128-255, see notes 4 at top of file
    -# Test7 print function integer
    -# Test8 print function float + numerical types (HEX BIN etc)
    -# Test9 "thick" font 2
    -# Test10 "seven segment" font 3
    -# Test11 "wide " font 4
    -# Test12 "tiny" font 5
    -# Test13 "homespun" font 6
    -# Test14 "bignum" font 7
    -# Test15 "mednums" font 8
*/

#include "ERM19264_UC1609.h"

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
// define a buffer to cover whole screen   1536 bytes
uint8_t  screenBuffer[MYLCDWIDTH * (MYLCDHEIGHT/8)]; 
// instantiate an LCD object
ERM19264_UC1609  mylcd(CD, RST, CS); 
// Instantiate  a screen object, in this case to cover whole screen
ERM19264_UC1609_Screen fullScreen(screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0); 

//Define Some test timing delay 
#define DISPLAY_DELAY_ONE 5000
#define DISPLAY_DELAY_TWO 0
#define MYCOUNT_UPTIME 1000

// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(LCDCONTRAST, LCDRAMADDRCTRL); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
  mylcd.ActiveBuffer = &fullScreen; // Set the active buffer pointer to the address of full screen object
}

// ************** MAIN LOOP ***********
void loop()
{
    DisplayText();   // Tests 1-8
    DisplayFonts();  // Tests 9-15
}
// ************** END OF MAIN ***********


void DisplayText()
{
  char myString[9] = {'1', '0', ':', '1', '6', ':', '2', '9'};
  
  mylcd.setTextWrap(true);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.LCDclearBuffer(); // Clear the buffer

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

  TestReset();

  // Test 4b  Drawtext funtion.() bignum func not going work over size)
  mylcd.drawText(0,0,  myString, FOREGROUND, BACKGROUND,1);
  mylcd.drawText(0,32, myString, FOREGROUND, BACKGROUND,2);
  
  TestReset();

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
    delay(DISPLAY_DELAY_TWO);
  }

  TestReset();

  // Test 6

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
    delay(DISPLAY_DELAY_TWO);
  }

  TestReset();

  // Test 7
  mylcd.setCursor(0, 0);
  mylcd.print(12569);

  // Test 8
  mylcd.setCursor(5, 30);
  mylcd.print(33.91674, 2); // will print 33.92

  mylcd.setCursor(5, 40);
  mylcd.print(-211);

  TestReset();

  // Test 8b 
  mylcd.setCursor(0, 0);
  mylcd.print(47 , DEC);
  mylcd.setCursor(0, 16);
  mylcd.print(47 , HEX); 
  mylcd.setCursor(0, 32);
  mylcd.print(47, BIN);
  mylcd.setCursor(0, 48);
  mylcd.print(47 , OCT);

  TestReset();

} // end function


void DisplayFonts()
{
  char myString[9] = {'1', '3', ':', '2', '6', ':', '1', '8'};
  uint16_t  countUp = 0;

  mylcd.setTextWrap(true);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextColor(FOREGROUND);
  mylcd.LCDclearBuffer(); // Clear the buffer

  // Test 9
  mylcd.setCursor(0, 0);
  mylcd.print(F("Thick Font:"));
  mylcd.setFontNum(UC1609Font_Thick);
  mylcd.setCursor(0, 15);
  mylcd.print(F("THICK FONT 82362*!"));
  mylcd.drawChar(150, 25 , 'T', FOREGROUND, BACKGROUND, 4);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(F("TAB 6279"));

  TestReset();

  // Test 10
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
  mylcd.print(F("Seven Segment  Font:"));
  mylcd.setFontNum(UC1609Font_Seven_Seg);
  mylcd.setCursor(0, 15);
  mylcd.print(F("Seven seg  45638299"));
  mylcd.drawChar(150, 25 , 'S', FOREGROUND, BACKGROUND, 4);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(3);
  mylcd.print(F("3941"));


  TestReset();

  // Test 11
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
  mylcd.print(F("Wide Font:"));
  mylcd.setFontNum(UC1609Font_Wide);
  mylcd.setCursor(0, 15);
  mylcd.print(F("WIDE FONT 3871*!"));
  mylcd.drawChar(150, 25 , 'W', FOREGROUND, BACKGROUND, 3);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(3.14);

  TestReset();


  // Test 12
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
  mylcd.print(F("Tiny Font:"));
  mylcd.setFontNum(UC1609Font_Tiny);
  mylcd.setCursor(0, 15);
  mylcd.print(F("tiny FONT 9837*!"));
  mylcd.drawChar(150, 25 , 't', FOREGROUND, BACKGROUND, 3);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(3.144445, 2);

  TestReset();


  // Test 13
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
  mylcd.print(F("homespun Font:"));
  mylcd.setFontNum(UC1609Font_Homespun);
  mylcd.setCursor(0, 15);
  mylcd.print(F("Homespun 321*!"));
  mylcd.drawChar(150, 25 , 'h', FOREGROUND, BACKGROUND, 3);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(7.15);

  TestReset();

  // Test 14
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
  mylcd.print(F("big nums Font:"));
  mylcd.setFontNum(UC1609Font_Bignum );
  mylcd.setCursor(0, 15);
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
  mylcd.print(26.05); // Test 14a print a Float
  mylcd.setCursor(110, 15);
  mylcd.print(-28); // Test 14a print a negative number

  TestReset();

  mylcd.drawTextNumFont(0, 32, myString , BACKGROUND, FOREGROUND); // Test 14b drawTextNumFont , 13:26:18 inverted
  mylcd.drawCharNumFont(0, 0, '8', FOREGROUND, BACKGROUND); // Test 14c drawCharNumFont
  mylcd.drawCharNumFont(160, 0, '4', BACKGROUND, FOREGROUND); // Test 14d drawCharNumFont inverted

  TestReset();
  while (countUp < MYCOUNT_UPTIME) // Show a count 14e
  {
    mylcd.setCursor(0, 0);
    mylcd.print(countUp++);
    mylcd.LCDupdate();
  }
  TestReset();

  //test 15
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default );
  mylcd.setTextSize(1);
  mylcd.print(F("med nums Font:"));
  mylcd.setFontNum(UC1609Font_Mednum);
  mylcd.setCursor(0, 15);
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
  mylcd.print(26.43); // Test 15a print a Float
  mylcd.setCursor(110, 15);
  mylcd.print(-95); // Test 14a print a negative number

  TestReset();

  mylcd.drawTextNumFont(0, 32, myString , BACKGROUND, FOREGROUND); // Test 15b drawTextNumFont , 13:26:18 inverted
  mylcd.drawCharNumFont(0, 0, '8', FOREGROUND, BACKGROUND); // Test 15c drawCharNumFont
  mylcd.drawCharNumFont(160, 0, '4', BACKGROUND, FOREGROUND); // Test 15d drawCharNumFont inverted

  TestReset();
  countUp = 0;
  while (countUp < MYCOUNT_UPTIME) // Show a count 15e
  {
    mylcd.setCursor(0, 0);
    mylcd.print(countUp++);
    mylcd.LCDupdate();
  }
  TestReset();
}


void TestReset(void)
{
  mylcd.LCDupdate();  // Write to the buffer
  delay(DISPLAY_DELAY_ONE);
  mylcd.LCDclearBuffer();
}
