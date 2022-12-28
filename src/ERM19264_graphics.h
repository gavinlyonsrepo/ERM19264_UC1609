/*
* Project Name: ERM19264_UC1609
* File: ERM19264_graphics.h
* URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
*/

#ifndef _ERM19264_GRAPHICS_H
#define _ERM19264_GRAPHICS_H

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#define swap(a, b) { int16_t t = a; a = b; b = t; }

typedef enum 
{
    UC1609Font_Default = 1,
    UC1609Font_Thick = 2, // NO LOWERCASE
    UC1609Font_Seven_Seg = 3,
    UC1609Font_Wide = 4, // NO LOWERCASE
    UC1609Font_Tiny = 5,
    UC1609Font_Homespun = 6,
    UC1609Font_Bignum = 7, // NUMBERS + : . ,one size
    UC1609Font_Mednum = 8 // NUMBERS + : . ,one size
}LCD_FONT_TYPE_e;

typedef enum 
{
	UC1609FontWidth_3 = 3,
	UC1609FontWidth_5 = 5, 
	UC1609FontWidth_7 = 7, 
	UC1609FontWidth_4 = 4, 
	UC1609FontWidth_8 = 8,
	UC1609FontWidth_16 = 16
}UC1609FontWidth_e; // width of the font in bits *(N bytes cols).

typedef enum 
{
	UC1609FontOffset_Extend = 0x00, //   extends ASCII
	UC1609FontOffset_Space = 0x20,  // Starts at Space
	UC1609FontOffset_Number = 0x30,  // Starts at number '0'
}UC1609FontOffset_e; // font offset in the ASCII table

typedef enum 
{
	UC1609FontHeight_8 = 8, 
	UC1609FontHeight_16 = 16, 
	UC1609FontHeight_32 = 32
}UC1609FontHeight_e; // height of the font in bits


class ERM19264_graphics : public Print {

 public:

	ERM19264_graphics(int16_t w, int16_t h); // Constructor

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
    
	LCD_FONT_TYPE_e FontNum;

 protected:
	const int16_t WIDTH;
	const int16_t HEIGHT;  

	int16_t _width;
	int16_t _height;
	int16_t cursor_x;
	int16_t cursor_y;
	
	uint8_t textcolor;
	uint8_t textbgcolor;
	
	boolean wrap; // If set, 'wrap' text at right edge of display
	boolean drawBitmapAddr; // True vertical , false = horizontal
		
	uint8_t   textsize;
	uint8_t _FontNumber = 1;
	uint8_t _CurrentFontWidth = 5;
	uint8_t _CurrentFontoffset = 0x00;
	uint8_t _CurrentFontheight = 8;
	
};

#endif 
