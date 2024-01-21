/*!
* @file ERM19264_graphics_font.h
* @brief  ERM19264_UC1609 , ERM19264 LCD driven by UC1609C controller, font data file 12 fonts.
* @author Gavin Lyons.
* @details <https://github.com/gavinlyonsrepo/ERM19264_UC1609>
*/

#ifndef ERM19264_FONT_S_H
#define ERM19264_FONT_S_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifdef __AVR__
 #include <avr/io.h>
 #include <avr/pgmspace.h>
#else
#ifndef ESP8266
 #define PROGMEM
#endif
#endif
 
/*!
	@brief USER FONT OPTION ONE , FONT DEFINE SECTION 
	@note Comment in the fonts YOU want, UC1609_Font_One is default. 
		-#  UC1609_Font_One  default  (FUll ASCII with mods)
		-#  UC1609_Font_Two  thick (NO LOWERCASE)
		-#  UC1609_Font_Three seven segment 
		-#  UC1609_Font_Four wide (NO LOWERCASE)
		-#  UC1609_Font_Five  tiny
		-#  UC1609_Font_Six Home Spun
		-#  UC1609_Font_Seven big numbers ( extended Numeric )
		-#  UC1609_Font_Eight Medium numbers (extended Numeric )
		-#  UC1609_Font_Nine Arial round
		-#  UC1609_Font_Ten  Arial bold
		-# UC1609_Font_Eleven  Mia
		-# UC1609_Font_Twelve  dedica
*/

#define UC1609_Font_One         /**< (1) default  (FUll ASCII with mods) */ 
//#define UC1609_Font_Two       /**< (2) thick (NO LOWERCASE) */
//#define UC1609_Font_Three    /**<  (3) seven segment  */
//#define UC1609_Font_Four      /**<  (4) wide (NO LOWERCASE) */
//#define UC1609_Font_Five      /**< (5) tiny */
//#define UC1609_Font_Six       /**< (6) HomeSpun */
//#define UC1609_Font_Seven /**<  (7) big numbers ( extended Numeric )*/
//#define UC1609_Font_Eight /**< (8) Med numbers (extended Numeric )*/
//#define UC1609_Font_Nine /**< (9) Arial round*/
//#define UC1609_Font_Ten  /**<  (10) Arial bold*/
//#define UC1609_Font_Eleven /**< (11) Mia*/
//#define UC1609_Font_Twelve  /**< (12) dedica*/



/*! 
 * @brief USER FONT OPTION TWO
	@details This is defined to include full extended ASCII set 127-255, Will increase program size (635 bytes ) if enabled. 
	if commented in  _CurrentFontLength  in setFontNum must also be edited to 
	_CurrentFontLength = UC1609FontLenAll;
	@note  UC_Font_One only.
*/
//#define UC_FONT_MOD_TWO /**< USER OPTION 2 */

 
// Font data is in the cpp file accessed thru extern pointers.

#ifdef UC1609_Font_One 
extern const unsigned char * pFontDefaultptr; /**< Pointer to Default  font data */
#endif
#ifdef UC1609_Font_Two 
extern const unsigned char * pFontThickptr; /**< Pointer to thick font data */
#endif
#ifdef UC1609_Font_Three
extern const unsigned char * pFontSevenSegptr; /**< Pointer to Seven segment font data */
#endif
#ifdef UC1609_Font_Four
extern const unsigned char * pFontWideptr; /**< Pointer to Wide font data */
#endif
#ifdef UC1609_Font_Five
extern const  unsigned char * pFontTinyptr; /**< Pointer to Tiny font data */
#endif
#ifdef UC1609_Font_Six
extern const unsigned char * pFontHomeSpunptr; /**< Pointer to Home Spun font data */
#endif
#ifdef UC1609_Font_Seven
extern const uint8_t (* pFontBigNum16x32ptr)[64]; /**< Pointer to Big Numbers font data */
#endif
#ifdef UC1609_Font_Eight
extern const uint8_t (* pFontMedNum16x16ptr)[32];  /**< Pointer to Medium  Numbers font data */
#endif
#ifdef UC1609_Font_Nine
extern const uint8_t (* pFontArial16x24ptr)[48]; /**< Pointer to Arial Round font data */
#endif
#ifdef UC1609_Font_Ten
extern const uint8_t (* pFontArial16x16ptr)[32]; /**< Pointer to Arial bold font data */
#endif
#ifdef UC1609_Font_Eleven
extern const uint8_t (* pFontMia8x16ptr)[16]; /**< Pointer to Mia font data */
#endif
#ifdef UC1609_Font_Twelve
extern const uint8_t (* pFontDedica8x12ptr)[12]; /**< Pointer to dedica font data */
#endif


#endif // font file guard header
