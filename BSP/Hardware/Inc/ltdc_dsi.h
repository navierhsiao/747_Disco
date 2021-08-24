#ifndef __LTDC_DSI_H
#define __LTDC_DSI_H

#define LCD_FRAME_BUFFER        0xD0000000

#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define LAYER0_ADDRESS  (0xD0000000)
#define HACT            400

typedef struct ltdc_dsi_structDef
{
    DMA2D_HandleTypeDef hdma2d;
    DSI_HandleTypeDef   hdsi;
    LTDC_HandleTypeDef  hltdc;
    __IO int32_t        pend_buffer;

    void (*dsi_IO_write)    (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
    void (*dsi_IO_read)     (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size)
}ltdc_dsi_objectTypeDef;

void LTDC_DSI_object_Init(ltdc_dsi_objectTypeDef *object);

#endif