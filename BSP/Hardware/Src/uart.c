#include "../../system.h"

uart_objectHandle *object_handle;

//save for future use
uart_objectAttr temp={
  .Instance = USART1,
  .BaudRate = 115200,
  .WordLength = UART_WORDLENGTH_8B,
  .StopBits = UART_STOPBITS_1,
  .Parity = UART_PARITY_NONE,
  .Mode = UART_MODE_TX_RX,
  .HwFlowCtl = UART_HWCONTROL_NONE,
  .OverSampling = UART_OVERSAMPLING_16,
  .OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
  .ClockPrescaler = UART_PRESCALER_DIV1,
  .AdvFeatureInit = UART_ADVFEATURE_NO_INIT
};

void UART_Object_Init(uart_objectTypeDef* object,uart_objectAttr attr)
{
    object_handle=&object->object_handle;
}

void UART_Init(struct uart_objectStructDef* object)
{
    object->object_handle.huart.Instance = object->object_attr.Instance;
    object->object_handle.huart.Init.BaudRate = object->object_attr.BaudRate;
    object->object_handle.huart.Init.WordLength = object->object_attr.WordLength;
    object->object_handle.huart.Init.StopBits = object->object_attr.StopBits;
    object->object_handle.huart.Init.Parity = object->object_attr.Parity;
    object->object_handle.huart.Init.Mode = object->object_attr.Mode;
    object->object_handle.huart.Init.HwFlowCtl = object->object_attr.HwFlowCtl;
    object->object_handle.huart.Init.OverSampling = object->object_attr.OverSampling;
    object->object_handle.huart.Init.OneBitSampling = object->object_attr.OneBitSampling;
    object->object_handle.huart.Init.ClockPrescaler = object->object_attr.ClockPrescaler;
    object->object_handle.huart.AdvancedInit.AdvFeatureInit = object->object_attr.AdvFeatureInit;

    if (HAL_UART_Init(&object->object_handle.huart) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&object->object_handle.huart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&object->object_handle.huart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    if (HAL_UARTEx_DisableFifoMode(&object->object_handle.huart) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(huart->Instance==USART1)
  {
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();	
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA10     ------> USART1_RX
    PA9     ------> USART1_TX
    */

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 DMA Init */
    /* USART1_RX Init */
    object_handle->hdma_uart_rx.Instance = DMA1_Stream0;
    object_handle->hdma_uart_rx.Init.Request = DMA_REQUEST_USART1_RX;
    object_handle->hdma_uart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    object_handle->hdma_uart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    object_handle->hdma_uart_rx.Init.MemInc = DMA_MINC_ENABLE;
    object_handle->hdma_uart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    object_handle->hdma_uart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    object_handle->hdma_uart_rx.Init.Mode = DMA_CIRCULAR;
    object_handle->hdma_uart_rx.Init.Priority = DMA_PRIORITY_LOW;
    object_handle->hdma_uart_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&object_handle->hdma_uart_rx);

    __HAL_LINKDMA(huart,hdmarx,object_handle->hdma_uart_rx);

    /* USART1_TX Init */
    object_handle->hdma_uart_tx.Instance = DMA1_Stream1;
    object_handle->hdma_uart_tx.Init.Request = DMA_REQUEST_USART1_TX;
    object_handle->hdma_uart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    object_handle->hdma_uart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    object_handle->hdma_uart_tx.Init.MemInc = DMA_MINC_ENABLE;
    object_handle->hdma_uart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    object_handle->hdma_uart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    object_handle->hdma_uart_tx.Init.Mode = DMA_CIRCULAR;
    object_handle->hdma_uart_tx.Init.Priority = DMA_PRIORITY_LOW;
    object_handle->hdma_uart_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&object_handle->hdma_uart_tx);

    __HAL_LINKDMA(huart,hdmatx,object_handle->hdma_uart_tx);	

    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 4, 1);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  }
}
