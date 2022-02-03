#include "../../system.h"
#include <stdarg.h>
#include <string.h>

#define ABS(X)                 ((X) > 0 ? (X) : -(X))

static const uint8_t LcdRegData27[] = {0x00, 0x00, 0x03, 0x1F};
static const uint8_t LcdRegData28[] = {0x00, 0x00, 0x01, 0xDF};

void LCD_write_reg_short(lcd_objectTypeDef *object,uint16_t reg, uint32_t data);
void LCD_write_reg_long(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length);
void LCD_read_reg(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length);

void lcd_readID(lcd_objectTypeDef *object);
void lcd_setBrightness(lcd_objectTypeDef *object,uint32_t brightness);
void lcd_displayOnOFF(lcd_objectTypeDef *object,uint8_t state);
void lcd_setOrientation(lcd_objectTypeDef *object,uint32_t orientation);
void lcd_getXsize(lcd_objectTypeDef *object,uint32_t *xSize);
void lcd_getYsize(lcd_objectTypeDef *object,uint32_t *ySize);

void LCD_draw_pixel(lcd_objectTypeDef *object,uint16_t x,uint16_t y,uint32_t color);
void LCD_draw_char(lcd_objectTypeDef *object,uint16_t x,uint16_t y,uint8_t chr,sFONT *fonts,uint32_t color);

void LCD_draw_line(lcd_objectTypeDef *object,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color);
void LCD_draw_rect(lcd_objectTypeDef *object,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint32_t color);
void LCD_showString(lcd_objectTypeDef *object,uint16_t x,uint16_t y,sFONT *fonts,uint32_t color,const char *string,...);

void lcd_init(lcd_objectTypeDef *object,uint32_t colorCoding,uint32_t orientation)
{
  LTDC_DSI_object_Init(&object->dsi_object);
  object->lcd_write_reg_short=LCD_write_reg_short;
  object->lcd_write_reg_long=LCD_write_reg_long;
  object->lcd_read_reg=LCD_read_reg;

  object->lcd_readID=lcd_readID;
  object->lcd_setBrightness=lcd_setBrightness;
  object->lcd_displayOnOFF=lcd_displayOnOFF;
  object->lcd_setOrientation=lcd_setOrientation;
  object->lcd_getXsize=lcd_getXsize;
  object->lcd_getYsize=lcd_getYsize;

  object->lcd_draw_line=LCD_draw_line;
  object->lcd_draw_rect=LCD_draw_rect;
  object->lcd_showString=LCD_showString;

  // uint8_t reg_data[114]={
  //   0x10,0x00,0x01,0x00,0x16,0x33,0x03,0x09,0x06,0x88,0x88,0x00,0x44,0x49,0x07,0x00,0x07,0x00,
  //   0x00,0x04,0x0C,0x0E,0x07,0x18,0x09,0x0A,0x02,0x08,0x05,0x02,0x0B,0x38,0x35,0x00,
  //   0x00,0x03,0x0D,0x11,0x0A,0x15,0x0B,0x09,0x04,0x08,0x08,0x05,0x0A,0x21,0x1B,0x00,
  //   0x21,0x09,0x00,0x00,0x01,0x01,0x80,0x05,0x02,0x80,0x00,0x00,0x0A,0x0A,0x00,0x00,0xE0,0xE4,0x04,0x00,0x00,0xC0,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x23,0x45,0x67,0x01,
  //   0x23,0x45,0x67,0x01,0x11,0x00,0xEE,0xFF,0xBB,0xCA,0xDD,0xAC,0x76,0x67,0x22,0x22,0x22,0x22,0x22,0x22,0x13,
  //   0x10,0x22,0x77,0x79,0x10,0xB2,0x00,0x00
  // };

  uint8_t page1_change_set[5]={0xFF,0x98,0x06,0x04,0x01};
  uint8_t page6_change_set[5]={0xFF,0x98,0x06,0x04,0x06};
  uint8_t page7_change_set[5]={0xFF,0x98,0x06,0x04,0x07};
  uint8_t page0_change_set[5]={0xFF,0x98,0x06,0x04,0x00};

  object->lcd_write_reg_long(object,0xFF,page1_change_set,5);

  object->lcd_write_reg_short(object, 0x08, 0x10);		//Output    SDA
  object->lcd_write_reg_short(object, 0x20, 0x00);		//Set DE/VSYNC mode  0
  object->lcd_write_reg_short(object, 0x21, 0x00);		//DE = 1 Active  1
  object->lcd_write_reg_short(object, 0x30, 0x01);		//Resolution setting 2 -> 480 X 800, 1-> 480x854
  object->lcd_write_reg_short(object, 0x31, 0x00);		//Inversion setting 2-dot
  object->lcd_write_reg_short(object, 0x40, 0x16);		//BT  AVDD,AVDD
  object->lcd_write_reg_short(object, 0x41, 0x33);
  object->lcd_write_reg_short(object, 0x42, 0x03);		//VGL=DDVDH+VCIP -DDVDL,VGH=2DDVDL-VCIP
  object->lcd_write_reg_short(object, 0x43, 0x09);		//SET VGH clamp level
  object->lcd_write_reg_short(object, 0x44, 0x06);		//SET VGL clamp level


  object->lcd_write_reg_short(object, 0x50, 0x88);		//VREG1
  object->lcd_write_reg_short(object, 0x51, 0x88);		//VREG2
  object->lcd_write_reg_short(object, 0x52, 0x00);		//Flicker MSB
  object->lcd_write_reg_short(object, 0x53, 0x44);		//Flicker LSB //VCOM  49
  object->lcd_write_reg_short(object, 0x55, 0x49);		//Flicker
  object->lcd_write_reg_short(object, 0x60, 0x07);		// Source Timing Adjust 1
  object->lcd_write_reg_short(object, 0x61, 0x00);		// Source Timing Adjust 2
  object->lcd_write_reg_short(object, 0x62, 0x07);		// Source Timing Adjust 3
  object->lcd_write_reg_short(object, 0x63, 0x00);		// Source Timing Adjust 4

  //++++++++++++++++++ Gamma Setting ++++++++++++++++++//
  //A0h~AFh : PGAMCTRL1~16 (Positive Gamma Control 1~16)
  object->lcd_write_reg_short(object, 0xA0, 0x00);		//Positive Gamma
  object->lcd_write_reg_short(object, 0xA1, 0x09);
  object->lcd_write_reg_short(object, 0xA2, 0x11);
  object->lcd_write_reg_short(object, 0xA3, 0x0B);
  object->lcd_write_reg_short(object, 0xA4, 0x05);
  object->lcd_write_reg_short(object, 0xA5, 0x08);
  object->lcd_write_reg_short(object, 0xA6, 0x06);
  object->lcd_write_reg_short(object, 0xA7, 0x04);
  object->lcd_write_reg_short(object, 0xA8, 0x09);
  object->lcd_write_reg_short(object, 0xA9, 0x0C);
  object->lcd_write_reg_short(object, 0xAA, 0x15);
  object->lcd_write_reg_short(object, 0xAB, 0x08);
  object->lcd_write_reg_short(object, 0xAC, 0x0F);
  object->lcd_write_reg_short(object, 0xAD, 0x12);
  object->lcd_write_reg_short(object, 0xAE, 0x09);
  object->lcd_write_reg_short(object, 0xAF, 0x00);

  ///==============Nagitive
  //C0h~CFh : NGAMCTRL1~16(Negative Gamma Correction 1~16)
  object->lcd_write_reg_short(object, 0xC0, 0x00);	 //Negative Gamma
  object->lcd_write_reg_short(object, 0xC1, 0x09);
  object->lcd_write_reg_short(object, 0xC2, 0x10);
  object->lcd_write_reg_short(object, 0xC3, 0x0C);
  object->lcd_write_reg_short(object, 0xC4, 0x05);
  object->lcd_write_reg_short(object, 0xC5, 0x08);
  object->lcd_write_reg_short(object, 0xC6, 0x06);
  object->lcd_write_reg_short(object, 0xC7, 0x04);
  object->lcd_write_reg_short(object, 0xC8, 0x08);
  object->lcd_write_reg_short(object, 0xC9, 0x0C);
  object->lcd_write_reg_short(object, 0xCA, 0x14);
  object->lcd_write_reg_short(object, 0xCB, 0x08);
  object->lcd_write_reg_short(object, 0xCC, 0x0F);
  object->lcd_write_reg_short(object, 0xCD, 0x11);
  object->lcd_write_reg_short(object, 0xCE, 0x09);
  object->lcd_write_reg_short(object, 0xCF, 0x00);

  // Change to Page 6 CMD for GIP timing
  object->lcd_write_reg_long(object,0xFF,page6_change_set,5);
  // 00h~1Dh : GIPCTRL1 (GIP Control 1)
  object->lcd_write_reg_short(object, 0x00, 0x20);
  object->lcd_write_reg_short(object, 0x01, 0x0A);
  object->lcd_write_reg_short(object, 0x02, 0x00);
  object->lcd_write_reg_short(object, 0x03, 0x00);
  object->lcd_write_reg_short(object, 0x04, 0x01);
  object->lcd_write_reg_short(object, 0x05, 0x01);
  object->lcd_write_reg_short(object, 0x06, 0x98);
  object->lcd_write_reg_short(object, 0x07, 0x06);
  object->lcd_write_reg_short(object, 0x08, 0x01);
  object->lcd_write_reg_short(object, 0x09, 0x80);
  object->lcd_write_reg_short(object, 0x0A, 0x00);
  object->lcd_write_reg_short(object, 0x0B, 0x00);
  object->lcd_write_reg_short(object, 0x0C, 0x01);
  object->lcd_write_reg_short(object, 0x0D, 0x01);
  object->lcd_write_reg_short(object, 0x0E, 0x05);
  object->lcd_write_reg_short(object, 0x0F, 0x00);


  object->lcd_write_reg_short(object, 0x10, 0xF0);
  object->lcd_write_reg_short(object, 0x11, 0xF4);
  object->lcd_write_reg_short(object, 0x12, 0x01);
  object->lcd_write_reg_short(object, 0x13, 0x00);
  object->lcd_write_reg_short(object, 0x14, 0x00);
  object->lcd_write_reg_short(object, 0x15, 0xC0);
  object->lcd_write_reg_short(object, 0x16, 0x08);
  object->lcd_write_reg_short(object, 0x17, 0x00);
  object->lcd_write_reg_short(object, 0x18, 0x00);
  object->lcd_write_reg_short(object, 0x19, 0x00);
  object->lcd_write_reg_short(object, 0x1A, 0x00);
  object->lcd_write_reg_short(object, 0x1B, 0x00);
  object->lcd_write_reg_short(object, 0x1C, 0x00);
  object->lcd_write_reg_short(object, 0x1D, 0x00);


  object->lcd_write_reg_short(object, 0x20, 0x01);
  object->lcd_write_reg_short(object, 0x21, 0x23);
  object->lcd_write_reg_short(object, 0x22, 0x45);
  object->lcd_write_reg_short(object, 0x23, 0x67);
  object->lcd_write_reg_short(object, 0x24, 0x01);
  object->lcd_write_reg_short(object, 0x25, 0x23);
  object->lcd_write_reg_short(object, 0x26, 0x45);
  object->lcd_write_reg_short(object, 0x27, 0x67);




  object->lcd_write_reg_short(object, 0x30, 0x11);
  object->lcd_write_reg_short(object, 0x31, 0x11);
  object->lcd_write_reg_short(object, 0x32, 0x00);
  object->lcd_write_reg_short(object, 0x33, 0xEE);
  object->lcd_write_reg_short(object, 0x34, 0xFF);
  object->lcd_write_reg_short(object, 0x35, 0xBB);
  object->lcd_write_reg_short(object, 0x36, 0xAA);
  object->lcd_write_reg_short(object, 0x37, 0xDD);
  object->lcd_write_reg_short(object, 0x38, 0xCC);
  object->lcd_write_reg_short(object, 0x39, 0x66);
  object->lcd_write_reg_short(object, 0x3A, 0x77);
  object->lcd_write_reg_short(object, 0x3B, 0x22);
  object->lcd_write_reg_short(object, 0x3C, 0x22);
  object->lcd_write_reg_short(object, 0x3D, 0x22);
  object->lcd_write_reg_short(object, 0x3E, 0x22);
  object->lcd_write_reg_short(object, 0x3F, 0x22);
  object->lcd_write_reg_short(object, 0x40, 0x22);


  // Change to Page 7 CMD for GIP timing
  object->lcd_write_reg_long(object,0xFF,page7_change_set,5);
  object->lcd_write_reg_short(object, 0x17, 0x22);
  object->lcd_write_reg_short(object, 0x02, 0x77);		// --> ???AIAI ??A?????? ????e????A??
  object->lcd_write_reg_short(object, 0x26, 0xB2);		// --> ???AIAI ??A?????? ????e????A??


  // Change to Page 0 CMD for Normal command
  object->lcd_write_reg_long(object,0xFF,page0_change_set,5);

  object->lcd_write_reg_short(object, 0x35, 0x00);	//TE ON
  object->lcd_write_reg_short(object, 0x3A, 0x50);	 //50 -> 16BIT,  70 -> 24BIT
  object->lcd_write_reg_short(object, 0x11, 0x00);	 // exit Sleep
  HAL_Delay(120);
  object->lcd_write_reg_short(object, 0x29, 0x00);	// Display On
  HAL_Delay(25);
}
/*
*********************************************************************************************
*                             控制功能
*********************************************************************************************
*/
void lcd_readID(lcd_objectTypeDef *object)
{
  // object->lcd_read_reg(object,OTM8009A_CMD_ID1,&object->ID,0);
}
//brightness:0x00-0xFF
void lcd_setBrightness(lcd_objectTypeDef *object,uint32_t brightness)
{
  // object->brightness=brightness;
  // object->lcd_write_reg(object,OTM8009A_CMD_WRDISBV,&object->brightness,0);

  // uint8_t data=0;
  // object->lcd_read_reg(object,0x52,(uint8_t*)&data,0);
}
void lcd_displayOnOFF(lcd_objectTypeDef *object,uint8_t state)
{
  // uint8_t display=0;

  // if(state==DISPLAY_ON)
  // {
  //   object->lcd_write_reg(object,OTM8009A_CMD_DISPON,&display,0);
  // }
  // else if(state==DISPLAY_OFF)
  // {
  //   object->lcd_write_reg(object,OTM8009A_CMD_DISPOFF,&display,0);
  // }
}
void lcd_setOrientation(lcd_objectTypeDef *object,uint32_t orientation)
{
  // uint8_t tmp = OTM8009A_MADCTR_MODE_LANDSCAPE;
  // uint8_t tmp1 = OTM8009A_MADCTR_MODE_PORTRAIT;

  // if(orientation==OTM8009A_ORIENTATION_LANDSCAPE)
  // {
  //   object->lcd_write_reg(object,OTM8009A_CMD_MADCTR,&tmp,0);
  //   object->lcd_write_reg(object,OTM8009A_CMD_CASET,LcdRegData27,4);
  //   object->lcd_write_reg(object,OTM8009A_CMD_PASET,LcdRegData28,4);

  //   object->orientation=OTM8009A_ORIENTATION_LANDSCAPE;
  // }
  // else if(orientation==OTM8009A_ORIENTATION_PORTRAIT)
  // {
  //   object->lcd_write_reg(object,OTM8009A_CMD_MADCTR,&tmp1,0);
  //   object->lcd_write_reg(object,OTM8009A_CMD_CASET,LcdRegData27,4);
  //   object->lcd_write_reg(object,OTM8009A_CMD_PASET,LcdRegData28,4);

  //   object->orientation=OTM8009A_ORIENTATION_PORTRAIT;
  // }
}
void lcd_getXsize(lcd_objectTypeDef *object,uint32_t *xSize)
{
  // switch(object->orientation)
  // {
  //   case OTM8009A_ORIENTATION_PORTRAIT:
  //     *xSize=OTM8009A_480X800_WIDTH;
  //     break;
  //   case OTM8009A_ORIENTATION_LANDSCAPE:
  //     *xSize=OTM8009A_800X480_WIDTH;
  //     break;
  // }
}
void lcd_getYsize(lcd_objectTypeDef *object,uint32_t *ySize)
{
  // switch(object->orientation)
  // {
  //   case OTM8009A_ORIENTATION_PORTRAIT:
  //     *ySize=OTM8009A_480X800_HEIGHT;
  //     break;
  //   case OTM8009A_ORIENTATION_LANDSCAPE:
  //     *ySize=OTM8009A_800X480_HEIGHT;
  //     break;
  // }
}
/*
*********************************************************************************************
*                             繪圖功能
*********************************************************************************************
*/
//內部使用function
void LCD_draw_pixel(lcd_objectTypeDef *object,uint16_t x,uint16_t y,uint32_t color)
{
  /* Write data value to SDRAM memory */
  *(__IO uint32_t*) (LCD_FRAME_BUFFER + (4U*(y*Xsize + x))) = color;
}

void LCD_draw_char(lcd_objectTypeDef *object,uint16_t x,uint16_t y,uint8_t chr,sFONT *fonts,uint32_t color)
{
  uint32_t offset=0;
  uint32_t height=0;
  uint32_t width=0;
  uint32_t line=0;

  uint8_t *pchar;
  uint8_t *chr_data;

  chr_data=&fonts->table[(chr-' ')*fonts->Height*((fonts->Width+7)/8)];
  height=fonts->Height;
  width=fonts->Width;
  offset=8*((width+7)/8)-width;

  for(int i=0;i<height;i++)
  {
    pchar=((uint8_t*)chr_data+(width+7)/8*i);

    switch (((width+7)/8))
    {
    case 1:
      line=pchar[0];
      break;
    case 2:
      line=(pchar[0]<< 8)|pchar[1];
      break;
    case 3:
    default:
      line =(pchar[0]<< 16)|(pchar[1]<< 8)|pchar[2];
      break;
    }

    for(int j=0;j<width;j++)
    {
      if(line & (1 << (width- j + offset- 1)))
      {
        LCD_draw_pixel(object,(x+j),(y+i),color);
      }
    }
  }
}

//提供外部使用
void LCD_draw_line(lcd_objectTypeDef *object,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
  curpixel = 0;
  int32_t x_diff, y_diff;

  x_diff = x2 - x1;
  y_diff = y2 - y1;

  deltax = ABS(x_diff);         /* The absolute difference between the x's */
  deltay = ABS(y_diff);         /* The absolute difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */

  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }

  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }

  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }

  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    LCD_draw_pixel(object,x, y, color);   /* Draw the current pixel */
    num += numadd;                            /* Increase the numerator by the top of the fraction */
    if (num >= den)                           /* Check if numerator >= denominator */
    {
      num -= den;                             /* Calculate the new numerator value */
      x += xinc1;                             /* Change the x as appropriate */
      y += yinc1;                             /* Change the y as appropriate */
    }
    x += xinc2;                               /* Change the x as appropriate */
    y += yinc2;                               /* Change the y as appropriate */
  }

  object->dsi_object.dsi_refresh(&object->dsi_object);
}

void LCD_draw_rect(lcd_objectTypeDef *object,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize,uint32_t color)
{
  uint32_t address=0;

  /* Get the rectangle start address */
  address = (LCD_FRAME_BUFFER) + (3*(Xsize*y + x));
  object->dsi_object.copy_buffer_R2M(&object->dsi_object,(uint32_t*)address,xsize,ysize,(Xsize-xsize),color);

  // DMA2D->CR=0x00030000UL|(1<<9);
  // DMA2D->OCOLR=color;
  // DMA2D->OMAR=(uint32_t)address;
  // //offline
  // DMA2D->OOR=(Xsize-xsize);
  // DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;
  // DMA2D->NLR=(uint32_t)(xsize<<16)|(uint16_t)ysize;
  // DMA2D->CR |= DMA2D_CR_START;

  // while(DMA2D->CR&DMA2D_CR_START){}
}

void LCD_showString(lcd_objectTypeDef *object,uint16_t x,uint16_t y,sFONT *fonts,uint32_t color,const char *string,...)
{
  va_list ap;
  uint16_t xpos=0;
  uint8_t count=0;

  va_start(ap, string);
  char temp[100]={0};
  vsprintf(temp,string,ap);
  va_end(ap);
  
  while(temp[count]!=0&xpos<Xsize)
  {
    LCD_draw_char(object,x+xpos,y,temp[count],fonts,color);
    xpos+=fonts->Width;
    count++;
  }
}

/*
*********************************************************************************************
*                             讀取用功能
*********************************************************************************************
*/
void LCD_write_reg_short(lcd_objectTypeDef *object,uint16_t reg, uint32_t data)
{
  object->dsi_object.dsi_IO_shortWrite(&object->dsi_object,0,reg,data);
}

void LCD_write_reg_long(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length)
{
  object->dsi_object.dsi_IO_longWrite(&object->dsi_object,0,reg,data,length);
}

void LCD_read_reg(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length)
{
  object->dsi_object.dsi_IO_read(&object->dsi_object,0,reg,data,length);
}