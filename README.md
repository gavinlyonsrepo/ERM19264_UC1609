![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Hardware](#hardware)
  * [Software](#software)
  * [Files](#files)
  * [Tested](#tested)
  * [Ports](#ports)
  
Overview
--------------------
* Name : ERM19264_UC1609
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller) for the Arduino eco-system
* Description : 

1. Arduino eco-system library.      
2. Inverse, Scroll, rotate and contrast control. 
3. 6 ASCII fonts included.
4. Graphics class included.
5. Sleep mode.
6. 3 different modes: Multi-buffer , single buffer , light weight text only
7. Bitmaps supported.
8. Hardware & software SPI options

* Author: Gavin Lyons
* Arduino IDE: 1.8.10
* History: See Changelog in extras/doc folder
* Copyright: GNU GPL v3

Output
---------------------------------

Output Screenshots, From left to right top to bottom.

1. Full screen bitmap displayed
2. Multi buffer mode screen divided into two buffers
3. Different size and inverted  default font 
4. ASCII font 1-127 printed out with default font size 1 
5. Fonts 5-6
6. Fonts 1-4

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output.jpg)

![op2](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output2.jpg)

Installation
------------------------------

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 

See link below for instruction for this and for the other methods too.

[Installing Additional Arduino Libraries guide](https://www.arduino.cc/en/Guide/Libraries)

Hardware
----------------------------

9 pins , Vcc and GND, anode and cathode for the backlight LED and an SPI interface.
The example files are setup for an UNO for the pin connections used by for other MCU tested see extras folder GPIO_MCU_used.txt file. The backlight control is left up to user.
If  using Hardware SPI two of  pins will be tied to the SPI CLK and MOSI lines if using software SPI you should be able use any GPIO you want for all five pins.
Datasheets are in the extras folder. 

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)
 
1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The library was tested on 1 and 2. 
The Backlight should always be connected to 3.3V according to datasheets. 

The ERM LCD module has a "662k" 3.3V regulator at back and by default setup is a 3.3V device. However if can also be run at 5V and connect to 5V device by 
Modifying  jumper J1 on back. 
This wiring Diagram from the manufacturer showing hardware setup connected to an ~8051 MCU, showing both 3.3 volt and 5 volt systems. NOTE the J1 position. 
If J1 is shorted the LCD can connect to 5V , J1 bypasses the 3.3V regulator. 

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)

Software
-------------------------

*SPI*

Hardware and software SPI. Two different class constructors. User can pick the relevant constructor, see examples files. Hardware SPI is recommended, far faster and more reliable but Software SPI allows for more flexible GPIO selection and easier to port to other MCU' s. When running Software SPI it may be necessary on very high frequency MCU's to change the UC1609_HIGHFREQ_DELAY define, It is a microsecond delay by default it is at 0. All the hardware SPI settings are defined in the header file and can be easily changed if necessary.  It should be able to share SPI bus with other SPI devices on different SPI settings.

*buffers*

3 buffer modes 

1. MULTI_BUFFER (default)
2. SINGLE_BUFFER 
3. NO_BUFFER , Text only no buffer , relatively light weight. Turns LCD into simple character LCD(216 characters). Default font only. Bitmaps can still be written directly to screen but no graphics possible.

To switch between modes, user must make a change to the USER BUFFER OPTION SECTION  at top of 
ERM19264_UC1609.h file.  Pick ONE option and one option ONLY. The example files at top, say which option to pick. If wrong option is picked, example files will not work or maybe even compile.

*fonts*

There are six fonts.
A print class is available to print out most passed data types.
The fonts 1-4 are a byte high(at text size 1) scale-able fonts.
Font 5-6 are specials large fonts but are numbers only and cannot be scaled(just one size).  
Font 5-6 will print just numbers + semi-colons ,  if you print a float using print command
it will place a space and use a circle for a decimal point.

Font data table: 

| Font num | Font enum name | Font size xbyy |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | UC1609Font_Default  | 5x8 |  ASCII 0 - 0xFF, Full Extended | 1275 |
| 2 | UC1609Font_Thick   | 7x8 | ASCII  0x20 - 0x5A , no lowercase letters , | 406 | 
| 3 | UC1609Font_Seven_Seg | 4x8 | ASCII  0x20 - 0x7A | 360 |
| 4 | UC1609Font_Wide | 8x8 | ASCII 0x20 - 0x5A,  no lowercase letters,| 464 |
| 5 | UC1609Font_Bignum | 16x32 | ASCII 0x30-0x3A , Numbers + : only | 704 |
| 6 | UC1609Font_Mednum | 16x16 | ASCII 0x30-0x3A , Numbers + : only | 352 |

By default only Font 1 is commented in and ready to go to save memory.
So to use a non-default Font (2-6), two steps.

1. Comment in the respective define at top of library header file ERM19264_UC1609_graphics_font.h in the USER FONT OPTION ONE section
2. Call SetFontNum function and pass it name of respective font.  eg SetFontNum(UC1609Font_Wide)

*font mods*

The default ASCII font (font one) is an [extended ASCII font](https://www.extended-ascii.com/) 0-255 characters.
If you do not need characters 127-255 and wish to save memory space:
In library header file ERM19264_UC1609_graphics_font.h  in the USER FONT OPTION TWO section
Simply comment this define out. 

1. UC_FONT_MOD_TWO (save 640 bytes) extended ASCII 127-255

You can also remove the first 30 characters if not needed but user will need to change 
ERM19264_ASCII_OFFSET  from 0x00 to 0x20. This will save a further 150 bytes.


*bitmaps*

There is a few different ways of displaying bitmaps, 

| Num | Method | Buffer mode |   Data addressing | Note |
| ------ | ------ | ------ | ------ |  ------ |  
| 1 | LcdBitmap() | any  | Vertical |  Writes directly to screen , no buffer used. | 
| 2 | LcdBuffer() | Multi or Single |  Vertical  |  For internal use mostly | 
| 3 | Multi buffer init  | Multibuffer | Vertical  |  Can be used when initialising a MB | 
| 4 | Single buffer init | Single | Vertical  |  Can be used when initialising SB | 
| 5 | drawBitmap() | Multi or Single | Vertical | default,  setDrawBitmapAddr(true) | 
| 6 | drawBitmap() | Multi or Single |  Horizontal | setDrawBitmapAddr(false) |

See the bitmap example file for more details on each method. Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) , Bitmaps  should be defined in the program memory and buffers in the data memory, for methods 3 & 4 buffers can be initialised with bitmap data.

*User adjustments*

When the user calls LCDbegin() to start LCD they can specify a contrast setting from 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT). Lower contrast works better on the blue version.

It is also possible for user to change LCD bias ,  Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker. See Data sheet for range of values
here. Defaults where found to be fine during all testing of this library.


| Parameter | default Values |  Define | Register |
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + internal V LCD |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49h default|  Set by user with LCDbegin | PM 7:0 |

*Functions*

Functions: Detailed information on the functions can be found in comments in the ERM19264_UC1609.cpp  file and a list of them in keywords.txt. The graphic functions can be found in the ERM19264_graphic.h file. 

Files
-------------------

| Src Files| Desc |
| ------ | ------ |
| ERM19264_UC1609.h | library header file  |
| ERM19264_UC1609.cpp |  library  source file  |
| ERM19264_graphics.h | Custom graphics header file |
| ERM19264_graphics.cpp | Custom graphics source file |
| ERM19264_graphics_font.h | Custom graphics  font  file |

| Examples files ino  | Desc | Buffer mode |
| ------ | ------ | ------ |
| BITMAP | Shows use of bitmaps  | Several see notes | 
| GRAPHICS |  Tests use of graphics   | Multi_buffer |
| MISC | Shows misc functions, rotate, invert etc | Multi_buffer |
| MULTIBUFFER | Shows use of multi buffer mode | Multi_buffer |
| NOBUFFER | Shows use of no buffer text only mode | No_buffer |
| TEXT | Shows use of text and fonts, Fonts must be enabled  | Multi_buffer |
| SINGLEBUFFER | Shows use of single buffer mode | Single_buffer |
| SWSPI | Shows use of software SPI | Multi_buffer |
| HELLO | Hello world basic use case | Multi_buffer |

Tested
-----------------------------

Tested on following MCUs both software and hardware SPI,
The example files are setup for an UNO for the pin connections used 
by for other MCU testing see extras/doc folder GPIO_MCU_used.txt file.

1. Arduino  UNO & NANO v3
2. ESP8266 
3. ESP32 
4. STM32 "blue pill"

Ports
------------------------------------------

* ERM19264_UC1609_RPI , Raspberry pi C++. 
[Link](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI)

* ERM19264_UC1609_T (T for text). Light weight Text only version for arduino ecosystem [Link](https://github.com/gavinlyonsrepo/ERM19264_UC1609_T)

* PIC xc8 C [Link](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

* Stm32cubeIDE STM32F070RBT6  C++ [Link](https://github.com/gavinlyonsrepo/STM32_projects) 

