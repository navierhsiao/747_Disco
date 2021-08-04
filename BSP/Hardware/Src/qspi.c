#include "../../system.h"

MDMA_HandleTypeDef hmdma_quadspi_fifo_th;

#define QSPI_BK1_IO0_Pin GPIO_PIN_11
#define QSPI_BK1_IO0_GPIO_Port GPIOD
#define QSPI_BK1_IO1_Pin GPIO_PIN_9
#define QSPI_BK1_IO1_GPIO_Port GPIOF
#define QSPI_BK1_IO2_Pin GPIO_PIN_7
#define QSPI_BK1_IO2_GPIO_Port GPIOF
#define QSPI_BK1_IO3_Pin GPIO_PIN_6
#define QSPI_BK1_IO3_GPIO_Port GPIOF

#define QSPI_BK2_IO0_Pin GPIO_PIN_2
#define QSPI_BK2_IO0_GPIO_Port GPIOH
#define QSPI_BK2_IO1_Pin GPIO_PIN_3
#define QSPI_BK2_IO1_GPIO_Port GPIOH
#define QSPI_BK2_IO2_Pin GPIO_PIN_9
#define QSPI_BK2_IO2_GPIO_Port GPIOG
#define QSPI_BK2_IO3_Pin GPIO_PIN_14
#define QSPI_BK2_IO3_GPIO_Port GPIOG

#define QSPI_BK1_NCS_Pin GPIO_PIN_6
#define QSPI_BK1_NCS_GPIO_Port GPIOG
#define QSPI_CLK_Pin GPIO_PIN_2
#define QSPI_CLK_GPIO_Port GPIOB

void QSPI_Init(QSPI_objectTypeDef *object);
void QSPI_Write_Command(QSPI_objectTypeDef *object,QSPI_CommandTypeDef *cmd);
void QSPI_Write_Data(QSPI_objectTypeDef *object,uint8_t *reg);
void QSPI_Read_Data(QSPI_objectTypeDef *object,uint8_t *reg);
void QSPI_Auto_Polling(QSPI_objectTypeDef *object,QSPI_CommandTypeDef *cmd,QSPI_AutoPollingTypeDef *cofig);
void QSPI_MemoryMapped(QSPI_objectTypeDef *object,QSPI_CommandTypeDef *cmd,QSPI_MemoryMappedTypeDef *config);

void QSPI_object_Init(QSPI_objectTypeDef *object,QSPI_objectAttr attr)
{
    object->qspi_init=QSPI_Init;
    object->qspi_writeCmd=QSPI_Write_Command;
    object->qspi_writeData=QSPI_Write_Data;
    object->qspi_readData=QSPI_Read_Data;
    object->qspi_autoPolling=QSPI_Auto_Polling;
    object->qspi_memoryMapped=QSPI_MemoryMapped;
    object->object_attr=attr;
}

void QSPI_Init(QSPI_objectTypeDef *object)
{
    object->hqspi.Instance                = object->object_attr.Instance;
    object->hqspi.Init.ClockPrescaler     = object->object_attr.ClockPrescaler;
    object->hqspi.Init.FifoThreshold      = object->object_attr.FifoThreshold;
    object->hqspi.Init.SampleShifting     = object->object_attr.SampleShifting;
    object->hqspi.Init.FlashSize          = object->object_attr.FlashSize;
    object->hqspi.Init.ChipSelectHighTime = object->object_attr.ChipSelectHighTime; 
    object->hqspi.Init.ClockMode          = object->object_attr.ClockMode;
    object->hqspi.Init.FlashID            = object->object_attr.DualFlash;
    object->hqspi.Init.DualFlash          = object->object_attr.DualFlash;
    
    if(HAL_QSPI_Init(&object->hqspi) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }
}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }
    /* Enable the QuadSPI memory interface clock */
    __HAL_RCC_QSPI_CLK_ENABLE();
    __HAL_RCC_MDMA_CLK_ENABLE();
    /* Reset the QuadSPI memory interface */
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /*##-2- Configure peripheral GPIO ##########################################*/

    /**QUADSPI GPIO Configuration
    PG9     ------> QUADSPI_BK2_IO2
    PG14     ------> QUADSPI_BK2_IO3
    PG6     ------> QUADSPI_BK1_NCS
    PF6     ------> QUADSPI_BK1_IO3
    PF7     ------> QUADSPI_BK1_IO2
    PF9     ------> QUADSPI_BK1_IO1
    PH2     ------> QUADSPI_BK2_IO0
    PH3     ------> QUADSPI_BK2_IO1
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    */
   
    GPIO_InitStruct.Pin = QSPI_BK2_IO2_Pin|QSPI_BK2_IO3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_NCS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(QSPI_BK1_NCS_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_IO3_Pin|QSPI_BK1_IO2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_IO1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(QSPI_BK1_IO1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK2_IO0_Pin|QSPI_BK2_IO1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_CLK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = QSPI_BK1_IO0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(QSPI_BK1_IO0_GPIO_Port, &GPIO_InitStruct);

    /* QUADSPI MDMA Init */
    /* QUADSPI_FIFO_TH Init */
    hmdma_quadspi_fifo_th.Instance = MDMA_Channel0;
    hmdma_quadspi_fifo_th.Init.Request = MDMA_REQUEST_QUADSPI_FIFO_TH;
    hmdma_quadspi_fifo_th.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
    hmdma_quadspi_fifo_th.Init.Priority = MDMA_PRIORITY_LOW;
    hmdma_quadspi_fifo_th.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
    hmdma_quadspi_fifo_th.Init.SourceInc = MDMA_SRC_INC_BYTE;
    hmdma_quadspi_fifo_th.Init.DestinationInc = MDMA_DEST_INC_DISABLE;
    hmdma_quadspi_fifo_th.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
    hmdma_quadspi_fifo_th.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
    hmdma_quadspi_fifo_th.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
    hmdma_quadspi_fifo_th.Init.BufferTransferLength = 1;
    hmdma_quadspi_fifo_th.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
    hmdma_quadspi_fifo_th.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
    hmdma_quadspi_fifo_th.Init.SourceBlockAddressOffset = 0;
    hmdma_quadspi_fifo_th.Init.DestBlockAddressOffset = 0;
    if (HAL_MDMA_Init(&hmdma_quadspi_fifo_th) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_MDMA_ConfigPostRequestMask(&hmdma_quadspi_fifo_th, 0, 0) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(hqspi,hmdma,hmdma_quadspi_fifo_th);

    /*##-3- Configure the NVIC for QSPI #########################################*/
    /* NVIC configuration for QSPI interrupt */
    HAL_NVIC_SetPriority(QUADSPI_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
}

void QSPI_Write_Command(QSPI_objectTypeDef *object,QSPI_CommandTypeDef *cmd)
{
    if(HAL_QSPI_Command(&object->hqspi,cmd,HAL_QSPI_TIMEOUT_DEFAULT_VALUE != HAL_OK))
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void QSPI_Write_Data(QSPI_objectTypeDef *object,uint8_t *reg)
{
    if (HAL_QSPI_Transmit(&object->hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void QSPI_Read_Data(QSPI_objectTypeDef *object,uint8_t *reg)
{
    if (HAL_QSPI_Receive(&object->hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void QSPI_Auto_Polling(QSPI_objectTypeDef *object,QSPI_CommandTypeDef *cmd,QSPI_AutoPollingTypeDef *cofig)
{
    if (HAL_QSPI_AutoPolling(&object->hqspi, cmd, cofig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void QSPI_MemoryMapped(QSPI_objectTypeDef *object,QSPI_CommandTypeDef *cmd,QSPI_MemoryMappedTypeDef *config)
{
    if (HAL_QSPI_MemoryMapped(&object->hqspi, cmd, config) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}