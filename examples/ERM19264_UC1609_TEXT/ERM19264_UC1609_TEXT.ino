/*!
  @file ERM19264_UC1609_TEXT.ino
  @brief Example file for ERM19264_UC1609 library, showing use of Text and Fonts.
  @author Gavin Lyons
  @details
    -# GPIO is for arduino UNO for other tested MCU see readme.
    -# This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
     -# In order for tests 9-15 to work fully: the respective font
      must be enabled, see USER FONT OPTION ONE in file <ERM19264_graphics_font.h>.
      If font NOT enabled NOTHING will appear during test on LCD screen.
    -# URL = <https://github.com/gavinlyonsrepo/ERM19264_UC1609>

  @test
    -# 1 Test1 font size 3 string
    -# 2 Test2 font size 2 string
    -# 3 Test3 font size 1 string inverted
    -# 4 Test4 draw a single character font size 4, 4B drawtext method
    -# 5 Test5 print ASCII font 0-127
    -# 6 Test6 text wrap print and drawtext
    -# 7 Test7 print function integer
    -# 8 Test8 print function float + numerical types (HEX BIN etc)
    -# 9 Test9 "thick" font 2
    -# 10 Test10 "seven segment" font 3
    -# 11 Test11 "wide " font 4
    -# 12 Test12 "tiny" font 5
    -# 13 Test13 "homespun" font 6
    -# 14  Test14 "bignum" font 7
    -# 15 Test15 "mednums" font 8
    -# 16 Test16 "Arial Round" font 9
    -# 17 Test17 "Arial bold" font 10
    -# 18 Test18 "Mia" font 11
    -# 19 Test19 "dedica" font 12
    -# 20 Test20 Error checking
*/

#include "ERM19264_UC1609.h"

// LCD setup
// GPIO 5-wire SPI interface
#define CD 10  // GPIO pin number pick any you want
#define RST 9  // GPIO pin number pick any you want
#define CS 8   // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

#define LCDCONTRAST 0x49     // contrast: Range 0-0xFE, optional, default 0x49
#define LCDRAMADDRCTRL 0x02  // RAM address control: Range 0-0x07, optional, default 0x02
#define MYLCDHEIGHT 64
#define MYLCDWIDTH 192
#define FULLSCREEN (MYLCDWIDTH * (MYLCDHEIGHT / 8))  // 1536 bytes

// define a buffer to cover whole screen
uint8_t screenBuffer[FULLSCREEN];
// instantiate an LCD object
ERM19264_UC1609 mylcd(MYLCDWIDTH, MYLCDHEIGHT, CD, RST, CS);
// Instantiate  a screen object, in this case to cover whole screen
ERM19264_UC1609_Screen fullScreen(screenBuffer, MYLCDWIDTH, MYLCDHEIGHT, 0, 0);

//Define Some test timing delay
#define DISPLAY_DELAY_ONE 5000
#define DISPLAY_DELAY_TWO 0
#define MYCOUNT_UPTIME 250

// ************* SETUP ***************
void setup() {
  Serial.begin(38400);  // For Test 20 error checking
  delay(100);
  mylcd.LCDbegin(LCDCONTRAST, LCDRAMADDRCTRL);  // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);                 // Clear the screen
  mylcd.ActiveBuffer = &fullScreen;             // Set the active buffer pointer to the address of full screen object
  mylcd.LCDclearBuffer();                       // Clear the buffer
  mylcd.setTextWrap(true);
}

// ************** MAIN LOOP ***********
void loop() {

  DisplayText();      // Tests 1-8
  DisplayFonts();     // Tests 9-13
  DisplayFontsTwo();  // Tests 14-19
  ErrorChecking();    // Test 20
  mylcd.LCDPowerDown();
  while (1) { delay(100); };
}
// ************** END OF MAIN ***********


void DisplayText() {
  char myString[9] = { '1', '0', ':', '1', '6', ':', '2', '9' };
  mylcd.setFontNum(UC1609Font_Default);

  // Test 1
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
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
  mylcd.drawChar(150, 25, 'H', FOREGROUND, BACKGROUND, 4);

  TestReset();

  // Test 4b  Drawtext funtion.() bignum func not going work over size)
  mylcd.drawText(0, 0, myString, FOREGROUND, BACKGROUND, 1);
  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND, 2);  //inverted

  TestReset();

  // Test 5
  mylcd.setCursor(0, 0);
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
  mylcd.setTextSize(1);
  mylcd.print(F("ASCII font 0-127"));
  mylcd.setCursor(0, 15);
  // Print first 127 chars of font
  for (char i = 0; i < 126; i++) {
    if (i == '\n' || i == '\r') continue;
    mylcd.print(i);
    delay(DISPLAY_DELAY_TWO);
  }
  TestReset();

  // Test 6 testwrap
  // test 6a fonts 1-6
  mylcd.drawText(120, 0, myString, BACKGROUND, FOREGROUND, 2);  //inverted
  mylcd.setCursor(135, 32);
  mylcd.setTextSize(2);
  mylcd.print("Text wrap print");
  TestReset();
  // test 6b fonts 7-12
  mylcd.setFontNum(UC1609Font_ArialBold);
  mylcd.drawText(120, 0, myString, BACKGROUND, FOREGROUND);  //inverted
  mylcd.setCursor(135, 32);
  mylcd.print("Text wrap");
  TestReset();

  // Test 7
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.setCursor(0, 0);
  mylcd.print(12569);

  // Test 8
  mylcd.setCursor(5, 30);
  mylcd.print(33.91674, 2);  // will print 33.92

  mylcd.setCursor(5, 40);
  mylcd.print(-211);

  TestReset();

  // Test 8b
  mylcd.setCursor(0, 0);
  mylcd.print(47, DEC);
  mylcd.setCursor(0, 16);
  mylcd.print(47, HEX);
  mylcd.setCursor(0, 32);
  mylcd.print(47, BIN);
  mylcd.setCursor(0, 48);
  mylcd.print(47, OCT);

  TestReset();

}  // end function


void DisplayFonts() {

  mylcd.setTextWrap(true);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
  mylcd.LCDclearBuffer();  // Clear the buffer

  // Test 9
  mylcd.setCursor(0, 0);
  mylcd.print(F("Thick Font:"));
  mylcd.setFontNum(UC1609Font_Thick);
  mylcd.setCursor(0, 15);
  mylcd.print(F("THICK FONT 82362*!"));
  mylcd.drawChar(150, 25, 'T', FOREGROUND, BACKGROUND, 4);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(F("TAB 6279"));

  TestReset();

  // Test 10
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Seven Segment  Font:"));
  mylcd.setFontNum(UC1609Font_Seven_Seg);
  mylcd.setCursor(0, 15);
  mylcd.print(F("Seven seg  45638299"));
  mylcd.drawChar(150, 25, 'S', FOREGROUND, BACKGROUND, 4);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(3);
  mylcd.print(F("3941"));


  TestReset();

  // Test 11
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Wide Font:"));
  mylcd.setFontNum(UC1609Font_Wide);
  mylcd.setCursor(0, 15);
  mylcd.print(F("WIDE FONT 3871*!"));
  mylcd.drawChar(150, 25, 'W', FOREGROUND, BACKGROUND, 3);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(3.14);

  TestReset();


  // Test 12
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Tiny Font:"));
  mylcd.setFontNum(UC1609Font_Tiny);
  mylcd.setCursor(0, 15);
  mylcd.print(F("tiny FONT 9837*!"));
  mylcd.drawChar(150, 25, 't', FOREGROUND, BACKGROUND, 3);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(3.144445, 2);

  TestReset();


  // Test 13
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("homespun Font:"));
  mylcd.setFontNum(UC1609Font_Homespun);
  mylcd.setCursor(0, 15);
  mylcd.print(F("Homespun 321*!"));
  mylcd.drawChar(150, 25, 'h', FOREGROUND, BACKGROUND, 3);
  mylcd.setCursor(0, 40);
  mylcd.setTextSize(2);
  mylcd.print(7.15);

  TestReset();
}

void DisplayFontsTwo(void) {
  char myString[9] = { '1', '3', ':', '2', '6', ':', '1', '8' };
  uint16_t countUp = 0;
  mylcd.setTextColor(FOREGROUND, BACKGROUND);

  // Test 14
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("big nums Font:"));
  mylcd.setFontNum(UC1609Font_Bignum);
  mylcd.setCursor(0, 15);
  mylcd.print(26.05);  // Test 14a print a Float
  mylcd.setCursor(110, 15);
  mylcd.print(-28);  // Test 14a print a negative number

  TestReset();

  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND);  // Test 14b drawText , 13:26:18 inverted
  mylcd.drawChar(0, 0, '8', FOREGROUND, BACKGROUND);        // Test 14c drawChar
  mylcd.drawChar(160, 0, '4', BACKGROUND, FOREGROUND);      // Test 14d drawChar inverted

  TestReset();
  while (countUp < MYCOUNT_UPTIME)  // Show a count 14e
  {
    mylcd.setCursor(0, 0);
    mylcd.print(countUp++);
    mylcd.LCDupdate();
  }
  TestReset();

  //test 15
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("med nums Font:"));
  mylcd.setFontNum(UC1609Font_Mednum);
  mylcd.setCursor(0, 15);
  mylcd.print(26.43);  // Test 15a print a Float
  mylcd.setCursor(110, 15);
  mylcd.print(-95);  // Test 14a print a negative number

  TestReset();

  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND);  // Test 15b drawText , 13:26:18 inverted
  mylcd.drawChar(0, 0, '8', FOREGROUND, BACKGROUND);        // Test 15c drawChar
  mylcd.drawChar(160, 0, '4', BACKGROUND, FOREGROUND);      // Test 15d drawChar inverted

  TestReset();
  countUp = 0;
  while (countUp < MYCOUNT_UPTIME)  // Show a count 15e
  {
    mylcd.setCursor(0, 0);
    mylcd.print(countUp++);
    mylcd.LCDupdate();
  }
  TestReset();

  // Test 16 Arial round Font
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Arial Round Font:"));
  mylcd.setFontNum(UC1609Font_ArialRound);
  mylcd.setCursor(0, 15);
  mylcd.print(-16.05);
  mylcd.setCursor(0, 40);
  mylcd.print("Arial R");

  TestReset();

  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND);
  mylcd.drawChar(0, 0, 'A', FOREGROUND, BACKGROUND);
  mylcd.drawChar(160, 0, 'R', BACKGROUND, FOREGROUND);

  TestReset();

  // Test 17 Arial bold Font
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Arial bold font:"));
  mylcd.setFontNum(UC1609Font_ArialBold);
  mylcd.setCursor(0, 15);
  mylcd.print(-17.05);
  mylcd.setCursor(0, 40);
  mylcd.print("Arial bold");

  TestReset();

  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND);
  mylcd.drawChar(0, 0, 'A', FOREGROUND, BACKGROUND);
  mylcd.drawChar(160, 0, 'B', BACKGROUND, FOREGROUND);

  TestReset();

  // Test 18 Mia font
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Mia  Font:"));
  mylcd.setFontNum(UC1609Font_Mia);
  mylcd.setCursor(0, 15);
  mylcd.print(-18.05);
  mylcd.setCursor(0, 40);
  mylcd.print("Mia Font");

  TestReset();

  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND);
  mylcd.drawChar(0, 0, 'M', FOREGROUND, BACKGROUND);
  mylcd.drawChar(160, 0, 'A', BACKGROUND, FOREGROUND);

  TestReset();

  // Test 19 Dedica Font
  mylcd.setCursor(0, 0);
  mylcd.setFontNum(UC1609Font_Default);
  mylcd.setTextSize(1);
  mylcd.print(F("Dedica Font:"));
  mylcd.setFontNum(UC1609Font_Mia);
  mylcd.setCursor(0, 15);
  mylcd.setTextColor(FOREGROUND, BACKGROUND);
  mylcd.print(-18.05);
  mylcd.setCursor(0, 40);
  mylcd.print("Dedica Font");

  TestReset();

  mylcd.drawText(0, 32, myString, BACKGROUND, FOREGROUND);
  mylcd.drawChar(0, 0, 'D', FOREGROUND, BACKGROUND);
  mylcd.drawChar(160, 0, 'F', BACKGROUND, FOREGROUND);
  TestReset();
}

// Test 20 Error Checking
// Excepted Ouput
// 11:53:55.109 -> Test 20-A drawChar
// 11:53:55.109 -> 02334
// 11:54:00.343 -> Test 20-A drawText
// 11:54:00.343 -> 0245
// 11:54:05.606 -> Test 20-B drawChar
// 11:54:05.606 -> 02334
// 11:54:10.842 -> Test 20-B drawText
// 11:54:10.842 -> 0254
void ErrorChecking(void) {

  char myStringPASS[] = "WIDE";
  char myStringFAIL[] = "WIzDE";  // Wide font has no lowercase letters
  char* myStringNull = nullptr;

  //20-A font 1-6
  //drawchar
  mylcd.setFontNum(UC1609Font_Wide);
  Serial.println("Test 20-A drawChar");
  Serial.print(mylcd.drawChar(0, 0, 'W', FOREGROUND, BACKGROUND, 1));     // LCD_Success = 0
  Serial.print(mylcd.drawChar(20, 0, 'W', FOREGROUND, BACKGROUND));       // LCD_WrongFont = 2
  Serial.print(mylcd.drawChar(200, 0, 'W', FOREGROUND, BACKGROUND, 1));   // LCD_CharScreenBounds = 3
  Serial.print(mylcd.drawChar(0, 100, 'W', FOREGROUND, BACKGROUND, 1));   // LCD_CharScreenBounds = 3
  Serial.println(mylcd.drawChar(23, 0, 'z', FOREGROUND, BACKGROUND, 1));  // LCD_CharFontASCIIRange = 4
  TestReset();

  //drawText
  Serial.println("Test 20-A drawText");
  Serial.print(mylcd.drawText(0, 0, myStringPASS, FOREGROUND, BACKGROUND, 1));     // LCD_Success = 0
  Serial.print(mylcd.drawText(0, 20, myStringPASS, FOREGROUND, BACKGROUND));       // LCD_WrongFont = 2
  Serial.print(mylcd.drawText(0, 40, myStringFAIL, FOREGROUND, BACKGROUND, 1));    // LCD_CharFontASCIIRange = 4
  Serial.println(mylcd.drawText(0, 40, myStringNull, FOREGROUND, BACKGROUND, 1));  // LCD_CharArrayNullptr = 5
  TestReset();

  //Test 20-b fotns 7-12
  //drawchar
  Serial.println("Test 20-B drawChar");
  mylcd.setFontNum(UC1609Font_Mednum);
  Serial.print(mylcd.drawChar(0, 0, '1', FOREGROUND, BACKGROUND));      // LCD_Success = 0
  Serial.print(mylcd.drawChar(20, 0, '2', FOREGROUND, BACKGROUND, 1));  // LCD_WrongFont = 2
  Serial.print(mylcd.drawChar(200, 0, '3', FOREGROUND, BACKGROUND));    // LCD_CharScreenBounds = 3
  Serial.print(mylcd.drawChar(0, 100, '4', FOREGROUND, BACKGROUND));    // LCD_CharScreenBounds = 3
  Serial.println(mylcd.drawChar(23, 0, 'z', FOREGROUND, BACKGROUND));   // LCD_CharFontASCIIRange = 4
  TestReset();

  //drawText
  Serial.println("Test 20-B drawText");
  mylcd.setFontNum(UC1609Font_ArialBold);
  Serial.print(mylcd.drawText(0, 0, myStringPASS, FOREGROUND, BACKGROUND));      // LCD_Success = 0
  Serial.print(mylcd.drawText(0, 20, myStringPASS, FOREGROUND, BACKGROUND, 1));  // LCD_WrongFont = 2
  Serial.print(mylcd.drawText(0, 40, myStringNull, FOREGROUND, BACKGROUND));     // LCD_CharArrayNullptr = 5
  mylcd.setFontNum(UC1609Font_Mednum);
  Serial.println(mylcd.drawText(0, 40, myStringFAIL, FOREGROUND, BACKGROUND));  // LCD_CharFontASCIIRange = 4
  TestReset();
}

void TestReset(void) {
  mylcd.LCDupdate();  // Write to the buffer
  delay(DISPLAY_DELAY_ONE);
  mylcd.LCDclearBuffer();
}
