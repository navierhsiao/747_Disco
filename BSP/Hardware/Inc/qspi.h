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
}QSPI_objectTypeDef;

#endif