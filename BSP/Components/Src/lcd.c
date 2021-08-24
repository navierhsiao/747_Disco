#include "../../system.h"

//Todo:加入與LCD相關的驅動程式
void LCD_write_reg(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length);
void LCD_read_reg(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length);

void lcd_init(lcd_objectTypeDef *object,uint32_t colorCoding,uint32_t orientation)
{
  LTDC_DSI_object_Init(&object->dsi_object);
  object->lcd_write_reg=LCD_write_reg;
  object->lcd_read_reg=LCD_read_reg;

  static const uint8_t lcd_reg_data1[]  = {0x80,0x09,0x01};
  static const uint8_t lcd_reg_data2[]  = {0x80,0x09};
  static const uint8_t lcd_reg_data3[]  = {0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01};
  static const uint8_t lcd_reg_data4[]  = {0x00,0x09,0x0F,0x0E,0x07,0x10,0x0B,0x0A,0x04,0x07,0x0B,0x08,0x0F,0x10,0x0A,0x01};
  static const uint8_t lcd_reg_data5[]  = {0x79,0x79};
  static const uint8_t lcd_reg_data6[]  = {0x00,0x01};
  static const uint8_t lcd_reg_data7[]  = {0x85,0x01,0x00,0x84,0x01,0x00};
  static const uint8_t lcd_reg_data8[]  = {0x18,0x04,0x03,0x39,0x00,0x00,0x00,0x18,0x03,0x03,0x3A,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data9[]  = {0x18,0x02,0x03,0x3B,0x00,0x00,0x00,0x18,0x01,0x03,0x3C,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data10[] = {0x01,0x01,0x20,0x20,0x00,0x00,0x01,0x02,0x00,0x00};
  static const uint8_t lcd_reg_data11[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data12[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data13[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data14[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data15[] = {0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data16[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data17[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data18[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  static const uint8_t lcd_reg_data19[] = {0x00,0x26,0x09,0x0B,0x01,0x25,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data20[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x0A,0x0C,0x02};
  static const uint8_t lcd_reg_data21[] = {0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data22[] = {0x00,0x25,0x0C,0x0A,0x02,0x26,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data23[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x0B,0x09,0x01};
  static const uint8_t lcd_reg_data24[] = {0x26,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  static const uint8_t lcd_reg_data25[] = {0xFF,0xFF,0xFF};
  static const uint8_t LcdRegData27[] = {0x00, 0x00, 0x03, 0x1F};
  static const uint8_t LcdRegData28[] = {0x00, 0x00, 0x01, 0xDF};
  static const uint8_t short_reg_data[] = {
    0x00,0x00,0x80,0x30,0x8A,0x40,0xB1,0xA9,0x91,0x34,0xB4,0x50,0x4E,0x81,0x66,0xA1,0x08,
    0x92,0x01,0x95,0x94,0x33,0xA3,0x1B,0x82,0x83,0x83,0x0E,0xA6,0xA0,0xB0,0xC0,0xD0,0x90,
    0xE0,0xF0,0x00,OTM8009A_COLMOD_RGB565,OTM8009A_COLMOD_RGB888,0x7F,0x2C,0x02,0xFF,0x00,
    0x00,0x00,0x66,0xB6,0x06,0xB1,0x06
  };
  
  /* Enable CMD2 to access vendor specific commands                               */
  /* Enter in command 2 mode and set EXTC to enable address shift function (0x00) */ 
  object->lcd_write_reg(object,OTM8009A_CMD_NOP,&short_reg_data[1],0);
  
  object->lcd_write_reg(object,0xFF, lcd_reg_data1, 3);
  /* Enter ORISE Command 2 */
  /* Shift address to 0x80 */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[2], 0);
  
  object->lcd_write_reg(object, 0xFF, lcd_reg_data2, 2);
  
  /////////////////////////////////////////////////////////////////////
  /* SD_PCH_CTRL - 0xC480h - 129th parameter - Default 0x00          */
  /* Set SD_PT                                                       */
  /* -> Source output level during porch and non-display area to GND */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[2], 0);
  object->lcd_write_reg(object, 0xC4, &short_reg_data[3], 0);
  
  HAL_Delay(10);
  /* Not documented */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[4], 0);
  object->lcd_write_reg(object, 0xC4, &short_reg_data[5], 0);
  HAL_Delay(10);
  /////////////////////////////////////////////////////////////////////
  
  /* PWR_CTRL4 - 0xC4B0h - 178th parameter - Default 0xA8 */
  /* Set gvdd_en_test                                     */
  /* -> enable GVDD test mode !!!                         */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[6], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[7], 0);
  
  /* PWR_CTRL2 - 0xC590h - 146th parameter - Default 0x79      */
  /* Set pump 4 vgh voltage                                    */
  /* -> from 15.0v down to 13.0v                               */
  /* Set pump 5 vgh voltage                                    */
  /* -> from -12.0v downto -9.0v                               */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[8], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[9], 0);
  
  /* P_DRV_M - 0xC0B4h - 181th parameter - Default 0x00 */
  /* -> Column inversion                                */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[10], 0);
  object->lcd_write_reg(object, 0xC0, &short_reg_data[11], 0);
  
  /* VCOMDC - 0xD900h - 1st parameter - Default 0x39h */
  /* VCOM Voltage settings                            */
  /* -> from -1.0000v downto -1.2625v                 */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  object->lcd_write_reg(object, 0xD9, &short_reg_data[12], 0);
  
  /* Oscillator adjustment for Idle/Normal mode (LPDT only) set to 65Hz (default is 60Hz) */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[13], 0);
  object->lcd_write_reg(object, 0xC1, &short_reg_data[14], 0);
  
  /* Video mode internal */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[15], 0);
  object->lcd_write_reg(object, 0xC1, &short_reg_data[16], 0);
  
  /* PWR_CTRL2 - 0xC590h - 147h parameter - Default 0x00 */
  /* Set pump 4&5 x6                                     */
  /* -> ONLY VALID when PUMP4_EN_ASDM_HV = "0"           */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[17], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[18], 0);
  
  /* PWR_CTRL2 - 0xC590h - 150th parameter - Default 0x33h */
  /* Change pump4 clock ratio                              */
  /* -> from 1 line to 1/2 line                            */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[19], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[9], 0);
  
  /* GVDD/NGVDD settings */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  object->lcd_write_reg(object, 0xD8, lcd_reg_data5, 2);
  
  /* PWR_CTRL2 - 0xC590h - 149th parameter - Default 0x33h */
  /* Rewrite the default value !                           */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[20], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[21], 0);
  
  /* Panel display timing Setting 3 */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[22], 0);
  object->lcd_write_reg(object, 0xC0, &short_reg_data[23], 0);
  
  /* Power control 1 */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[24], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[25], 0);
  
  /* Source driver precharge */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[13], 0);
  object->lcd_write_reg(object, 0xC4, &short_reg_data[26], 0);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[15], 0);
  object->lcd_write_reg(object, 0xC1, &short_reg_data[27], 0);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[28], 0);
  object->lcd_write_reg(object, 0xB3, lcd_reg_data6, 2);
  
  /* GOAVST */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[2], 0);
  object->lcd_write_reg(object, 0xCE, lcd_reg_data7, 6);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[29], 0);
  object->lcd_write_reg(object, 0xCE, lcd_reg_data8, 14);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[30], 0);
  object->lcd_write_reg(object, 0xCE, lcd_reg_data9, 14);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[31], 0);
  object->lcd_write_reg(object, 0xCF, lcd_reg_data10, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[32], 0);
  object->lcd_write_reg(object, 0xCF, &short_reg_data[45], 0);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[2], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data11, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[33], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data12, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[29], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data13, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[30], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data14, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[31], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data15, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[32], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data16, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[34], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data17, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[35], 0);
  object->lcd_write_reg(object, 0xCB, lcd_reg_data18, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[2], 0);
  object->lcd_write_reg(object, 0xCC, lcd_reg_data19, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[33], 0);
  object->lcd_write_reg(object, 0xCC, lcd_reg_data20, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[29], 0);
  object->lcd_write_reg(object, 0xCC, lcd_reg_data21, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[30], 0);
  object->lcd_write_reg(object, 0xCC, lcd_reg_data22, 10);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[31], 0);
  object->lcd_write_reg(object, 0xCC, lcd_reg_data23, 15);
  
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[32], 0);
  object->lcd_write_reg(object, 0xCC, lcd_reg_data24, 15);
  
  /////////////////////////////////////////////////////////////////////////////
  /* PWR_CTRL1 - 0xc580h - 130th parameter - default 0x00 */
  /* Pump 1 min and max DM                                */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[13], 0);
  object->lcd_write_reg(object, 0xC5, &short_reg_data[46], 0);
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[47], 0);
  object->lcd_write_reg(object, 0xF5, &short_reg_data[48], 0);
  /////////////////////////////////////////////////////////////////////////////
  
  /* CABC LEDPWM frequency adjusted to 19,5kHz */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[49], 0);
  object->lcd_write_reg(object, 0xC6, &short_reg_data[50], 0);
  
  /* Exit CMD2 mode */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  object->lcd_write_reg(object, 0xFF, lcd_reg_data25, 3);
  
  /*************************************************************************** */
  /* Standard DCS Initialization TO KEEP CAN BE DONE IN HSDT                   */
  /*************************************************************************** */
  
  /* NOP - goes back to DCS std command ? */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  
  /* Gamma correction 2.2+ table (HSDT possible) */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  object->lcd_write_reg(object, 0xE1, lcd_reg_data3, 16);
  
  /* Gamma correction 2.2- table (HSDT possible) */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  object->lcd_write_reg(object, 0xE2, lcd_reg_data4, 16);
  
  /* Send Sleep Out command to display : no parameter */
  object->lcd_write_reg(object, OTM8009A_CMD_SLPOUT, &short_reg_data[36], 0);
  
  /* Wait for sleep out exit */
  HAL_Delay(120);
  
  switch(colorCoding)
  {
  case OTM8009A_FORMAT_RBG565 :
    /* Set Pixel color format to RGB565 */
    object->lcd_write_reg(object, OTM8009A_CMD_COLMOD, &short_reg_data[37], 0);
    object->colorCode = OTM8009A_FORMAT_RBG565;
    break;
  case OTM8009A_FORMAT_RGB888 :
    /* Set Pixel color format to RGB888 */
    object->lcd_write_reg(object, OTM8009A_CMD_COLMOD, &short_reg_data[38], 0);
    object->colorCode = OTM8009A_FORMAT_RGB888;
    break;
  default :
    break;
  }
  
  /* Send command to configure display in landscape orientation mode. By default
  the orientation mode is portrait  */
  if(orientation == OTM8009A_ORIENTATION_LANDSCAPE)
  {
	uint8_t tmp = OTM8009A_MADCTR_MODE_LANDSCAPE;
    object->lcd_write_reg(object, OTM8009A_CMD_MADCTR, &tmp, 0);
    object->lcd_write_reg(object, OTM8009A_CMD_CASET, LcdRegData27, 4);
    object->lcd_write_reg(object, OTM8009A_CMD_PASET, LcdRegData28, 4);
    
    object->orientation = OTM8009A_ORIENTATION_LANDSCAPE;
  }
  else
  {
    object->orientation = OTM8009A_ORIENTATION_PORTRAIT;
  }
  
  /** CABC : Content Adaptive Backlight Control section start >> */
  /* Note : defaut is 0 (lowest Brightness], 0xFF is highest Brightness, try 0x7F : intermediate value */
  object->lcd_write_reg(object, OTM8009A_CMD_WRDISBV, &short_reg_data[39], 0);
  /* Store default brightness */
  object->brightness = ((uint32_t)short_reg_data[39] / 0xFFU) * 100U;
  
  /* defaut is 0, try 0x2C - Brightness Control Block, Display Dimming & BackLight on */
  object->lcd_write_reg(object, OTM8009A_CMD_WRCTRLD, &short_reg_data[40], 0);
  
  /* defaut is 0, try 0x02 - image Content based Adaptive Brightness [Still Picture] */
  object->lcd_write_reg(object, OTM8009A_CMD_WRCABC, &short_reg_data[41], 0);
  
  /* defaut is 0 (lowest Brightness], 0xFF is highest Brightness */
  object->lcd_write_reg(object, OTM8009A_CMD_WRCABCMB, &short_reg_data[42], 0);
  
  /** CABC : Content Adaptive Backlight Control section end << */
  
  /* Send Command Display On */
  object->lcd_write_reg(object, OTM8009A_CMD_DISPON, &short_reg_data[43], 0);
  
  /* NOP command */
  object->lcd_write_reg(object, OTM8009A_CMD_NOP, &short_reg_data[1], 0);
  
  /* Send Command GRAM memory write (no parameters) : this initiates frame write via other DSI commands sent by */
  /* DSI host from LTDC incoming pixels in video mode */
  object->lcd_write_reg(object, OTM8009A_CMD_RAMWR, &short_reg_data[44], 0);

  /* Configure the audio driver */
  HAL_DSI_ShortWrite(&object->dsi_object.hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF, 0x00);

  DSI_LPCmdTypeDef LPCmd = {0};

  LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&object->dsi_object.hdsi, &LPCmd);

  HAL_LTDC_SetPitch(&object->dsi_object.hltdc, 800, 0);
  __HAL_LTDC_ENABLE(&object->dsi_object.hltdc);
}

void LCD_write_reg(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length)
{
  object->dsi_object.dsi_IO_write(&object->dsi_object,0,reg,data,length);
}

void LCD_read_reg(lcd_objectTypeDef *object,uint16_t reg, uint8_t *data, uint16_t length)
{
  object->dsi_object.dsi_IO_read(&object->dsi_object,0,reg,data,length);
}