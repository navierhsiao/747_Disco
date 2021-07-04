#ifndef __LTDC_DSI_H
#define __LTDC_DSI_H

void DSI_Config_Channel(DSI_LPCmdTypeDef *cmd);
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);
int DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
int DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);

void LCD_LayerInit(uint16_t LayerIndex, uint32_t Address, int LCD_Xsize,int LCD_Ysize);
void LTDC_DSI_Init(void);

#endif