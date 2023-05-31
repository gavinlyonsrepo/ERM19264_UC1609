 /*!
	@file ERM19264_graphics.h
	@brief ERM19264 LCD driven by UC1609 controller, header file
		for the graphics  based functions.
	@details Project Name: ERM19264_UC1609
		URL: <https://github.com/gavinlyonsrepo/ERM19264_UC1609>
	@author  Gavin Lyons
*/

#ifndef _ERM19264_GRAPHICS_H
#define _ERM19264_GRAPHICS_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#define ERM19264_swap(a, b) { int16_t t = a; a = b; b = t; }

/*! Enum to define current font type selected  */
enum LCD_FONT_TYPE_e : uint8_t
{
	UC1609Font_Default = 1,      /**<  (1) default  (FUll ASCII with mods) */
	UC1609Font_Thick = 2,          /**<  (2) thick (NO LOWERCASE) */
	UC1609Font_Seven_Seg = 3,  /**<  (3) seven segment  */
	UC1609Font_Wide = 4,            /**<  (4) wide (NO LOWERCASE) */
	UC1609Font_Tiny = 5,            /**<  (5) tiny */
	UC1609Font_Homespun = 6,  /**<  (6) HomeSpun */
	UC1609Font_Bignum = 7,      /**< (7) big numbers  NUMBERS only + : . ,fixed size */
	UC1609Font_Mednum = 8     /**< (8) Med numbers NUMBERS + : . , fixed size */
};

/*! @brief Graphics class to hold graphic related functions */
class ERM19264_graphics : public Print {

 public:

	ERM19264_graphics(int16_t w, int16_t h);

	// This is defined by the subclass:
	virtual void drawPixel(int16_t x, int16_t y, uint8_t color) = 0;

	virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
	virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
	virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
	virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
	virtual void fillScreen(uint8_t color);

	void	drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void	drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
			uint8_t color);
	void	fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
	void	fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
			int16_t delta, uint8_t color);
	void	drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
			int16_t x2, int16_t y2, uint8_t color);
	void	fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
			int16_t x2, int16_t y2, uint8_t color);
	void	drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
			int16_t radius, uint8_t color);
	void	fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
			int16_t radius, uint8_t color);
	void	drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
			int16_t w, int16_t h, uint8_t color, uint8_t bg);
	void	setDrawBitmapAddr(boolean mode);

	void	setCursor(int16_t x, int16_t y);
	void	setTextColor(uint8_t c);
	void	setTextColor(uint8_t c, uint8_t bg);
	void	setTextSize(uint8_t s);
	void	setTextWrap(boolean w);
	void setFontNum(LCD_FONT_TYPE_e FontNumber);
	void drawCharNumFont(uint8_t x, uint8_t y, uint8_t c, uint8_t color , uint8_t bg);
	void drawTextNumFont(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg);
	void	drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);
	void drawText(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg, uint8_t size);

#if ARDUINO >= 100
	virtual size_t write(uint8_t);
#else
	virtual void   write(uint8_t);
#endif

	int16_t height(void) const;
	int16_t width(void) const;

 protected:


	const int16_t WIDTH;  /**< This is the 'raw' display w - never changes */
	const int16_t HEIGHT;  /**< This is the 'raw' display h - never changes*/

	int16_t _width;  /**< Display w as modified by current rotation*/
	int16_t _height;  /**< Display h as modified by current rotation*/
	int16_t cursor_x; /**< Current X co-ord cursor position */
	int16_t cursor_y;  /**< Current Y co-ord cursor position */

	uint8_t textcolor;  /**< Text foreground color */
	uint8_t textbgcolor;   /**< Text background color */
	uint8_t   textsize = 1; /**< Size of text ,fonts 1-6 */
	boolean wrap;                    /**< If set, 'wrap' text at right edge of display*/

	boolean drawBitmapAddr; /**< data addressing mode for method drawBitmap, True-vertical , false-horizontal */

	uint8_t _FontNumber = 1;                /**< Holds current font number */
	uint8_t _CurrentFontWidth = 5;        /**<Holds current font width in bits */
	uint8_t _CurrentFontoffset = 0x00;  /**<Holds current font ASCII table offset */
	uint8_t _CurrentFontheight = 8;      /**<Holds current font height in bits */

	private:

	/*!  Width of the font in bits  *(N bytes cols) */
	enum UC1609FontWidth_e : uint8_t
	{
		UC1609FontWidth_3 = 3,   /**< 3 tiny font */
		UC1609FontWidth_4 = 4,   /**< 4 seven segment font */
		UC1609FontWidth_5 = 5,   /**< 5 default font */
		UC1609FontWidth_7 = 7,   /**< 7 homespun & thick font*/
		UC1609FontWidth_8 = 8,    /**< 8 wide font */
		UC1609FontWidth_16 = 16 /**< Big and Medium number  font */
	}; 
	
	/*! font offset in the ASCII table */
	enum UC1609FontOffset_e : uint8_t
	{
		UC1609FontOffset_Extend = 0x00,   /**<  Full  ASCII */
		UC1609FontOffset_Space = 0x20,     /**< Starts at Space  */
		UC1609FontOffset_Number = 0x30,  /**<  Starts at number '0' */
	}; 

	/*! height of the font in bits */
	enum UC1609FontHeight_e :   uint8_t
	{
		UC1609FontHeight_8 = 8,  /**< 8 font  1-6 at size 1*/
		UC1609FontHeight_16 = 16,  /**< 16 font 8 */
		UC1609FontHeight_32 = 32  /**< 32 font 7 */
	}; 

};

#endif
