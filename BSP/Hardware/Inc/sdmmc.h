#ifndef SDMMC_H
#define SDMMC_H

#define IS_SD_INSERTED HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_8)

typedef struct sdmmc_structDef
{
    uint8_t                 is_card_detected;
    SD_HandleTypeDef        hsdmmc;
    HAL_SD_CardInfoTypeDef  cardInfo;
    
    void    (*sdmmc_readBlocks_DMA)     (struct sdmmc_structDef *obejct,uint32_t *data,uint32_t index,uint32_t number);
    void    (*sdmmc_writeBlocks_DMA)    (struct sdmmc_structDef *obejct,uint32_t *data,uint32_t index,uint32_t number);
    void    (*sdmmc_erace)              (struct sdmmc_structDef *obejct,uint32_t index,uint32_t number);
    void    (*sdmmc_get_cardInfo)       (struct sdmmc_structDef *obejct);
    void    (*sdmmc_scan_card_state)    (struct sdmmc_structDef *object);
}sdmmc_objectTypeDef;

sdmmc_objectTypeDef *sdmmc_object_init();

#endif