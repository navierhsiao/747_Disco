#include "../../system.h"

void I2C_Init(i2c_objectTypeDef* object);
void I2C_Write_mem(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t size,uint16_t length);
void I2C_Read_mem(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *pData,uint16_t size,uint16_t length);
uint8_t I2C_Write(i2c_objectTypeDef* object,uint16_t deviceAddr,uint8_t *pData,uint16_t size);
uint8_t I2C_Read(i2c_objectTypeDef* object,uint16_t deviceAddr,uint8_t *pData,uint16_t size);

void I2C_Object_Init(i2c_objectTypeDef *object,i2c_objectAttr attr)
{
  object->i2c_read_mem=I2C_Read_mem;
  object->i2c_write_mem=I2C_Write_mem;
  object->i2c_read=I2C_Read;
  object->i2c_write=I2C_Write;
  object->object_attr=attr;

  object->hi2c.Instance = object->object_attr.Instance;
  object->hi2c.Init.Timing = object->object_attr.Timing;
  object->hi2c.Init.OwnAddress1 = object->object_attr.OwnAddress1;
  object->hi2c.Init.AddressingMode = object->object_attr.AddressingMode;
  object->hi2c.Init.DualAddressMode = object->object_attr.DualAddressMode;
  object->hi2c.Init.OwnAddress2 = object->object_attr.OwnAddress2;
  object->hi2c.Init.OwnAddress2Masks = object->object_attr.OwnAddress2Masks;
  object->hi2c.Init.GeneralCallMode = object->object_attr.GeneralCallMode;
  object->hi2c.Init.NoStretchMode = object->object_attr.NoStretchMode;
  if (HAL_I2C_Init(&object->hi2c) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&object->hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&object->hi2c, 0) != HAL_OK)
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
    __HAL_RCC_I2C4_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**I2C4 GPIO Configuration
    PD12     ------> I2C4_SCL
    PD13     ------> I2C4_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral clock enable */
    
  }

}

void I2C_Write_mem(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *data,uint16_t size,uint16_t length)
{
  if(HAL_I2C_Mem_Write(&object->hi2c, deviceAddr, reg, size, data, length,1000) != HAL_OK)
  {    
    Error_Handler(__FILE__, __LINE__);
  }
}

void I2C_Read_mem(i2c_objectTypeDef* object,uint16_t deviceAddr,uint16_t reg,uint8_t *data,uint16_t size,uint16_t length)
{
  if (HAL_I2C_Mem_Read(&object->hi2c, deviceAddr, reg, size, data, length,1000) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}

uint8_t I2C_Write(i2c_objectTypeDef* object,uint16_t deviceAddr,uint8_t *data,uint16_t size)
{
  uint8_t state=HAL_I2C_Master_Transmit(&object->hi2c, deviceAddr, data, size,2000);
  return state;
}

uint8_t I2C_Read(i2c_objectTypeDef* object,uint16_t deviceAddr,uint8_t *data,uint16_t size)
{
  uint8_t state=HAL_I2C_Master_Receive(&object->hi2c, deviceAddr, data, size,2000);
  return state;
}
