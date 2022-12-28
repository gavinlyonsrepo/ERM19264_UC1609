# Changelog

* version 1.0.0 October 2020
	* first release

* version 1.1.0 April 2021
	* Minor update 
	* Typos in Keyword.txt and,  Example file -MISC  removed.
	* Delay "UC1609_RESET_DELAY2" reduced, as ERM19264SBS (white on blue)version of LCD on  Software SPI would not initialise, intermittently.

* version 1.2.0 May 2021
	* Extended ASCII not displaying correctly with No buffer mode on ESP32 issue, solved.
	* "Yield()" statements added to prevent potential  WDT issue's with ESP8266. ESP8266 needs a periodic yield() call to avoid watchdog reset.
	* Added SPI_transactions so LCD can share the SPI bus with other SPI devices on different settings.
	* Added LCDPowerDown function.

* Version 1.3.0 June 2021
	* Added five fonts.

* Version 1.4.0 Feb 2022
	* drawBitmap() function is now set up for both horizontal and vertical addressed bitmap data,
	as per pull request  #2 from Myaflick on github.  Vertical is default. The addressing mode is changed by setDrawBitmapAddr(), new function.  
	* Enum added for font name labels instead of raw numbers, This will cause font compiler warnings for sketch's written on versions before 1.4.0 , The sketch's will still compile and work,
	Simply replace font numbers  with the relevant enum text labels to get rid of warnings. 
	* A new function added for initialising a multibuffer struct , LCDinitBufferStruct(),
	backward compatible with old manual method.  

* Version 1.5.0 December 2022
	* Added DrawText method.
	* Added font file X.cpp.
	* Added Fonts "tiny" & "homespun".
	* Added negative sign for Fonts 7&8 when using "print" method.
	* Changed "UC1609_POWER_CONTROL" from 0x2F to 0x28. This sets it to the excepted datasheet default  , 1.4mA.  
  