
// Example file name : ERM19264_UC1609_NOBUFFER.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of "No buffer" mode . 
// in this mode the LCD can be used as a relatively light weight character LCD
// 8 rows and 216 characters, 7by8 ,font size (192/7 * 64/8 ), 
// you can also write bitmaps directly to screen in this mode.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme.
// (2) In the <ERM19264_UC1609.h> USER BUFFER OPTION SECTION, at top of file
// option NO_BUFFER must be selected and only this option. 
// (3) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ******************************

#include <ERM19264_UC1609.h>
#include <ERM19264_graphics_font.h> // include just to see font mod. UC_FONT_MOD_TWO for test 5

#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. user adjust

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609  mylcd(CD, RST, CS); 

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // clear screen
  delay(500);
}

// ************** MAIN LOOP ***********
void loop() {
  while(1)
  {
    Tests();
  }
}
// ************** END OF MAIN ***********

//Function to run a group of tests
// Test 0 clear screen 
// Test 1 String function and goto function 
// Test 2 clear page function
// Test 3 character function
// Test 4 print ASCII font with character function 0-127
// Test 5 print extended ASCII  font 128-255

void Tests()
{
  unsigned char test0_1[] = "No buffer mode 216 chars";
  unsigned char test0_2[] = "(192/7 * 64/8)= 216";

  unsigned char test1_1[] = "This is page 0";
  unsigned char test1_2[] = "123456789012345678901234567";
  unsigned char test1_3[] = "This is page 7";
    
    // Test 0 clear screen 
    mylcd.LCDNoBufferGotoXY(0, 0);
    mylcd.LCDNoBufferString(test0_1); // Write a character string
    mylcd.LCDNoBufferGotoXY(0, 1);
    mylcd.LCDNoBufferString(test0_2); // Write a character string
    delay(5000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen
    delay(50);
    
    // Test 1 String function and goto function  
    mylcd.LCDNoBufferGotoXY(0, 0); // (Col , page ) Col 0 to 191 , page 0 to 7
    mylcd.LCDNoBufferString(test1_1); // Write a character string
    mylcd.LCDNoBufferGotoXY(0, 4);
    mylcd.LCDNoBufferString(test1_2);
    mylcd.LCDNoBufferGotoXY(0, 7);
    mylcd.LCDNoBufferString(test1_3);
    delay(5000);

    // Test 2 clear page function 
    mylcd.LCDNoBufferGotoXY(0, 7);
    mylcd.LCDFillPage(0x00); // Clear page
    mylcd.LCDNoBufferGotoXY(0, 6);
    mylcd.LCDFillPage(0x7E); // Write pattern (0111 1110) to a page
    delay(5000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen

    // Test 3 character function 
    mylcd.LCDNoBufferGotoXY(100, 2);
    mylcd.LCDNoBufferChar('G');  // write single  character
    mylcd.LCDNoBufferChar('L');
    delay(5000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen

    // Test 4 print entire font with character function
    // For characters after 'z{|}' in ASCII table user can comment out UC_FONT_MOD_TWO in font file (NOTE: this will increase program size)
    mylcd.LCDNoBufferGotoXY(0, 0);
    uint8_t row = 1;
    for(char i=1; i < 126 ; i++)
    {
      if (i % 27 == 0) mylcd.LCDNoBufferGotoXY(0, row++);
      mylcd.LCDNoBufferChar(i); 
      delay(50);
    }
    delay(7000);
     mylcd.LCDFillScreen(0x00, 0); // Clear the screen
        
  // TEST 5 print ASCII font 128-255 with character function
  // For characters after 'z{|}' in ASCII table user can comment out UC_FONT_MOD_TWO in font file
  // (NOTE: this will increase program size 635 bytes
#ifndef UC_FONT_MOD_TWO
  mylcd.LCDNoBufferGotoXY(0, 0);
  row = 1;
  unsigned char k = 1;
  for (k = 128; k < 255 ; k++)
  {
    if (k % 27 == 0) mylcd.LCDNoBufferGotoXY(0, row++);
    mylcd.LCDNoBufferChar(k);
    delay(50);
  }
  delay(5000);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
#endif

}
