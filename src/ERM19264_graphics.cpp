 /*!
	@file ERM19264_graphics.cpp
	@brief ERM19264 LCD driven by UC1609 controller, source file
		for the graphics  based functions.
	@details Project Name: ERM19264_UC1609
		URL: <https://github.com/gavinlyonsrepo/ERM19264_UC1609>
	@author  Gavin Lyons
*/

#include "ERM19264_graphics.h"
#include "ERM19264_graphics_font.h"

#ifdef __AVR__
 #include <avr/pgmspace.h>
#else
#ifndef  ESP8266
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #endif
#endif

/*!
	@brief init the LCD Graphics class object
	@param w width defined  in sub-class
	@param h height defined in sub-class
 */
ERM19264_graphics::ERM19264_graphics(int16_t w, int16_t h):
	WIDTH(w), HEIGHT(h)
{
	_width    = WIDTH;
	_height   = HEIGHT;
	_cursorY  = 0;
	_cursorX  = 0;
	_textSize = 1;
	_textColor =     0x00;
	_textBgColor = 0x01;
	_textWrap      = true;
	drawBitmapAddr = true;
}

/*!
	@brief draws a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color The color of the circle
*/
void ERM19264_graphics::drawCircle(int16_t x0, int16_t y0, int16_t r,
		uint8_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0  , y0+r, color);
	drawPixel(x0  , y0-r, color);
	drawPixel(x0+r, y0  , color);
	drawPixel(x0-r, y0  , color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
	
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

/*!
	@brief Used internally by drawRoundRect
*/
void ERM19264_graphics::drawCircleHelper( int16_t x0, int16_t y0,
							 int16_t r, uint8_t cornername, uint8_t color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		} 
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

/*!
	@brief fills a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color color of the filled circle 
*/
void ERM19264_graphics::fillCircle(int16_t x0, int16_t y0, int16_t r,
						uint8_t color) {
	drawFastVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

/*!
	@brief Used internally by fill circle fillRoundRect and fillcircle
*/
void ERM19264_graphics::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, int16_t delta, uint8_t color) {

	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
			drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
			drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}

/*!
	@brief draws a line from (x0,y0) to (x1,y1).
	@param x0 x start coordinate
	@param y0 y start coordinate
	@param x1 x end coordinate
	@param y1 y end coordinate
	@param color color to draw line
*/
void ERM19264_graphics::drawLine(int16_t x0, int16_t y0,
					int16_t x1, int16_t y1,
					uint8_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		ERM19264_swap(x0, y0);
		ERM19264_swap(x1, y1);
	}

	if (x0 > x1) {
		ERM19264_swap(x0, x1);
		ERM19264_swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

/*!
	@brief draws rectangle at (x,y) where h is height and w is width of the rectangle.
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to draw  rect
*/
void ERM19264_graphics::drawRect(int16_t x, int16_t y,
					int16_t w, int16_t h,
					uint8_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h-1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x+w-1, y, h, color);
}

/*!
	@brief Draws a vertical line starting at (x,y) with height h.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param h The height of the line
	@param color The color of the line
*/
void ERM19264_graphics::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint8_t color) {
	drawLine(x, y, x, y+h-1, color);
}

/*!
	@brief Draws a horizontal line starting at (x,y) with width w.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param w The width of the line
	@param color The color of the line 
*/
void ERM19264_graphics::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint8_t color) {
	drawLine(x, y, x+w-1, y, color);
}

/*!
	@brief fills a rectangle starting from coordinates (x,y) with width of w and height of h.
	@param x x coordinate
	@param y y coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 
*/
void ERM19264_graphics::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
					uint8_t color) {
	for (int16_t i=x; i<x+w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

/*!
	@brief Fills the whole screen with a given color.
	@param  color color to fill screen
*/
void ERM19264_graphics::fillScreen(uint8_t color) {
	fillRect(0, 0, _width, _height, color);
}

/*!
	@brief draws a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r radius of the rounded edges
	@param color color to draw rounded rectangle 
*/
void ERM19264_graphics::drawRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r, uint8_t color) {
	drawFastHLine(x+r  , y    , w-2*r, color); // Top
	drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
	drawFastVLine(x    , y+r  , h-2*r, color); // Left
	drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
	// draw four corners
	drawCircleHelper(x+r    , y+r    , r, 1, color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

/*!
	@brief Fills a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r  radius of the rounded edges
	@param color color to fill round  rectangle 
*/
void ERM19264_graphics::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint8_t color) {
	fillRect(x+r, y, w-2*r, h, color);
	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

/*!
	@brief draws a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to draw triangle 
*/
void ERM19264_graphics::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint8_t color) {
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

/*!
	@brief Fills a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to fill  triangle
*/
void ERM19264_graphics::fillTriangle ( int16_t x0, int16_t y0,
					int16_t x1, int16_t y1,
					int16_t x2, int16_t y2, uint8_t color) {

	int16_t a, b, y, last;

	if (y0 > y1) {
		ERM19264_swap(y0, y1); ERM19264_swap(x0, x1);
	}
	if (y1 > y2) {
		ERM19264_swap(y2, y1); ERM19264_swap(x2, x1);
	}
	if (y0 > y1) {
		ERM19264_swap(y0, y1); ERM19264_swap(x0, x1);
	}

	if(y0 == y2) {
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		drawFastHLine(a, y0, b-a+1, color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa   = 0,
		sb   = 0;

	if(y1 == y2) last = y1;   
	else         last = y1-1; 

	for(y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b) ERM19264_swap(a,b);
		drawFastHLine(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b) ERM19264_swap(a,b);
		drawFastHLine(a, y, b-a+1, color);
	}
}

/*!
	@brief Draw a 1-bit color bitmap 
	@param x x co-ord position
	@param y y co-ord posiiton a
	@param bitmap pointer to bitmap data (must be PROGMEM memory) 
	@param w width of the bitmap 
	@param h height of the bitmap 
	@param color foreground colour 
	@param bg background colour.
	@return LCD_Return_Codes_e enum.
	@note Variable drawBitmapAddr controls data addressing
		-# drawBitmapAddr  = true Vertical  data addressing
		-# drawBitmapAddr  = false Horizontal data addressing
		-# A vertical  Bitmap's h must be divisible by 8.for a  bitmap with h=128 & h=64.
		-# Bitmap excepted size = 128 * (64/8) = 1024 bytes.
		-# A horizontal Bitmap's w must be divisible by 8. For a bitmap with w=88 & h=48.
		-# Bitmap excepted size = (88/8) * 48 = 528 bytes.
*/
LCD_Return_Codes_e ERM19264_graphics::drawBitmap(int16_t x, int16_t y,
						const uint8_t *bitmap, int16_t w, int16_t h,
						uint8_t color, uint8_t bg) {
							
// User error checks
// 1. bitmap is null
if (bitmap == nullptr){return LCD_BitmapNullptr ;}
// 2. Completely out of bounds
if (x > _width || y > _height){return LCD_BitmapScreenBounds;}
// 3. bitmap weight and height
if (w > _width || h > _height){return LCD_BitmapLargerThanScreen ;}

if (drawBitmapAddr== true)
{
	// 4A.check vertical bitmap  h must be divisible
	if((h % 8 != 0)){return LCD_BitmapVerticalSize;}
	// Vertical byte bitmaps mode 
	uint8_t vline;
	int16_t i, j, r = 0, yin = y;

	for (i=0; i<(w+1); i++ ) {
		if (r == (h+7)/8 * w) break;
		vline = pgm_read_byte(bitmap + r );
		r++;
		if (i == w) {
			y = y+8;
			i = 0;
		}
		
		for (j=0; j<8; j++ ) {
			if (y+j-yin == h) break;
			if (vline & 0x1) {
				drawPixel(x+i, y+j, color);
			}
			else {
				drawPixel(x+i, y+j, bg);
			}	
			vline >>= 1;
		}
	}
} else if (drawBitmapAddr == false) {
	// 4B.check Horizontal w must be divisible by 8. 
	if((w % 8 != 0)){return LCD_BitmapHorizontalSize;}
	// Horizontal byte bitmaps mode 
	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	for (int16_t j = 0; j < h; j++, y++) 
	{
		for (int16_t i = 0; i < w; i++) 
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
			drawPixel(x+i, y, (byte & 0x80) ? color : bg);
		}
	}

} // end of elseif
return LCD_Success;
}

/*!
	@brief: called by the print class after it converts the data to a character
	@param character The character to write 
*/
#if ARDUINO >= 100
size_t ERM19264_graphics::write(uint8_t character) {
#else
void ERM19264_graphics::write(uint8_t character) {
#endif
	int DrawCharReturnCode;
	if (_FontNumber < UC1609Font_Bignum)
	{
		switch (character)
		{
		case '\n':
			_cursorY += _textSize*_CurrentFontheight;
			_cursorX  = 0;
		break;
		case'\r':/* skip */ break;
		default:
			DrawCharReturnCode = drawChar(_cursorX, _cursorY, character, _textColor, _textBgColor, _textSize) ;
			if (DrawCharReturnCode  != LCD_Success){return DrawCharReturnCode ;}
			_cursorX += _textSize*(_CurrentFontWidth+1);
			if (_textWrap && (_cursorX > (_width - _textSize*(_CurrentFontWidth+1))))
			{
					_cursorY += _textSize*_CurrentFontheight;
					_cursorX = 0;
			}
		break;
		}
	}else // for font numbers 7-12
	{
		switch (character)
		{
			case '\n':
				_cursorY += _CurrentFontheight;
				_cursorX  = 0;
			break;
			case '\r': /* skip */  break;
			default:
				DrawCharReturnCode = drawChar(_cursorX, _cursorY, character, _textColor, _textBgColor) ;
				if (DrawCharReturnCode  != LCD_Success) {return DrawCharReturnCode ;}
				_cursorX += (_CurrentFontWidth);
				if (_textWrap && (_cursorX  > (_width - (_CurrentFontWidth+1))))
				{
					_cursorY += _CurrentFontheight;
					_cursorX = 0;
				}
			break;
		} // end of switch
	} // end of else

#if ARDUINO >= 100
	return 1;
#endif
}

/*!
	@brief  writes a char (c) on the LCD
	@param  x X coordinate
	@param  y Y coordinate
	@param  character The ASCII character
	@param color  color
	@param bg background color
	@param size 1-x
	@return LCD_Return_Codes_e enum.
	@note for font #1-6 only
*/
LCD_Return_Codes_e ERM19264_graphics::drawChar(int16_t x, int16_t y, unsigned char character,
								uint8_t color, uint8_t bg, uint8_t size) {

	// 1. Check for wrong font
	if (_FontNumber >= UC1609Font_Bignum){return LCD_WrongFont;}
	// 2. Check for screen out of  bounds
	if((x >= _width)            || // Clip right
	(y >= _height)           || // Clip bottom
	((x + (_CurrentFontWidth+1) * size - 1) < 0) || // Clip left
	((y + _CurrentFontheight  * size - 1) < 0))   // Clip top
	{
		return LCD_CharScreenBounds;
	}
	// 3. Check for character out of font range bounds
	if ( character < _CurrentFontoffset || character >= (_CurrentFontLength+ _CurrentFontoffset))
	{return LCD_CharFontASCIIRange;}

  for (int8_t i=0; i<(_CurrentFontWidth+1); i++ ) {
    uint8_t line;
    if (i == _CurrentFontWidth)
    { 
      line = 0x0;
    }
    else 
    {
           	switch (_FontNumber) {
#ifdef UC1609_Font_One
				case UC1609Font_Default : line = pgm_read_byte(pFontDefaultptr + ((character - _CurrentFontoffset) * _CurrentFontWidth) + i); break;
#endif 
#ifdef UC1609_Font_Two
				case UC1609Font_Thick :line = pgm_read_byte(pFontThickptr+ ((character - _CurrentFontoffset) * _CurrentFontWidth) + i);  break;
#endif
#ifdef UC1609_Font_Three
				case UC1609Font_Seven_Seg : line = pgm_read_byte(pFontSevenSegptr + ((character - _CurrentFontoffset) * _CurrentFontWidth) + i); break;
#endif
#ifdef UC1609_Font_Four
				case UC1609Font_Wide:  line = pgm_read_byte(pFontWideptr + ((character - _CurrentFontoffset) * _CurrentFontWidth) + i); break;
#endif
#ifdef UC1609_Font_Five
				case UC1609Font_Tiny : line = pgm_read_byte(pFontTinyptr + ((character - _CurrentFontoffset) * _CurrentFontWidth) + i); break;
#endif
#ifdef UC1609_Font_Six
				case UC1609Font_Homespun: line = pgm_read_byte(pFontHomeSpunptr + ((character - _CurrentFontoffset) * _CurrentFontWidth) + i); break;
#endif
				default: // wrong font number
					return LCD_WrongFont;
				break;
				}
    }
    for (int8_t j = 0; j<_CurrentFontheight; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
  return LCD_Success;
}

/*! 
	@brief set the cursor position  
	@param x X co-ord position 
	@param y Y co-ord position
*/
void ERM19264_graphics::setCursor(int16_t x, int16_t y) {
	_cursorX = x;
	_cursorY = y;
}

/*! 
	@brief set the text size , starts at 1 
	@param s Size of text 1-X 
*/
void ERM19264_graphics::setTextSize(uint8_t s) {
	_textSize= (s > 0) ? s : 1;
}

/*! 
	@brief set the text color  
	@param c Color fo text 
*/
void ERM19264_graphics::setTextColor(uint8_t c) {
	_textColor = _textBgColor = c;
}

/*! 
	@brief set the text color  
	@param c Color of text foreground
	@param b color of background of text 
*/
void ERM19264_graphics::setTextColor(uint8_t c, uint8_t b) {
	_textColor   = c;
	_textBgColor = b; 
}

/*!
	@brief turn on or off screen _textWrap of the text (fonts 1-6)
	@param w TRUE on
*/
void ERM19264_graphics::setTextWrap(boolean w) {
	_textWrap = w;
}

/*!
	@brief sets the data addressing mode in drawBitmap function.
	@param  mode boolean mode  , true default
		-# True =  bitmap data vertically addressed 
		-# False = bitmap data horizontally addressed 
*/
void ERM19264_graphics::setDrawBitmapAddr(boolean mode) {
	drawBitmapAddr = mode;
}

/*!
	@brief Gets the width of the display (per current rotation)
	@return width member of display in pixels 
*/
int16_t ERM19264_graphics::width(void) const {return _width;}

 /*!
	@brief Gets the height of the display (per current rotation)
	@return height member of display in pixels 
*/
int16_t ERM19264_graphics::height(void) const {return _height;}

/*!
	@brief   Set the font type
	@param FontNumber  enum LCD_FONT_TYPE_e
*/
void ERM19264_graphics::setFontNum(LCD_Font_Type_e FontNumber) {

	_FontNumber = FontNumber;

	switch (_FontNumber)
	{
		case UC1609Font_Default:  // Norm default 5 by 8
			_CurrentFontWidth = UC1609Font_width_5;
			_CurrentFontoffset = UC1609Font_offset_none;
			_CurrentFontheight = UC1609Font_height_8;
			//_CurrentFontLength = UC1609FontLenAll; (use this for full font hack USER FONT OPTION 2)
			 _CurrentFontLength = UC1609FontLenHalf; 
		break;
		case UC1609Font_Thick: // Thick 7 by 8 (NO LOWERCASE LETTERS)
			_CurrentFontWidth = UC1609Font_width_7;
			_CurrentFontoffset = UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_8;
			_CurrentFontLength = UC1609FontLenAlphaNumNoLCase;
		break;
		case UC1609Font_Seven_Seg:  // Seven segment 4 by 8
			_CurrentFontWidth = UC1609Font_width_4;
			_CurrentFontoffset = UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_8;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		case UC1609Font_Wide: // Wide  8 by 8 (NO LOWERCASE LETTERS)
			_CurrentFontWidth = UC1609Font_width_8;
			_CurrentFontoffset =  UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_8;
			_CurrentFontLength = UC1609FontLenAlphaNumNoLCase;
		break;
		case UC1609Font_Tiny:  // tiny 3 by 8
			_CurrentFontWidth = UC1609Font_width_3;
			_CurrentFontoffset =  UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_8;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		case UC1609Font_Homespun:  // homespun 7 by 8
			_CurrentFontWidth = UC1609Font_width_7;
			_CurrentFontoffset =  UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_8;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		case UC1609Font_Bignum: // big nums 16 by 32 (NUMBERS + : only)
			_CurrentFontWidth = UC1609Font_width_16;
			_CurrentFontoffset = UC1609Font_offset_minus;
			_CurrentFontheight = UC1609Font_height_32;
			_CurrentFontLength = UC1609FontLenNumeric;
		break;
		case UC1609Font_Mednum: // med nums 16 by 16 (NUMBERS + : only)
			_CurrentFontWidth = UC1609Font_width_16;
			_CurrentFontoffset = UC1609Font_offset_minus;
			_CurrentFontheight = UC1609Font_height_16;
			_CurrentFontLength = UC1609FontLenNumeric;
		break;
		case UC1609Font_ArialRound: // Arial round 16 by 24
			_CurrentFontWidth = UC1609Font_width_16;
			_CurrentFontoffset = UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_24;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		case UC1609Font_ArialBold: // Arial bold  16 by 16
			_CurrentFontWidth = UC1609Font_width_16;
			_CurrentFontoffset = UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_16;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		case UC1609Font_Mia: // mia  8 by 16
			_CurrentFontWidth = UC1609Font_width_8;
			_CurrentFontoffset = UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_16;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		case UC1609Font_Dedica: // dedica  6 by 12
			_CurrentFontWidth = UC1609Font_width_6;
			_CurrentFontoffset = UC1609Font_offset_space;
			_CurrentFontheight = UC1609Font_height_12;
			_CurrentFontLength = UC1609FontLenAlphaNum;
		break;
		default:
			_CurrentFontWidth = UC1609Font_width_5;
			_CurrentFontoffset = UC1609Font_offset_none;
			_CurrentFontheight = UC1609Font_height_8;
			 _CurrentFontLength = UC1609FontLenHalf;
			_FontNumber = UC1609Font_Default;
		break;
	}
}

/*!
	@brief writes a char (c) on the LCD
	@param x X coordinate
	@param y Y coordinate
	@param character The ASCII character
	@param color 
	@param bg background color
	@return LCD_Return_Codes_e enum.
	@note for font 7-12 only
*/
LCD_Return_Codes_e ERM19264_graphics::drawChar(uint8_t x, uint8_t y, uint8_t character, uint8_t color , uint8_t bg) 
{
	uint8_t FontSizeMod = 0;
	// Check user input
	// 1. Check for correct font and set FontSizeMod for fonts 7-12
	switch (_FontNumber)
	{
		case UC1609Font_Bignum:
		case UC1609Font_Mednum:
		case UC1609Font_ArialRound:
		case UC1609Font_ArialBold:
			FontSizeMod  = 2;
		break;
		case UC1609Font_Mia:
		case UC1609Font_Dedica:
			FontSizeMod  = 1;
		break;
		default:
			return LCD_WrongFont;
		break;
	}
	// 2. Check for character out of font bounds
	if ( character < _CurrentFontoffset || character >= (_CurrentFontLength + _CurrentFontoffset)){return LCD_CharFontASCIIRange;}
	// 3. Check for screen out of  bounds
	if((x >= _width)            || // Clip right
	(y >= _height)           || // Clip bottom
	((x + _CurrentFontWidth+1) < 0) || // Clip left
	((y + _CurrentFontheight) < 0))   // Clip top
	{return LCD_CharScreenBounds;}

	uint8_t i, j;
	uint8_t ctemp = 0, y0 = y; 

	for (i = 0; i < (_CurrentFontheight*FontSizeMod); i++)
	{
		switch (_FontNumber)
		{
#ifdef UC1609_Font_Seven
			case UC1609Font_Bignum:
				ctemp = pgm_read_byte(&pFontBigNum16x32ptr[character - _CurrentFontoffset][i]); 
			break;
#endif
#ifdef UC1609_Font_Eight
			case UC1609Font_Mednum: 
				ctemp = pgm_read_byte(&pFontMedNum16x16ptr[character - _CurrentFontoffset][i]);  
			break;
#endif
#ifdef UC1609_Font_Nine
			case UC1609Font_ArialRound: 
				ctemp = pgm_read_byte(&pFontArial16x24ptr[character - _CurrentFontoffset][i]); 
			break;
#endif
#ifdef UC1609_Font_Ten
			case UC1609Font_ArialBold: 
				ctemp = pgm_read_byte(&pFontArial16x16ptr[character - _CurrentFontoffset][i]); 	
			break;
#endif
#ifdef UC1609_Font_Eleven
			case UC1609Font_Mia: 
				ctemp = pgm_read_byte(&pFontMia8x16ptr[character - _CurrentFontoffset][i]);
			break;
#endif
#ifdef UC1609_Font_Twelve
			case UC1609Font_Dedica: 
				ctemp = pgm_read_byte(&pFontDedica8x12ptr[character - _CurrentFontoffset][i]); 
			break;
#endif
			default :
				return LCD_WrongFont;
			break;
		}
		
		for (j = 0; j < 8; j++) 
		{
			if (ctemp & 0x80) 
			{
				drawPixel(x, y, color);
			} else {
				drawPixel(x, y, bg);
			}

			ctemp <<= 1;
			y++;
			if ((y - y0) == _CurrentFontheight) {
				y = y0;
				x++;
				break;
			}
	}
	}
	return LCD_Success;
}

/*!
	@brief Writes text string (*ptext) on the LCD
	@param x X coordinate
	@param y Y coordinate
	@param pText pointer to string of ASCII character's
	@param color text color
	@param bg background color
	@return LCD_Return_Codes_e enum.
	@note for font 7-12 only
*/
LCD_Return_Codes_e ERM19264_graphics::drawText(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg) 
{
	// Check correct font number
	if (_FontNumber < UC1609Font_Bignum){return LCD_WrongFont;}
	// Check for null pointer
	if(pText == nullptr){return LCD_CharArrayNullptr ;}
	LCD_Return_Codes_e DrawCharReturnCode;
	while (*pText != '\0') 
	{
		if (x > (_width - _CurrentFontWidth )) 
		{
			x = 0;
			y += _CurrentFontheight ;
			if (y > (_height - _CurrentFontheight)) 
			{
					y = x = 0;
			}
		}
		DrawCharReturnCode = drawChar(x, y, *pText, color, bg);
		if (DrawCharReturnCode  != LCD_Success) {return DrawCharReturnCode ;}
		x += _CurrentFontWidth ;
		pText++;
	}
	return LCD_Success;
}

/*!
	@brief Writes text string on the LCD
	@param x X coordinate
	@param y Y coordinate
	@param pText pointer to string/array
	@param color text color
	@param bg background color
	@param size 1-x
	@return LCD_Return_Codes_e enum.
	@note for font #1-6 only
*/
LCD_Return_Codes_e ERM19264_graphics::drawText(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg, uint8_t size) {
	// check Correct font number
	if(_FontNumber >= UC1609Font_Bignum){return LCD_WrongFont;}
	// Check for null pointer
	if(pText == nullptr){return LCD_CharArrayNullptr ;}
	
	LCD_Return_Codes_e DrawCharReturnCode;
	uint8_t lcursorX;
	uint8_t lcursorY;
	lcursorX = x; 
	lcursorY = y;

	while (*pText != '\0')
	{
		if (_textWrap && ((lcursorX + size * _CurrentFontWidth) > _width))
		{
			lcursorX = 0;
			lcursorY = lcursorY + size * 7 + 3;
			if (lcursorY > _height) lcursorY = _height;
		}
		DrawCharReturnCode = drawChar(lcursorX, lcursorY , *pText, color, bg, size);
		if (DrawCharReturnCode  != LCD_Success) {return DrawCharReturnCode ;}
		lcursorX = lcursorX + size * (_CurrentFontWidth + 1);
		if (lcursorX > _width) lcursorX = _width;
		pText++;
	}
	return LCD_Success;
}

/*!
   @brief Gets the _rotation of the display
   @return rotation enum value 0-3
*/
LCD_rotate_e ERM19264_graphics::getRotation(void){return LCD_rotate;}

/*!
   @brief Sets the rotation of the display
   @param CurrentRotation rotation enum value 0-3
   @details Note this is separate from LCD command method for rotation. This rotates the Software buffer.
*/
void ERM19264_graphics::setRotation(LCD_rotate_e CurrentRotation)
{
	switch (CurrentRotation)
	{
	case 0:
	case 2:
		_width = WIDTH;
		_height = HEIGHT;
		break;
	case 1:
	case 3:
		_width = HEIGHT;
		_height = WIDTH;
		break;
	}
	LCD_rotate = CurrentRotation;
}
