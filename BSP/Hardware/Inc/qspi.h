#ifndef __QSPI_H
#define __QSPI_H

typedef struct
{
    QUADSPI_TypeDef     *Instance;
    uint32_t            ClockPrescaler;    
    uint32_t            FifoThreshold;    
    uint32_t            SampleShifting;  
    uint32_t            FlashSize;        
    uint32_t            ChipSelectHighTime;
    uint32_t            ClockMode;         
    uint32_t            FlashID; 
    uint32_t            DualFlash;          
}QSPI_objectAttr;

typedef struct QSPI_objectStructDef
{
    QSPI_HandleTypeDef  hqspi;
    QSPI_objectAttr     object_attr;

    void (*qspi_init)           (struct QSPI_objectStructDef*);
    void (*qspi_writeCmd)       (struct QSPI_objectStructDef*,QSPI_CommandTypeDef *);
    void (*qspi_writeData)      (struct QSPI_objectStructDef*,uint8_t *);
    void (*qspi_readData)       (struct QSPI_objectStructDef*,uint8_t *);
    void (*qspi_autoPolling)    (struct QSPI_objectStructDef*,QSPI_CommandTypeDef *,QSPI_AutoPollingTypeDef *);
}QSPI_objectTypeDef;

void QSPI_object_Init(QSPI_objectTypeDef *object,QSPI_objectAttr attr);

#endif