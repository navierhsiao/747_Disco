#ifndef __LCD_H
#define __LCD_H

#include "../../Hardware/Inc/ltdc_dsi.h"
#include "Fonts/fonts.h"

//color code
#define LCD_COLOR_BLUE          0x0000FFUL
#define LCD_COLOR_GREEN         0x00FF00UL
#define LCD_COLOR_RED           0xFF0000UL
#define LCD_COLOR_CYAN          0x00FFFFUL
#define LCD_COLOR_MAGENTA       0xFF00FFUL
#define LCD_COLOR_YELLOW        0xFFFF00UL
#define LCD_COLOR_LIGHTBLUE     0x8080FFUL
#define LCD_COLOR_LIGHTGREEN    0x80FF80UL
#define LCD_COLOR_LIGHTRED      0xFF8080UL
#define LCD_COLOR_LIGHTCYAN     0x80FFFFUL
#define LCD_COLOR_LIGHTMAGENTA  0xFF80FFUL
#define LCD_COLOR_LIGHTYELLOW   0xFFFF80UL
#define LCD_COLOR_DARKBLUE      0x000080UL
#define LCD_COLOR_DARKGREEN     0x008000UL
#define LCD_COLOR_DARKRED       0x800000UL
#define LCD_COLOR_DARKCYAN      0x008080UL
#define LCD_COLOR_DARKMAGENTA   0x800080UL
#define LCD_COLOR_DARKYELLOW    0x808000UL
#define LCD_COLOR_WHITE         0xFFFFFFUL
#define LCD_COLOR_LIGHTGRAY     0xD3D3D3UL
#define LCD_COLOR_GRAY          0x808080UL
#define LCD_COLOR_DARKGRAY      0x404040UL
#define LCD_COLOR_BLACK         0x000000UL
#define LCD_COLOR_BROWN         0xA52A2AUL
#define LCD_COLOR_ORANGE        0xFFA500UL

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
  void (*lcd_write_reg_short) (struct lcd_structDef *object,uint16_t reg, uint32_t data);
  void (*lcd_write_reg_long)  (struct lcd_structDef *object,uint16_t reg, uint8_t *data, uint16_t length);
  void (*lcd_read_reg)        (struct lcd_structDef *object,uint16_t reg, uint8_t *data, uint16_t length);

  void (*lcd_readID)          (struct lcd_structDef *object);
  void (*lcd_setBrightness)   (struct lcd_structDef *object,uint32_t brightness);
  void (*lcd_displayOnOFF)    (struct lcd_structDef *object,uint8_t state);
  void (*lcd_setOrientation)  (struct lcd_structDef *object,uint32_t orientation);
  void (*lcd_getXsize)        (struct lcd_structDef *object,uint32_t *xSize);
  void (*lcd_getYsize)        (struct lcd_structDef *object,uint32_t *ySize);

  void (*lcd_draw_line)       (struct lcd_structDef *object,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color);
  void (*lcd_draw_rect)       (struct lcd_structDef *object,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint32_t color);
  void (*lcd_showString)      (struct lcd_structDef *object,uint16_t x,uint16_t y,sFONT *fonts,uint32_t color,const char *string,...);
}lcd_objectTypeDef;

enum LCD_STATE{
  LCD_OK=0,
  LCD_ERROR=-1
};

void lcd_init(lcd_objectTypeDef *object,uint32_t colorCoding,uint32_t orientation);
#endif