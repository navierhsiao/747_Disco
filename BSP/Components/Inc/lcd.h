#ifndef __LCD_H
#define __LCD_H

#include "../../Hardware/Inc/ltdc_dsi.h"

/* List of OTM8009A used commands                                  */
/* Detailed in OTM8009A Data Sheet 'DATA_SHEET_OTM8009A_V0 92.pdf' */
/* Version of 14 June 2012                                         */
#define  OTM8009A_CMD_NOP                   0x00  /* NOP command      */
#define  OTM8009A_CMD_SWRESET               0x01  /* Sw reset command */   
#define  OTM8009A_CMD_RDDMADCTL             0x0B  /* Read Display MADCTR command : read memory display access ctrl */
#define  OTM8009A_CMD_RDDCOLMOD             0x0C  /* Read Display pixel format */
#define  OTM8009A_CMD_SLPIN                 0x10  /* Sleep In command */
#define  OTM8009A_CMD_SLPOUT                0x11  /* Sleep Out command */
#define  OTM8009A_CMD_PTLON                 0x12  /* Partial mode On command */

#define  OTM8009A_CMD_DISPOFF               0x28  /* Display Off command */
#define  OTM8009A_CMD_DISPON                0x29  /* Display On command */

#define  OTM8009A_CMD_CASET                 0x2A  /* Column address set command */
#define  OTM8009A_CMD_PASET                 0x2B  /* Page address set command */

#define  OTM8009A_CMD_RAMWR                 0x2C  /* Memory (GRAM) write command */
#define  OTM8009A_CMD_RAMRD                 0x2E  /* Memory (GRAM) read command  */

#define  OTM8009A_CMD_PLTAR                 0x30  /* Partial area command (4 parameters) */

#define  OTM8009A_CMD_TEOFF                 0x34  /* Tearing Effect Line Off command : command with no parameter */

#define  OTM8009A_CMD_TEEON                 0x35  /* Tearing Effect Line On command : command with 1 parameter 'TELOM' */

/* Parameter TELOM : Tearing Effect Line Output Mode : possible values */
#define OTM8009A_TEEON_TELOM_VBLANKING_INFO_ONLY            0x00
#define OTM8009A_TEEON_TELOM_VBLANKING_AND_HBLANKING_INFO   0x01

#define  OTM8009A_CMD_MADCTR                0x36  /* Memory Access write control command  */

/* Possible used values of MADCTR */
#define OTM8009A_MADCTR_MODE_PORTRAIT       0x00
#define OTM8009A_MADCTR_MODE_LANDSCAPE      0x60  /* MY = 0, MX = 1, MV = 1, ML = 0, RGB = 0 */

#define  OTM8009A_CMD_IDMOFF                0x38  /* Idle mode Off command */
#define  OTM8009A_CMD_IDMON                 0x39  /* Idle mode On command  */

#define  OTM8009A_CMD_COLMOD                0x3A  /* Interface Pixel format command */

/* Possible values of COLMOD parameter corresponding to used pixel formats */
#define  OTM8009A_COLMOD_RGB565             0x55
#define  OTM8009A_COLMOD_RGB888             0x77

#define  OTM8009A_CMD_RAMWRC                0x3C  /* Memory write continue command */
#define  OTM8009A_CMD_RAMRDC                0x3E  /* Memory read continue command  */

#define  OTM8009A_CMD_WRTESCN               0x44  /* Write Tearing Effect Scan line command */
#define  OTM8009A_CMD_RDSCNL                0x45  /* Read  Tearing Effect Scan line command */

/* CABC Management : ie : Content Adaptive Back light Control in IC OTM8009a */
#define  OTM8009A_CMD_WRDISBV               0x51  /* Write Display Brightness command          */
#define  OTM8009A_CMD_WRCTRLD               0x53  /* Write CTRL Display command                */
#define  OTM8009A_CMD_WRCABC                0x55  /* Write Content Adaptive Brightness command */
#define  OTM8009A_CMD_WRCABCMB              0x5E  /* Write CABC Minimum Brightness command     */

#define  OTM8009A_CMD_ID1                   0xDA  /* Read ID1 command      */
#define  OTM8009A_CMD_ID2                   0xDB  /* Read ID2 command      */
#define  OTM8009A_CMD_ID3                   0xDC  /* Read ID3 command      */

/* OTM8009A ID */
#define OTM8009A_ID                 0x40U

/**
 *  Possible values of Display Orientation
 */
#define OTM8009A_ORIENTATION_PORTRAIT    0U /* Portrait orientation choice of LCD screen  */
#define OTM8009A_ORIENTATION_LANDSCAPE   1U /* Landscape orientation choice of LCD screen */

/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */
#define OTM8009A_FORMAT_RGB888    ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
#define OTM8009A_FORMAT_RBG565    ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

/**
  * @brief  otm8009a_480x800 Size
  */

/* Width and Height in Portrait mode */
#define  OTM8009A_480X800_WIDTH             ((uint16_t)480)     /* LCD PIXEL WIDTH   */
#define  OTM8009A_480X800_HEIGHT            ((uint16_t)800)     /* LCD PIXEL HEIGHT  */

/* Width and Height in Landscape mode */
#define  OTM8009A_800X480_WIDTH             ((uint16_t)800)     /* LCD PIXEL WIDTH   */
#define  OTM8009A_800X480_HEIGHT            ((uint16_t)480)     /* LCD PIXEL HEIGHT  */

/**
  * @brief  OTM8009A_480X800 Timing parameters for Portrait orientation mode
  */
#define  OTM8009A_480X800_HSYNC             ((uint16_t)2)      /* Horizontal synchronization */
#define  OTM8009A_480X800_HBP               ((uint16_t)34)     /* Horizontal back porch      */
#define  OTM8009A_480X800_HFP               ((uint16_t)34)     /* Horizontal front porch     */
#define  OTM8009A_480X800_VSYNC             ((uint16_t)1)      /* Vertical synchronization   */
#define  OTM8009A_480X800_VBP               ((uint16_t)15)     /* Vertical back porch        */
#define  OTM8009A_480X800_VFP               ((uint16_t)16)     /* Vertical front porch       */

/**
  * @brief  OTM8009A_800X480 Timing parameters for Landscape orientation mode
  *         Same values as for Portrait mode in fact.
  */
#define  OTM8009A_800X480_HSYNC             OTM8009A_480X800_HSYNC  /* Horizontal synchronization */
#define  OTM8009A_800X480_HBP               OTM8009A_480X800_HBP    /* Horizontal back porch      */
#define  OTM8009A_800X480_HFP               OTM8009A_480X800_HFP    /* Horizontal front porch     */
#define  OTM8009A_800X480_VSYNC             OTM8009A_480X800_VSYNC  /* Vertical synchronization   */
#define  OTM8009A_800X480_VBP               OTM8009A_480X800_VBP    /* Vertical back porch        */
#define  OTM8009A_800X480_VFP               OTM8009A_480X800_VFP    /* Vertical front porch       */

/**
  * @brief  OTM8009A_480X800 frequency divider
  */
#define OTM8009A_480X800_FREQUENCY_DIVIDER  2   /* LCD Frequency divider      */

//color code
#define LCD_COLOR_BLUE          0xFF0000FFUL
#define LCD_COLOR_GREEN         0xFF00FF00UL
#define LCD_COLOR_RED           0xFFFF0000UL
#define LCD_COLOR_CYAN          0xFF00FFFFUL
#define LCD_COLOR_MAGENTA       0xFFFF00FFUL
#define LCD_COLOR_YELLOW        0xFFFFFF00UL
#define LCD_COLOR_LIGHTBLUE     0xFF8080FFUL
#define LCD_COLOR_LIGHTGREEN    0xFF80FF80UL
#define LCD_COLOR_LIGHTRED      0xFFFF8080UL
#define LCD_COLOR_LIGHTCYAN     0xFF80FFFFUL
#define LCD_COLOR_LIGHTMAGENTA  0xFFFF80FFUL
#define LCD_COLOR_LIGHTYELLOW   0xFFFFFF80UL
#define LCD_COLOR_DARKBLUE      0xFF000080UL
#define LCD_COLOR_DARKGREEN     0xFF008000UL
#define LCD_COLOR_DARKRED       0xFF800000UL
#define LCD_COLOR_DARKCYAN      0xFF008080UL
#define LCD_COLOR_DARKMAGENTA   0xFF800080UL
#define LCD_COLOR_DARKYELLOW    0xFF808000UL
#define LCD_COLOR_WHITE         0xFFFFFFFFUL
#define LCD_COLOR_LIGHTGRAY     0xFFD3D3D3UL
#define LCD_COLOR_GRAY          0xFF808080UL
#define LCD_COLOR_DARKGRAY      0xFF404040UL
#define LCD_COLOR_BLACK         0xFF000000UL
#define LCD_COLOR_BROWN         0xFFA52A2AUL
#define LCD_COLOR_ORANGE        0xFFFFA500UL

enum DISPLAY_STATE
{
  DISPLAY_ON  =1,
  DISPLAY_OFF =2
};

typedef struct lcd_structDef
{
  ltdc_dsi_objectTypeDef  dsi_object;
  uint32_t                colorCode;
  uint32_t                orientation;
  uint32_t                brightness;
  uint32_t                ID;
  void (*lcd_write_reg)       (struct lcd_structDef *object,uint16_t reg, uint8_t *data, uint16_t length);
  void (*lcd_read_reg)        (struct lcd_structDef *object,uint16_t reg, uint8_t *data, uint16_t length);

  void (*lcd_readID)          (struct lcd_structDef *object);
  void (*lcd_setBrightness)   (struct lcd_structDef *object,uint32_t brightness);
  void (*lcd_displayOnOFF)    (struct lcd_structDef *object,uint8_t state);
  void (*lcd_setOrientation)  (struct lcd_structDef *object,uint32_t orientation);
  void (*lcd_getXsize)        (struct lcd_structDef *object,uint32_t *xSize);
  void (*lcd_getYsize)        (struct lcd_structDef *object,uint32_t *ySize);

  void (*lcd_draw_line)       (struct lcd_structDef *object,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color);
}lcd_objectTypeDef;

enum LCD_STATE{
  LCD_OK=0,
  LCD_ERROR=-1
};

void lcd_init(lcd_objectTypeDef *object,uint32_t colorCoding,uint32_t orientation);
#endif