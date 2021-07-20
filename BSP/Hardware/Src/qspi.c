#include "../../system.h"

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
    GPIO_InitTypeDef gpio_init_structure;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable the QuadSPI memory interface clock */
    __HAL_RCC_QSPI_CLK_ENABLE();
    /* Reset the QuadSPI memory interface */
    __HAL_RCC_QSPI_FORCE_RESET();
    __HAL_RCC_QSPI_RELEASE_RESET();
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* QSPI CLK GPIO pin configuration  */
    gpio_init_structure.Pin       = GPIO_PIN_2;
    gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
    gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);

    /* QSPI CS GPIO pin configuration  */
    gpio_init_structure.Pin       = GPIO_PIN_6;
    gpio_init_structure.Pull      = GPIO_PULLUP;
    gpio_init_structure.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOG, &gpio_init_structure);

    /* QSPI D0 GPIO pin configuration  */
    gpio_init_structure.Pin       = GPIO_PIN_11;
    gpio_init_structure.Pull      = GPIO_NOPULL;
    gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOD, &gpio_init_structure);

    gpio_init_structure.Pin       = GPIO_PIN_2;
    gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOH, &gpio_init_structure);

    /* QSPI D1 GPIO pin configuration  */
    gpio_init_structure.Pin       = GPIO_PIN_9;
    gpio_init_structure.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &gpio_init_structure);

    gpio_init_structure.Pin       = GPIO_PIN_3;
    gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOH, &gpio_init_structure);

    /* QSPI D2 GPIO pin configuration  */
    gpio_init_structure.Pin       = GPIO_PIN_7;
    gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &gpio_init_structure);

    gpio_init_structure.Pin       = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOG, &gpio_init_structure);

    /* QSPI D3 GPIO pin configuration  */
    gpio_init_structure.Pin       = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOF, &gpio_init_structure);

    gpio_init_structure.Pin       = GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &gpio_init_structure);

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