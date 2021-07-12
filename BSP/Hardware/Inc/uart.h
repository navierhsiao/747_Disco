#ifndef __UART_H
#define __UART_H

enum uart_error_state
{
    UART_ERROR_NONE                  =0,
    UART_ERROR                       =1,
    UART_ERROR_ACKNOWLEDGE_FAILURE   =2,
    UART_ERROR_PERIPH_FAILURE        =3,
    UART_ERROR_BUSY                  =4
};

typedef struct
{
    USART_TypeDef *Instance;
    uint32_t BaudRate;            
    uint32_t WordLength;           
    uint32_t StopBits;
    uint32_t Parity;                
    uint32_t Mode;                
    uint32_t HwFlowCtl;            
    uint32_t OverSampling;          
    uint32_t OneBitSampling;       
    uint32_t ClockPrescaler;     
    uint32_t AdvFeatureInit;;
}uart_objectAttr;

typedef struct
{
    UART_HandleTypeDef huart;
    DMA_HandleTypeDef hdma_uart_tx;
    DMA_HandleTypeDef hdma_uart_rx;
}uart_objectHandle;

typedef struct uart_objectStructDef
{
    uart_objectAttr object_attr;
    uart_objectHandle object_handle;

    uint8_t status;

    void (*uart_init)(struct uart_objectStructDef*);
}uart_objectTypeDef;

void UART_Object_Init(uart_objectTypeDef* object,uart_objectAttr attr);

#endif