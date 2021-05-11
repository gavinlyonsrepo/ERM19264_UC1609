![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Hardware](#hardware)
  * [Features](#features)
  * [Files](#files)
  * [Tested_MCU](#tested_mcu)
  * [Ports](#ports)
  
Overview
--------------------
* Name : ERM19264_UC1609
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller) for the Arduino eco-system
* Description : 

1. Arduino eco-system library.      
2. Inverse, Scroll, rotate and contrast control. 
3. Extended  scale-able ASCII font.
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
3. Different size and type of fonts 
4. Available ASCII font printed out, this can be expanded to [extended ASCII font](https://www.extended-ascii.com/) by minor modification to font file see features.

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/output.jpg)


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
The UC1609 controller chip is a 3.3V device but the ERM LCD module has a "662k" 3.3V regulator at back.
So the ERM LCD module will run at 5V as well if this is present.
It was always run it at 3.3V during testing. 
The Backlight should always be connected to 3.3V according to datasheets.  

This wiring Diagram from the manufacturer showing hardware setup connected to an ~8051 MCU.
Showing both 5 volt and 3.3 volt systems. NOTE the J1 position. 

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/connect.jpg)

Features
-------------------------

*SPI*

Hardware and software SPI. Two different class constructors. User can pick the relevant constructor, see examples files. Hardware SPI is recommended, far faster and more reliable but Software SPI allows for more flexible GPIO selection and easier to port to other MCU' s. When running Software SPI it may be necessary on very high frequency MCU's to change the UC1609_HIGHFREQ_DELAY define, It is a microsecond delay by default it is at 0. All the hardware SPI settings are defined in the header file and can be easily changed if necessary.  It should be able to share SPI bus with other SPI devices on different SPI settings.

*buffers*

3 buffer modes 

1. MULTI_BUFFER (default)
2. SINGLE_BUFFER 
3. NO_BUFFER , Text only no buffer , relatively light weight. A "hello world" Sketch uses 2320 bytes (7%) of and 42 bytes (2%) of dynamic memory. Turns LCD into simple character LCD(216 characters)
Bitmaps can still be written directly to screen but no graphics possible.

To switch between modes, user must make a change to the USER BUFFER OPTION SECTION  at top of 
ERM19264_UC1609.h file.  Pick ONE option and one option ONLY. The example files at top, say which option to pick. If wrong option is picked, example files will not work or maybe even compile.


*fonts*

The ASCII font(in the custom_graphics_font.h file) is truncated by a define ( UC_FONT_MOD_TWO) after first 127 characters (see output pic) to save memory space(640 bytes), if you wish to use rest of the [extended ASCII font](https://www.extended-ascii.com/), simply comment this define out.
The font is a standard 5 by 7 ASCII font with two  columns  of padding added. So 7 by 8 in effect. In standard text size and "no buffer" mode, this means: 192/7 * 64/8 = 27 * 8 = 216 characters. 
There is also a light weight Text only version of library available [here at link](https://github.com/gavinlyonsrepo/ERM19264_UC1609_T)

*bitmaps*

Bitmaps are written directly to screen unless Bitmap set to a buffer.
Best to use multi-buffer to share screen between bitmaps and text + graphics.
Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) use vertical addressing draw mode. 

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
| GRAPHICS |  Shows use of graphics   | Multi_buffer |
| MISC | Shows misc functions, rotate invert etc | Multi_buffer |
| MULTIBUFFER | Shows use of multi buffer mode | Multi_buffer |
| NOBUFFER | Shows use of no buffer text only mode | No_buffer |
| TEXT | Shows use of text IN buffer mode   | Multi_buffer |
| SINGLEBUFFER| Shows use of single buffer mode | Single_buffer |
| SWSPI | Shows use of software SPI | Multi_buffer |

Tested_MCU
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

* Raspberry Pi SBC C++ [Link](https://github.com/gavinlyonsrepo/ERM19264_UC1609_RPI)

* PIC C XC8  [Link](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

* Stm32cubeIDE STM32F070RBT6  C++ [Link](https://github.com/gavinlyonsrepo/STM32_projects) 

