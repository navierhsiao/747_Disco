#include "../../system.h"

I2C_HandleTypeDef hi2c4;
DMA_HandleTypeDef hdma_i2c4_rx;
DMA_HandleTypeDef hdma_i2c4_tx;

static osSemaphoreId BspI2cSemaphore = 0;

void I2C_Init(void)
{
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x10C0ECFF;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
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
    hdma_i2c4_rx.Instance = BDMA_Channel0;
    hdma_i2c4_rx.Init.Request = BDMA_REQUEST_I2C4_RX;
    hdma_i2c4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c4_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c4_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_i2c4_rx) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(hi2c,hdmarx,hdma_i2c4_rx);

    /* I2C4_TX Init */
    hdma_i2c4_tx.Instance = BDMA_Channel1;
    hdma_i2c4_tx.Init.Request = BDMA_REQUEST_I2C4_TX;
    hdma_i2c4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c4_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c4_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_i2c4_tx) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(hi2c,hdmatx,hdma_i2c4_tx);
  }

}

void I2C_WriteReg(uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t length)
{
    osSemaphoreWait(BspI2cSemaphore, osWaitForever);

    if(HAL_I2C_Mem_Write_DMA(&hi2c4, deviceAddr, reg, I2C_MEMADD_SIZE_8BIT, pData, length) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    osSemaphoreRelease(BspI2cSemaphore);
}

void I2C_ReadReg(uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t length)
{
    osSemaphoreWait(BspI2cSemaphore, osWaitForever);

    if (HAL_I2C_Mem_Read_DMA(&hi2c4, deviceAddr, reg, I2C_MEMADD_SIZE_8BIT, pData, length) == HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    osSemaphoreRelease(BspI2cSemaphore);
}

void I2C_WriteReg16(uint16_t deviceAddr, uint16_t reg, uint8_t *pData, uint16_t length)
{
    osSemaphoreWait(BspI2cSemaphore, osWaitForever);

    if(HAL_I2C_Mem_Write_DMA(&hi2c4, deviceAddr, reg, I2C_MEMADD_SIZE_16BIT, pData, length) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    } 

    osSemaphoreRelease(BspI2cSemaphore);
}

void I2C_ReadReg16(uint16_t deviceAddr, uint16_t reg, uint8_t *pData, uint16_t length)
{
    osSemaphoreWait(BspI2cSemaphore, osWaitForever);

    if (HAL_I2C_Mem_Read_DMA(&hi2c4, deviceAddr, reg, I2C_MEMADD_SIZE_8BIT, pData, length) == HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    osSemaphoreRelease(BspI2cSemaphore);
}

void I2C_IsReady(uint16_t deviceAddr, uint32_t trials)
{
  osSemaphoreWait(BspI2cSemaphore, osWaitForever);

  if(HAL_I2C_IsDeviceReady(&hi2c4, deviceAddr, trials, 1000) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  osSemaphoreRelease(BspI2cSemaphore);
}