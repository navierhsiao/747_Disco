#ifndef __LTDC_DSI_H
#define __LTDC_DSI_H

#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define LAYER0_ADDRESS  (0xD0000000)
#define HACT            400

void DSI_Config_Channel(DSI_LPCmdTypeDef *cmd);
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams);
int DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
int DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);

void LCD_LayerInit(uint16_t LayerIndex, uint32_t Address, int LCD_Xsize,int LCD_Ysize);
void LTDC_DSI_Init(void);
void LTDC_Enable(void);
void LTDC_Set_Pitch(uint32_t linePitchInPixel,uint32_t layerIndex);

#endif