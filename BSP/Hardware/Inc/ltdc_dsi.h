#ifndef __LTDC_DSI_H
#define __LTDC_DSI_H

#define LCD_FRAME_BUFFER        0xD0000000

#define Xsize           480
#define Ysize           854

#define VSYNC           40
#define VBP             20
#define VFP             20
#define VACT            Ysize
#define HSYNC           40
#define HBP             90
#define HFP             20
#define HACT            Xsize

typedef struct ltdc_dsi_structDef
{
    DMA2D_HandleTypeDef hdma2d;
    DSI_HandleTypeDef   hdsi;
    LTDC_HandleTypeDef  hltdc;
    __IO int32_t        pend_buffer;

    void (*dsi_IO_shortWrite)    (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint32_t data);
    void (*dsi_IO_longWrite)    (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
    void (*dsi_IO_read)     (struct ltdc_dsi_structDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
    void (*dsi_refresh)     (struct ltdc_dsi_structDef *object);
    void (*copy_buffer_M2M) (struct ltdc_dsi_structDef *object,uint32_t *src,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize);
    void (*copy_buffer_R2M) (struct ltdc_dsi_structDef *object,uint32_t *dst,uint16_t xsize,uint16_t ysize,uint16_t offline,uint32_t color);
}ltdc_dsi_objectTypeDef;

void LTDC_DSI_object_Init(ltdc_dsi_objectTypeDef *object);

#endif