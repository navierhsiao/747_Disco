#ifndef __SDRAM_H
#define __SDRAM_H

typedef struct 
{
    FMC_SDRAM_TypeDef   *Instance;
    uint32_t            SDBank;                   
    uint32_t            ColumnBitsNumber;         
    uint32_t            RowBitsNumber;             
    uint32_t            MemoryDataWidth;           
    uint32_t            InternalBankNumber;        
    uint32_t            CASLatency;                   
    uint32_t            WriteProtection;            
    uint32_t            SDClockPeriod;             
    uint32_t            ReadBurst;                   
    uint32_t            ReadPipeDelay;    
}sdram_objectAttr;

typedef struct sdram_objectStructDef
{
    __IO sdram_objectAttr object_attr;
    __IO FMC_SDRAM_TimingTypeDef sdram_timing;
    __IO SDRAM_HandleTypeDef hsdram;

    void (*sdram_init)      (struct sdram_objectStructDef*);
    void (*sdram_writeCmd)  (struct sdram_objectStructDef *,FMC_SDRAM_CommandTypeDef *);
}sdram_objectTypeDef;

void SDRAM_object_Init(sdram_objectTypeDef *object,sdram_objectAttr attr);

#endif