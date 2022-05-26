#include "../../system.h"

sdmmc_objectTypeDef sd_object_temp;

void sdmmc_readBlock_DMA(sdmmc_objectTypeDef *object,uint32_t *data,uint32_t index,uint32_t number);
void sdmmc_writeBlock_DMA(sdmmc_objectTypeDef *object,uint32_t *data,uint32_t index,uint32_t number);
void sdmmc_erase(sdmmc_objectTypeDef *object,uint32_t index,uint32_t number);
void sdmmc_get_cardInfo(sdmmc_objectTypeDef *object);
void sdmmc_get_card_state(sdmmc_objectTypeDef *object);
void sdmmc_scan_card_state(sdmmc_objectTypeDef *object);

sdmmc_objectTypeDef *sdmmc_object_init()
{
    sd_object_temp.hsdmmc.Instance = SDMMC1;
    sd_object_temp.hsdmmc.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING; 
    sd_object_temp.hsdmmc.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE; 
    sd_object_temp.hsdmmc.Init.BusWide = SDMMC_BUS_WIDE_4B; 
    sd_object_temp.hsdmmc.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE; 
    sd_object_temp.hsdmmc.Init.ClockDiv = 2;
    if (HAL_SD_Init(&sd_object_temp.hsdmmc) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_SD_ConfigWideBusOperation(&sd_object_temp.hsdmmc, SDMMC_BUS_WIDE_4B) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    sd_object_temp.sdmmc_readBlocks_DMA=sdmmc_readBlock_DMA;
    sd_object_temp.sdmmc_writeBlocks_DMA=sdmmc_writeBlock_DMA;
    sd_object_temp.sdmmc_erace=sdmmc_erase;
    sd_object_temp.sdmmc_get_cardInfo=sdmmc_get_cardInfo;
    sd_object_temp.sdmmc_get_card_state=sdmmc_get_card_state;
    sd_object_temp.sdmmc_scan_card_state=sdmmc_scan_card_state;


    return &sd_object_temp;
}

void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if(hsd->Instance == SDMMC1)
    {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
        PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler(__FILE__, __LINE__);
        }
        /* Enable SDIO clock */
        __HAL_RCC_SDMMC1_CLK_ENABLE();

        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOI_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        /**SDMMC1 GPIO Configuration
        PC10    ------> SDMMC1_D2
        PC11    ------> SDMMC1_D3
        PC12    ------> SDMMC1_CK
        PD2     ------> SDMMC1_CMD
        PC8     ------> SDMMC1_D0
        PC9     ------> SDMMC1_D1
        PI8     ------> DET(init at system.c)
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_8
                            |GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_SDIO1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


        /* NVIC configuration for SDIO interrupts */
        HAL_NVIC_SetPriority(SDMMC1_IRQn, 8, 0);
        HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
    }
}

void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&sd_object_temp.hsdmmc);
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  
}

void sdmmc_readBlock_DMA(sdmmc_objectTypeDef *object,uint32_t *data,uint32_t index,uint32_t number)
{
    if(HAL_SD_ReadBlocks_DMA(&object->hsdmmc,(uint8_t*)data,index,number)!=HAL_OK)
    {
        // Error_Handler(__FILE__, __LINE__);
    }
}

void sdmmc_writeBlock_DMA(sdmmc_objectTypeDef *object,uint32_t *data,uint32_t index,uint32_t number)
{
    if(HAL_SD_WriteBlocks_DMA(&object->hsdmmc,(uint8_t*)data,index,number)!=HAL_OK)
    {
        // Error_Handler(__FILE__, __LINE__);
    }
}

void sdmmc_erase(sdmmc_objectTypeDef *object,uint32_t index,uint32_t number)
{
    if(HAL_SD_Erase(&object->hsdmmc,index,index+number)!=HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void sdmmc_get_cardInfo(sdmmc_objectTypeDef *object)
{
    if(HAL_SD_GetCardInfo(&object->hsdmmc,&object->cardInfo)!=HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void sdmmc_get_card_state(sdmmc_objectTypeDef *object)
{
    object->state=(int32_t)((HAL_SD_GetCardState(&object->hsdmmc) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

void sdmmc_scan_card_state(sdmmc_objectTypeDef *object)
{
    if(HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_8)==0)
    {
        object->is_card_detected=1;
    }
    else
    {
        memset(&object->cardInfo,0,sizeof(HAL_SD_CardInfoTypeDef));
        object->is_card_detected=0;
    }
}