#include "../../system.h"

void SDRAM_Init(sdram_objectTypeDef *object);
void SDRAM_Write_Command(sdram_objectTypeDef *object,FMC_SDRAM_CommandTypeDef *SdramCmd);

void SDRAM_object_Init(sdram_objectTypeDef *object,sdram_objectAttr attr)
{
    object->sdram_init=SDRAM_Init;
    object->sdram_writeCmd=SDRAM_Write_Command;
    object->object_attr=attr;
}

void SDRAM_Init(sdram_objectTypeDef *object)
{
    object->hsdram.Instance = object->object_attr.Instance;

    /* SDRAM handle configuration */
    object->hsdram.Init.SDBank             = object->object_attr.SDBank;
    object->hsdram.Init.ColumnBitsNumber   = object->object_attr.ColumnBitsNumber;
    object->hsdram.Init.RowBitsNumber      = object->object_attr.RowBitsNumber;
    object->hsdram.Init.MemoryDataWidth    = object->object_attr.MemoryDataWidth;
    object->hsdram.Init.InternalBankNumber = object->object_attr.InternalBankNumber;
    object->hsdram.Init.CASLatency         = object->object_attr.CASLatency;
    object->hsdram.Init.WriteProtection    = object->object_attr.WriteProtection;
    object->hsdram.Init.SDClockPeriod      = object->object_attr.SDClockPeriod;
    object->hsdram.Init.ReadBurst          = object->object_attr.ReadBurst;
    object->hsdram.Init.ReadPipeDelay      = object->object_attr.ReadPipeDelay;

    if(HAL_SDRAM_Init(&object->hsdram, &object->sdram_timing) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }
}

void SDRAM_Write_Command(sdram_objectTypeDef *object,FMC_SDRAM_CommandTypeDef *SdramCmd)
{
  SDRAM_HandleTypeDef hsdram=object->hsdram;

  if(HAL_SDRAM_SendCommand(&hsdram, SdramCmd, 0xFFFF) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef  *hsdram)
{
  static MDMA_HandleTypeDef mdma_handle;
  GPIO_InitTypeDef gpio_init_structure;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable chosen MDMAx clock */
   __HAL_RCC_MDMA_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;

  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                              GPIO_PIN_14 | GPIO_PIN_15;

  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;

  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  /* GPIOF configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;

  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  /* GPIOG configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 /*| GPIO_PIN_3 */|\
                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */
  gpio_init_structure.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;

  HAL_GPIO_Init(GPIOH, &gpio_init_structure);

  /* GPIOI configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;

  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* Configure common MDMA parameters */
  mdma_handle.Init.Request                  = MDMA_REQUEST_SW;
  mdma_handle.Init.TransferTriggerMode      = MDMA_BLOCK_TRANSFER;
  mdma_handle.Init.Priority                 = MDMA_PRIORITY_HIGH;
  mdma_handle.Init.Endianness               = MDMA_LITTLE_ENDIANNESS_PRESERVE;
  mdma_handle.Init.SourceInc                = MDMA_SRC_INC_WORD;
  mdma_handle.Init.DestinationInc           = MDMA_DEST_INC_WORD;
  mdma_handle.Init.SourceDataSize           = MDMA_SRC_DATASIZE_WORD;
  mdma_handle.Init.DestDataSize             = MDMA_DEST_DATASIZE_WORD;
  mdma_handle.Init.DataAlignment            = MDMA_DATAALIGN_PACKENABLE;
  mdma_handle.Init.SourceBurst              = MDMA_SOURCE_BURST_SINGLE;
  mdma_handle.Init.DestBurst                = MDMA_DEST_BURST_SINGLE;
  mdma_handle.Init.BufferTransferLength     = 128;
  mdma_handle.Init.SourceBlockAddressOffset = 0;
  mdma_handle.Init.DestBlockAddressOffset   = 0;
  mdma_handle.Instance                      = MDMA_Channel0;

   /* Associate the MDMA handle */
  __HAL_LINKDMA(hsdram, hmdma, mdma_handle);

  /* Deinitialize the stream for new transfer */
  (void)HAL_MDMA_DeInit(&mdma_handle);

  /* Configure the MDMA stream */
  (void)HAL_MDMA_Init(&mdma_handle);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(MDMA_IRQn, 15U, 0);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);
}