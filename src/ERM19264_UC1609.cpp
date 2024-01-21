/*!
	@file ERM19264_UC1609.cpp
	@brief ERM19264 LCD driven by UC1609C controller, Source file.
	@author Gavin Lyons.
	@details Project Name: ERM19264_UC1609 	<https://github.com/gavinlyonsrepo/ERM19264_UC1609>
*/

#include "ERM19264_UC1609.h"

/*!
	@brief init the screen/sharedBuffer object
	@param mybuffer Pointer to buffer array data(arrays decay to  pointers)
	@param w width of buffer
	@param h height of buffer
	@param x offset of buffer
	@param y offset of buffer
 */
ERM19264_UC1609_Screen::ERM19264_UC1609_Screen(uint8_t* mybuffer, uint8_t w,  uint8_t h, int16_t  x, int16_t y)
{
	screenBuffer = mybuffer; // pointer to buffer
	width = w;   // bitmap x size
	height = h; // bitmap y size
	xoffset = x; // x offset
	yoffset = y; // y offset
}

// Class Constructors main  class, 2 off Hardware + software SPI

/*!
	@brief init the LCD  class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
	@param cd GPIO data or command
	@param rst GPIO reset
	@param cs GPIO Chip select
	@note Hardware SPI version , sub class of ERM19264_graphics
 */
ERM19264_UC1609::ERM19264_UC1609(int16_t lcdwidth , int16_t lcdheight ,int8_t cd, int8_t rst, int8_t cs) :ERM19264_graphics(lcdwidth, lcdheight)
{
	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_CS = cs;
	_LCD_DIN = -1;   // -1 for din and sclk specify using hardware SPI
	_LCD_SCLK = -1;
	
	_widthScreen = lcdwidth ; 
	_heightScreen = lcdheight ;
}

/*!
	@brief init the LCD  class object
	@param lcdwidth width of LCD in pixels
	@param lcdheight height of LCD in pixels
	@param cd GPIO data or command
	@param rst GPIO reset
	@param cs GPIO Chip select
	@param sclk GPIO SPI Clock
	@param din GPIO MOSI
	@note Software  SPI version , sub class of ERM19264_graphics
 */
ERM19264_UC1609::ERM19264_UC1609(int16_t lcdwidth , int16_t lcdheight ,int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din) : ERM19264_graphics(lcdwidth, lcdheight )

{
	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_CS = cs;
	_LCD_DIN = din;
	_LCD_SCLK = sclk;
	
	_widthScreen = lcdwidth ; 
	_heightScreen = lcdheight ;
}

// === Methods ===

/*!
	@brief begin Method initialise LCD  Sets pinmodes and SPI setup
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
 */
void ERM19264_UC1609::LCDbegin (uint8_t VbiasPOT, uint8_t AddressSet)
{
	pinMode(_LCD_CD , OUTPUT);
	pinMode(_LCD_RST, OUTPUT);
	pinMode(_LCD_CS, OUTPUT);

	_VbiasPOT  = VbiasPOT;
	
	if (AddressSet > 7 ) // Check User input. Address set cannot exceed 7.
	{
		AddressSet = UC1609_ADDRESS_SET;
	}
	_AddressCtrl = AddressSet;

	if (isHardwareSPI())
	{
		SPI.begin();
	}else
	{
	 // Set software SPI specific pin outputs.
		pinMode(_LCD_DIN, OUTPUT);
		pinMode(_LCD_SCLK, OUTPUT);
	}

	LCDinit();
}

/*!
	@brief Called from LCDbegin carries out Power on sequence and register init
	Can be used to reset LCD to default values.
*/
void ERM19264_UC1609::LCDinit()
 {
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CD_SetHigh;
	UC1609_CS_SetHigh;
	delay(UC1609_INIT_DELAY2);

	LCDReset();

	UC1609_CS_SetLow;

	send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET);
	send_command(UC1609_ADDRESS_CONTROL, _AddressCtrl);
	send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);
	send_command(UC1609_POWER_CONTROL,  UC1609_PC_SET);
	delay(UC1609_INIT_DELAY);

	send_command(UC1609_GN_PM, 0);
	send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user

	send_command(UC1609_DISPLAY_ON, 0x01); // turn on display
	send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal

	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}


/*!
	 @brief Sends a command to the display
	 @param command Command to send
	 @param value the values to change
	 @note command and value  will be combined with OR
*/
void ERM19264_UC1609::send_command (uint8_t command, uint8_t value)
{
	UC1609_CD_SetLow;
	send_data(command | value);
	UC1609_CD_SetHigh;
}

/*!
	@brief Resets LCD in a four wire setup called at start
	and  should also be called in a controlled power down setting
*/
void ERM19264_UC1609::LCDReset ()
{
	UC1609_RST_SetLow;
	delay(UC1609_RESET_DELAY);
	UC1609_RST_SetHigh;
	delay(UC1609_RESET_DELAY2);
}

/*!
	@brief Powerdown procedure for LCD see datasheet P40
*/
void ERM19264_UC1609::LCDPowerDown(void)
{
	LCDReset ();
	LCDEnable(0);
	if(isHardwareSPI()) {SPI.end();}
}


/*!
	 @brief Turns On Display
	 @param bits  1  display on , 0 display off
*/
void ERM19264_UC1609::LCDEnable (uint8_t bits)
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_DISPLAY_ON, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Scroll the displayed image up by SL rows.
	@details The valid SL value is between 0 (for no
		scrolling) and (64).
		Setting SL outside of this range causes undefined effect on the displayed image.
	@param bits 0-64 line number y-axis
*/
void ERM19264_UC1609::LCDscroll (uint8_t bits)
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_SCROLL, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Rotates the display
	@details Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
		Param1: 4 possible values 000 010 100 110 (defined)
	@note If Mx is changed the buffer must BE updated see examples. 
*/
void ERM19264_UC1609::LCDrotate(uint8_t rotatevalue)
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	switch (rotatevalue)
	{
		case 0: rotatevalue = 0; break;
		case 0x02: rotatevalue = UC1609_ROTATION_FLIP_ONE; break;
		case 0x04: rotatevalue = UC1609_ROTATION_NORMAL; break;
		case 0x06: rotatevalue = UC1609_ROTATION_FLIP_TWO; break;
		default: rotatevalue = UC1609_ROTATION_NORMAL; break;
	}
	send_command(UC1609_LCD_CONTROL, rotatevalue);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	 @brief invert the display
	 @param bits 1 invert , 0 normal
*/
void ERM19264_UC1609::LCDInvertDisplay (uint8_t bits)
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_INVERSE_DISPLAY, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief turns on all Pixels
	@param bits Set DC[1] to force all SEG drivers to output ON signals
	1 all on ,  0 all off
*/
void ERM19264_UC1609::LCDallpixelsOn(uint8_t bits)
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_ALL_PIXEL_ON, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay optional delay in microseconds can be set to zero normally.
*/
void ERM19264_UC1609::LCDFillScreen(uint8_t dataPattern=0, uint8_t delay=0)
{
	 if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	 UC1609_CS_SetLow;
	uint16_t numofbytes = _widthScreen * (_heightScreen/8); // width * height
	for (uint16_t i = 0; i < numofbytes; i++)
	{
			send_data(dataPattern);
			delayMicroseconds(delay);
	}
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	 @brief Fill the chosen page at cursor  with a datapattern
	 @param dataPattern can be set to 0 to FF (not buffer)
*/
void ERM19264_UC1609::LCDFillPage(uint8_t dataPattern=0)
{
	 if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	 UC1609_CS_SetLow;
	uint16_t numofbytes = ((_widthScreen * (_heightScreen/8))/8); // (width * height/8)/8 = 192 bytes
	for (uint16_t i = 0; i < numofbytes; i++)
	{
		send_data(dataPattern);
	}
	 UC1609_CS_SetHigh;
	 if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	 @brief Goes to X Y position
	 @param  column Column 0-192
	 @param page 0-7
*/
void ERM19264_UC1609::LCDGotoXY(uint8_t column , uint8_t page)
{
        UC1609_CS_SetLow;
        send_command(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
        send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
        send_command(UC1609_SET_PAGEADD, page++); 
        UC1609_CS_SetHigh;
}

/*!
	 @brief Draw a bitmap in PROGMEM to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data  pointer to the bitmap must be in PROGMEM
	 @return LCD_Return_Codes_e enum.
*/
LCD_Return_Codes_e ERM19264_UC1609::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data)
{
// User error checks
// 1. bitmap is null
if (data == nullptr){return LCD_BitmapNullptr ;}
// 2. Start point Completely out of bounds
if (x > _width || y > _height){return LCD_BitmapScreenBounds;}
// 3. bitmap weight and height
if (w > _width || h > _height){return LCD_BitmapLargerThanScreen ;}
// 4A.check vertical bitmap h must be divisible
if((h % 8 != 0)){return LCD_BitmapVerticalSize;}

#if defined(ESP8266)
	// ESP8266 needs a periodic yield() call to avoid watchdog reset.
	yield();
#endif
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >>3;

	for (ty = 0; ty < h; ty = ty + 8)
	{
		if (y + ty < 0 || y + ty >= _heightScreen) {continue;}
		send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
		send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
		send_command(UC1609_SET_PAGEADD, page++);

		for (tx = 0; tx < w; tx++)
		{
			if (x + tx < 0 || x + tx >= _widthScreen) {continue;}
			offset = (w * (ty >> 3)) + tx;
			send_data(pgm_read_byte(&data[offset]));
		}
	}
	UC1609_CS_SetHigh;
	#if defined(ESP8266)
		yield();
	#endif
	 if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
	 return LCD_Success;
}

/*!
	 @brief Checks if software SPI is on
	 @returns true 1 if hardware SPi on , false 0 for software spi
*/
bool ERM19264_UC1609::isHardwareSPI() {
	return (_LCD_DIN == -1 && _LCD_SCLK == -1);
}

/*!
	 @brief used in software SPI mode to shift out data
	 @param bitOrder LSB or MSB set to MSBFIRST for UC1609C
	 @param value the byte to go out 
	 @details if using high freq MCU the delay define can be changed by LCDHighFreqDelaySet function . Default is at UC1609_HIGHFREQ_DELAY
*/
void ERM19264_UC1609::CustomshiftOut(uint8_t bitOrder, uint8_t value)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		if (bitOrder == LSBFIRST)
			!!(value & (1 << i)) ?  UC1609_SDA_SetHigh :  UC1609_SDA_SetLow;
		else
			!!(value & (1 << (7 - i))) ?  UC1609_SDA_SetHigh :  UC1609_SDA_SetLow;

		UC1609_SCLK_SetHigh ;
		delayMicroseconds(_HighFreqDelay);
		UC1609_SCLK_SetLow;
		delayMicroseconds(_HighFreqDelay);
	}
}


/*!
	 @brief Send data byte with SPI to UC1609
	 @param byte the data byte to send 
*/
void ERM19264_UC1609::send_data(uint8_t byte)
{
	if (isHardwareSPI())
	{
		(void)SPI.transfer(byte); // Hardware SPI
	}else
	{
		CustomshiftOut(MSBFIRST, byte); //Software SPI
	}
}


/*!
	 @brief updates the LCD  i.e. writes the  shared buffer to the active screen
		pointed to by ActiveBuffer
*/
void ERM19264_UC1609::LCDupdate()
{
	LCDBuffer( this->ActiveBuffer->xoffset, this->ActiveBuffer->yoffset, this->ActiveBuffer->width, this->ActiveBuffer->height, (uint8_t*) this->ActiveBuffer->screenBuffer);
}

/*!
	 @brief clears the buffer of the active screen pointed to by ActiveBuffer 
	 @note Does NOT write to the screen
*/
void ERM19264_UC1609::LCDclearBuffer()
{
	 memset( this->ActiveBuffer->screenBuffer, 0x00, (this->ActiveBuffer->width * (this->ActiveBuffer->height/ 8))  );
}

/*!
	 @brief Draw a data array  to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data pointer to the data array
	 @note Called by LCDupdate internally to write buffer to screen , can be called standalone 	as well
*/
void ERM19264_UC1609::LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data)
{
 #if defined(ESP8266)
	yield();
#endif
 if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
 UC1609_CS_SetLow;

	uint8_t tx, ty;
	uint16_t offset = 0;
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y/8;

	for (ty = 0; ty < h; ty = ty + 8)
	{
	if (y + ty < 0 || y + ty >= _heightScreen) {continue;}

	send_command(UC1609_SET_COLADD_LSB, (column & 0x0F));
	send_command(UC1609_SET_COLADD_MSB, (column & 0XF0) >> 4);
	send_command(UC1609_SET_PAGEADD, page++);

	for (tx = 0; tx < w; tx++)
	{
			if (x + tx < 0 || x + tx >= _widthScreen) {continue;}
			offset = (w * (ty /8)) + tx;
			send_data(data[offset++]);
	}
	}

UC1609_CS_SetHigh;
 if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
#if defined(ESP8266)
	yield();
#endif
}

/*!
	@brief Draws a Pixel to the screen , overrides the  graphics library 
	@param x x co-ord of pixel
	@param y y co-ord of pixel
	@param colour colour of  pixel
*/
void ERM19264_UC1609::drawPixel(int16_t x, int16_t y, uint8_t colour)
{
	// Check Boundary.
	if ((x < 0) || (x >= this->ActiveBuffer->width) || (y < 0) || (y >= this->ActiveBuffer->height)) {
	return ;
	}
	
	// Check rotation 
	int16_t temp;
	uint8_t RotateMode = getRotation();
	switch (RotateMode) {
	case 1:
		temp = x;
		x = WIDTH - 1 - y;
		y = temp;
	break;
	case 2:
		x = WIDTH - 1 - x;
		y = HEIGHT - 1 - y;
	break;
	case 3:
		temp = x;
		x = y;
		y = HEIGHT - 1 - temp;
	break;
	}
	
	// Draw the pixel
	uint16_t offset = (this->ActiveBuffer->width * (y/8)) + x;
	switch (colour)
	{
		case FOREGROUND: this->ActiveBuffer->screenBuffer[offset] |= (1 << (y & 7)); break;
		case BACKGROUND: this->ActiveBuffer->screenBuffer[offset] &= ~(1 << (y & 7)); break;
		case COLORINVERSE: this->ActiveBuffer->screenBuffer[offset] ^= (1 << (y & 7)); break;
	}
}

/*!
	@brief Getter for _VbiasPOT contrast member
	@return value of_VbiasPOT
*/
uint8_t ERM19264_UC1609::LCDGetConstrast(void){return _VbiasPOT;}

/*!
	@brief Getter for _AddressCtrl Display RAM address control member
	@return value of _AddressCtrl
*/
uint8_t ERM19264_UC1609::LCDGetAddressCtrl(void){return _AddressCtrl;}


/*!
	@brief Library version number getter
	@return The lib version number eg 180 = 1.8.0
*/
uint16_t ERM19264_UC1609::LCDLibVerNumGet(void) {return _LibVersionNum;}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ERM19264_UC1609::LCDHighFreqDelayGet(void){return _HighFreqDelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ERM19264_UC1609::LCDHighFreqDelaySet(uint16_t CommDelay){_HighFreqDelay = CommDelay;}


// === EOF ===
