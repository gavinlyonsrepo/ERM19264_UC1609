# Changelog

* version 1.0.0 October 2020
	* first release

* version 1.1.0 April 2021
	* Minor update 
	* Typos in Keyword.txt and,  Example file -MISC  removed.
	* Delay <UC1609_RESET_DELAY2> reduced, as ERM19264SBS (white on blue)version of LCD on  Software SPI would not initialise, intermittently.

* version 1.2.0 May 2021
	* Extended ASCII not displaying correctly with No buffer mode on ESP32 issue, solved.
	* "Yield()" statements added to prevent potential  WDT issue's with ESP8266. ESP8266 needs a periodic yield() call to avoid watchdog reset.
	* Added SPI_transactions so LCD can share the SPI bus with other SPI devices on different settings.
	* Added LCDPowerDown function.

* Version 1.3.0 June 2021
	* Added five fonts.
