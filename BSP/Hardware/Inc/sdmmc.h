#ifndef SDMMC_H
#define SDMMC_H

typedef struct sdmmc_structDef
{
    uint8_t                 is_card_detected;
    SD_HandleTypeDef        hsdmmc;
    HAL_SD_CardInfoTypeDef  cardInfo;
    
    void    (*sdmmc_readBlocks_DMA)     (struct sdmmc_structDef *obejct,uint32_t *data,uint32_t index,uint32_t number);
    void    (*sdmmc_writeBlocks_DMA)    (struct sdmmc_structDef *obejct,uint32_t *data,uint32_t index,uint32_t number);
    void    (*sdmmc_erace)              (struct sdmmc_structDef *obejct,uint32_t index,uint32_t number);
    void    (*sdmmc_get_cardInfo)       (struct sdmmc_structDef *obejct);
}sdmmc_objectTypeDef;

sdmmc_objectTypeDef *sdmmc_object_init();

#endif