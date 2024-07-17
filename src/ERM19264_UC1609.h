/*!
    @file ERM19264_UC1609.h
    @brief ERM19264 LCD driven by UC1609C controller, header file.
    @author Gavin Lyons.
    @details Project Name: ERM19264_UC1609 <https://github.com/gavinlyonsrepo/ERM19264_UC1609>
*/

#ifndef ERM19264_UC1609_H
#define ERM19264_UC1609_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "ERM19264_graphics.h"
#include <SPI.h>

/*!
    @brief Display Pixel colours  definition
    @details
        (1): white on blue,  FG = white, BG = blue
        ERM19264SBS-4 LCD Display White on Blue
        (2): black on white, FG = black, BG = white
        ERM19264FS-4 LCD Display Black on White
        (3):white on black,  FG = white, BG = black
        ERM19264DNS-4LCD Display White on Black
*/
#define FOREGROUND  0  /**< See Display Pixel colours  definition */
#define BACKGROUND 1    /**< See Display Pixel colours  definition */
#define COLORINVERSE 2 /**< See Display Pixel colours  definition */

// UC1909 Write registers
#define UC1609_SYSTEM_RESET 0xE2 /**< System Reset */

#define UC1609_POWER_CONTROL 0x28 /**< Power control Address */
#define UC1609_PC_SET 0x06 /**< PC[2:0] 110, Internal V LCD (7x charge pump) + 10b: 1.4mA */

#define UC1609_ADDRESS_CONTROL 0x88 /**< set RAM address control */
#define UC1609_ADDRESS_SET 0x02 /**< Set AC [2:0] Program registers  for RAM address control.*/

#define UC1609_SET_PAGEADD 0xB0 /**< Page address Set PA[3:0]  */
#define UC1609_SET_COLADD_LSB 0x00 /**< Column Address Set CA [3:0] */
#define UC1609_SET_COLADD_MSB 0x10 /**< Column Address Set CA [7:4] */

#define UC1609_TEMP_COMP_REG 0x27 /**< Temperature Compensation Register */
#define UC1609_TEMP_COMP_SET 0x00 /**< TC[1:0] = 00b= -0.00%/ C */

#define UC1609_FRAMERATE_REG 0xA0 /**< Frame rate register */
#define UC1609_FRAMERATE_SET 0x01  /**< Set Frame Rate LC [4:3] 01b: 95 fps */

#define UC1609_BIAS_RATIO 0xE8 /**< Bias Ratio. The ratio between V-LCD and V-D . */
#define UC1609_BIAS_RATIO_SET 0x03 /**<  Set BR[1:0] = 11 (set to 9 default, 11b = 9) */

#define UC1609_GN_PM 0x81 /**< Set V BIAS Potentiometer to fine tune V-D and V-LCD  (double-byte command) */
#define UC1609_DEFAULT_GN_PM 0x49 /**< default only used if user does not specify Vbias */

#define UC1609_LCD_CONTROL 0xC0 /**< Rotate map control */
#define UC1609_DISPLAY_ON 0xAE /**< enables display */
#define UC1609_ALL_PIXEL_ON 0xA4 /**< sets on all Pixels on */
#define UC1609_INVERSE_DISPLAY 0xA6 /**< inverts display */
#define UC1609_SCROLL 0x40 /**< scrolls , Set the scroll line number. 0-64 */

// Rotate
#define UC1609_ROTATION_FLIP_TWO 0x06/**< mirror image Rotation LCD command*/
#define UC1609_ROTATION_NORMAL 0x04  /**< Normal Rotation LCD command*/
#define UC1609_ROTATION_FLIP_ONE 0x02  /**< 180 degree  Rotation LCD command*/
#define UC1609_ROTATION_FLIP_THREE 0x00 /**< mirror image Rotation LCD command*/

// Delays
#define UC1609_RESET_DELAY 3 /**<  ms Rest Delay ,datasheet >3uS */
#define UC1609_RESET_DELAY2   0 /**< mS reset delay datasheet says > 5mS, does not work */
#define UC1609_INIT_DELAY 100   /**<  mS init delay ,after init  */
#define UC1609_INIT_DELAY2 3 /**<  mS init delay,  before reset called datasheet <3mS */
#define UC1609_HIGHFREQ_DELAY 0 /**< uS  delay, Can be used in software SPI for high freq MCU*/

// GPIO abstraction 
#define UC1609_CS_SetHigh digitalWrite(_LCD_CS, HIGH)
#define UC1609_CS_SetLow digitalWrite(_LCD_CS, LOW)
#define UC1609_CD_SetHigh digitalWrite(_LCD_CD, HIGH)
#define UC1609_CD_SetLow digitalWrite(_LCD_CD, LOW)
#define UC1609_RST_SetHigh digitalWrite(_LCD_RST, HIGH)
#define UC1609_RST_SetLow digitalWrite(_LCD_RST, LOW)
#define UC1609_SCLK_SetHigh digitalWrite(_LCD_SCLK, HIGH)
#define  UC1609_SCLK_SetLow digitalWrite(_LCD_SCLK, LOW)
#define  UC1609_SDA_SetHigh digitalWrite(_LCD_DIN, HIGH)
#define  UC1609_SDA_SetLow digitalWrite(_LCD_DIN, LOW)

// SPI HW setup
#define UC_SPI_FREQ 8000000 /**< Mhz SPI bus baud rate  */
#define UC_SPI_DIRECTION  MSBFIRST /**< SPI data bit order orientation */
#define UC_SPI_UC1609_MODE SPI_MODE0 /**< SPI Mode 0-3 */
#define UC_SPI_CLOCK_DIV SPI_CLOCK_DIV8 /**< SPI bus baud rate  ,STM32 data use only */

//There is a pre-defined macro SPI_HAS_TRANSACTION in SPI library for checking 
 //whether the firmware of the Arduino board supports SPI.beginTransaction().
#ifdef SPI_HAS_TRANSACTION
    #define UC_SPI_TRANSACTION_START SPI.beginTransaction(SPISettings(UC_SPI_FREQ, UC_SPI_DIRECTION, UC_SPI_UC1609_MODE)); 
    #define UC_SPI_TRANSACTION_END SPI.endTransaction();                
#else // SPI transactions likewise not present in MCU or lib
    #define UC_SPI_TRANSACTION_START SPI.setClockDivider(UC_SPI_CLOCK_DIV); // 72/8 = 9Mhz
    #define UC_SPI_TRANSACTION_END  // Blank
#endif

/*! @brief class to hold screen data , multiple screens can be made for the shared buffer. Buffer must be same size and offsets to if saving Data memory is goal 
  */
class  ERM19264_UC1609_Screen
{
    public :
        ERM19264_UC1609_Screen(uint8_t* mybuffer, uint8_t w,  uint8_t h, int16_t  x, int16_t y) ;
        ~ERM19264_UC1609_Screen(){};
        
        uint8_t* screenBuffer; /**<  pointer to shared buffer screen data */
        uint8_t width;   /**< buffer x size in pixels  */
        uint8_t height; /**< buffer y size in pixels */
        int16_t xoffset; /**< x offset in pixels */
        int16_t yoffset; /**< y offset in pixels */
    private:
};

/*! @brief class to drive the ERM19264_UC1609 LCD */
class ERM19264_UC1609 : public ERM19264_graphics {
  public:
    ERM19264_UC1609(int16_t lcdwidth , int16_t lcdheight, int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din);
    ERM19264_UC1609(int16_t lcdwidth , int16_t lcdheight, int8_t cd, int8_t rst, int8_t cs);

    ~ERM19264_UC1609(){};

    virtual void drawPixel(int16_t x, int16_t y, uint8_t colour) override;
     void LCDupdate(void);
     void LCDclearBuffer(void);
     void LCDBuffer(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t* data);

    void LCDbegin(uint8_t VbiasPot = UC1609_DEFAULT_GN_PM , uint8_t AddressSet =UC1609_ADDRESS_SET);
    void LCDinit(void);
    void LCDEnable(uint8_t on);
    void LCDFillScreen(uint8_t pixel, uint8_t mircodelay);
    void LCDFillPage(uint8_t pixels);
    void  LCDGotoXY(uint8_t column , uint8_t page);
    void LCDrotate(uint8_t rotatevalue);
    void LCDInvertDisplay(uint8_t on);
    void LCDallpixelsOn(uint8_t bits);
    void LCDscroll(uint8_t bits);
    void LCDReset(void);
    LCD_Return_Codes_e LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data);
    void LCDPowerDown(void);
    
    uint16_t LCDLibVerNumGet(void);
    uint16_t LCDHighFreqDelayGet(void);
    void LCDHighFreqDelaySet(uint16_t);
    uint8_t LCDGetConstrast(void);
    uint8_t LCDGetAddressCtrl(void);
    ERM19264_UC1609_Screen* ActiveBuffer = nullptr; /**< Active buffer pointer , a pointer to which screen object shared buffer will be written to */
 
  private:

    void send_data(uint8_t data); 
    void send_command(uint8_t command, uint8_t value);
    bool isHardwareSPI(void);
    void CustomshiftOut(uint8_t bitOrder, uint8_t val);

	int8_t _LCD_CS;    /**< GPIO Chip select  line */
	int8_t _LCD_CD;   /**< GPIO Data or command line */
	int8_t _LCD_RST;  /**< GPIO Reset line */
	int8_t _LCD_SCLK; /**< GPIO Clock Line Software SPI only*/
	int8_t _LCD_DIN;  /**< GPIO MOSI Line Software SPI only*/
    
    uint8_t _VbiasPOT= 0x49; /**< Contrast default 0x49 datasheet 00-FE */
    uint8_t _AddressCtrl= 0x02; /**< Set AC [2:0] Program registers  for RAM address control. 0x00 to 0x07*/
    uint16_t _HighFreqDelay = UC1609_HIGHFREQ_DELAY; /**< uS GPIO Communications delay, SW SPI ONLY */
     const uint16_t _LibVersionNum = 182; /**< Library version number 180 = 1.8.0*/
     
     uint8_t _widthScreen = 192; /**< Width of screen in pixels */
     uint8_t _heightScreen = 64;  /**< Height of screen in pixels */
   
};

#endif
