#include "../../system.h"

i2c_objectHandle *object_handle;
static osSemaphoreId BspI2cSemaphore = 0;

void I2C_Init(i2c_objectTypeDef* object);
void I2C_WriteReg(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t size,uint16_t length);
void I2C_ReadReg(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t size,uint16_t length);
void I2C_IsReady(i2c_objectTypeDef* object,uint16_t deviceAddr, uint32_t trials);

void I2C_Object_Init(i2c_objectTypeDef* object,i2c_objectAttr attr)
{
  object->i2c_init=I2C_Init;
  object->i2c_readReg=I2C_ReadReg;
  object->i2c_writeReg=I2C_WriteReg;
  object->i2c_isReady=I2C_IsReady;

  object->object_attr=attr;
  object_handle=&object->handle;
}

void I2C_Init(i2c_objectTypeDef* object)
{
  __HAL_RCC_BDMA_CLK_ENABLE();

  /* DMA interrupt init */
  /* BDMA_Channel0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BDMA_Channel0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(BDMA_Channel0_IRQn);
  /* BDMA_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BDMA_Channel1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(BDMA_Channel1_IRQn);

  object->handle.hi2c.Instance = object->object_attr.Instance;
  object->handle.hi2c.Init.Timing = object->object_attr.Timing;
  object->handle.hi2c.Init.OwnAddress1 = object->object_attr.OwnAddress1;
  object->handle.hi2c.Init.AddressingMode = object->object_attr.AddressingMode;
  object->handle.hi2c.Init.DualAddressMode = object->object_attr.DualAddressMode;
  object->handle.hi2c.Init.OwnAddress2 = object->object_attr.OwnAddress2;
  object->handle.hi2c.Init.OwnAddress2Masks = object->object_attr.OwnAddress2Masks;
  object->handle.hi2c.Init.GeneralCallMode = object->object_attr.GeneralCallMode;
  object->handle.hi2c.Init.NoStretchMode = object->object_attr.NoStretchMode;
  if (HAL_I2C_Init(&object->handle.hi2c) != HAL_OK)
  {
    object->i2c_status=I2C_ERROR;
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&object->handle.hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    object->i2c_status=I2C_ERROR;
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&object->handle.hi2c, 0) != HAL_OK)
  {
    object->i2c_status=I2C_ERROR;
    Error_Handler(__FILE__, __LINE__);
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if(hi2c->Instance==I2C4)
  {
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C4;
    PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**I2C4 GPIO Configuration
    PD12     ------> I2C4_SCL
    PD13     ------> I2C4_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C4_CLK_ENABLE();

    /* I2C4 DMA Init */
    /* I2C4_RX Init */
    object_handle->hdma_i2c_rx.Instance = BDMA_Channel0;
    object_handle->hdma_i2c_rx.Init.Request = BDMA_REQUEST_I2C4_RX;
    object_handle->hdma_i2c_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    object_handle->hdma_i2c_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    object_handle->hdma_i2c_rx.Init.MemInc = DMA_MINC_ENABLE;
    object_handle->hdma_i2c_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    object_handle->hdma_i2c_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    object_handle->hdma_i2c_rx.Init.Mode = DMA_NORMAL;
    object_handle->hdma_i2c_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&object_handle->hdma_i2c_rx) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(hi2c,hdmarx,object_handle->hdma_i2c_rx);

    /* I2C4_TX Init */
    object_handle->hdma_i2c_tx.Instance = BDMA_Channel1;
    object_handle->hdma_i2c_tx.Init.Request = BDMA_REQUEST_I2C4_TX;
    object_handle->hdma_i2c_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    object_handle->hdma_i2c_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    object_handle->hdma_i2c_tx.Init.MemInc = DMA_MINC_ENABLE;
    object_handle->hdma_i2c_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    object_handle->hdma_i2c_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    object_handle->hdma_i2c_tx.Init.Mode = DMA_NORMAL;
    object_handle->hdma_i2c_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&object_handle->hdma_i2c_tx) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(hi2c,hdmatx,object_handle->hdma_i2c_tx);
  }

}

void BDMA_Channel0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&object_handle->hdma_i2c_rx);
}

void BDMA_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&object_handle->hdma_i2c_tx);
}

void I2C_WriteReg(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t size,uint16_t length)
{
    osSemaphoreWait(BspI2cSemaphore, osWaitForever);

    if(HAL_I2C_Mem_Write_DMA(&object->handle.hi2c, deviceAddr, reg, size, pData, length) != HAL_OK)
    {
      if(HAL_I2C_GetError(&object->handle.hi2c)==HAL_I2C_ERROR_AF)
      {
        object->i2c_status=I2C_ERROR_ACKNOWLEDGE_FAILURE;
      }
      else
      {
        object->i2c_status=I2C_ERROR_PERIPH_FAILURE;
      }
      
      Error_Handler(__FILE__, __LINE__);
    }
    else
    {
      object->i2c_status=I2C_ERROR_NONE;
    }

    osSemaphoreRelease(BspI2cSemaphore);
}

void I2C_ReadReg(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t size,uint16_t length)
{
    osSemaphoreWait(BspI2cSemaphore, osWaitForever);

    if (HAL_I2C_Mem_Read_DMA(&object->handle.hi2c, deviceAddr, reg, size, pData, length) == HAL_OK)
    {
      if(HAL_I2C_GetError(&object->handle.hi2c)==HAL_I2C_ERROR_AF)
      {
        object->i2c_status=I2C_ERROR_ACKNOWLEDGE_FAILURE;
      }
      else
      {
        object->i2c_status=I2C_ERROR_PERIPH_FAILURE;
      }
      
      Error_Handler(__FILE__, __LINE__);
    }
    else
    {
      object->i2c_status=I2C_ERROR_NONE;
    }

    osSemaphoreRelease(BspI2cSemaphore);
}

void I2C_IsReady(i2c_objectTypeDef* object,uint16_t deviceAddr, uint32_t trials)
{
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);

  if(HAL_I2C_IsDeviceReady(&object->handle.hi2c, deviceAddr, trials, 1000) != HAL_OK)
  {
    object->i2c_status=I2C_ERROR_BUSY;
    Error_Handler(__FILE__, __LINE__);
  }

  osSemaphoreRelease(BspI2cSemaphore);
}