#include "../../system.h"

sdram_objectAttr sdram_attr={
    .Instance=              FMC_SDRAM_DEVICE,
    .SDBank=                FMC_SDRAM_BANK2,                   
    .ColumnBitsNumber=      FMC_SDRAM_COLUMN_BITS_NUM_9,         
    .RowBitsNumber=         FMC_SDRAM_ROW_BITS_NUM_12,             
    .MemoryDataWidth=       FMC_SDRAM_MEM_BUS_WIDTH_32,           
    .InternalBankNumber=    FMC_SDRAM_INTERN_BANKS_NUM_4,        
    .CASLatency=            FMC_SDRAM_CAS_LATENCY_3,                   
    .WriteProtection=       FMC_SDRAM_WRITE_PROTECTION_DISABLE,            
    .SDClockPeriod=         FMC_SDRAM_CLOCK_PERIOD_2,             
    .ReadBurst=             FMC_SDRAM_RBURST_ENABLE,                   
    .ReadPipeDelay=         FMC_SDRAM_RPIPE_DELAY_0
};

FMC_SDRAM_TimingTypeDef sdram_timing={
    .LoadToActiveDelay    = 2,
    .ExitSelfRefreshDelay = 7,
    .SelfRefreshTime      = 4,
    .RowCycleDelay        = 7,
    .WriteRecoveryTime    = 2,
    .RPDelay              = 2,
    .RCDDelay             = 2
};

IS42S32800J_Context_t RegMode={
    .TargetBank      = FMC_SDRAM_CMD_TARGET_BANK2,
    .RefreshMode     = IS42S32800J_AUTOREFRESH_MODE_CMD,
    .RefreshRate     = (uint32_t)0x0603,
    .BurstLength     = IS42S32800J_BURST_LENGTH_1,
    .BurstType       = IS42S32800J_BURST_TYPE_SEQUENTIAL,
    .CASLatency      = IS42S32800J_CAS_LATENCY_3,
    .OperationMode   = IS42S32800J_OPERATING_MODE_STANDARD,
    .WriteBurstMode  = IS42S32800J_WRITEBURST_MODE_SINGLE
};

void IS42S32800J_ClockEnable(sdram_objectTypeDef *object,uint32_t Interface) ;
void IS42S32800J_Precharge(sdram_objectTypeDef *object, uint32_t Interface) ;
void IS42S32800J_ModeRegConfig(sdram_objectTypeDef *object, IS42S32800J_Context_t *pRegMode) ;
void IS42S32800J_RefreshMode(sdram_objectTypeDef *object, uint32_t Interface, uint32_t RefreshMode) ;
void IS42S32800J_RefreshRate(sdram_objectTypeDef *object, uint32_t RefreshCount) ;
void IS42S32800J_EnterPowerMode(sdram_objectTypeDef *object, uint32_t Interface) ;
void IS42S32800J_ExitPowerMode(sdram_objectTypeDef *object, uint32_t Interface) ;

void IS42S32800J_Init(void)
{
    sdram_objectTypeDef sdram_object;
    SDRAM_object_Init(&sdram_object,sdram_attr);
    sdram_object.sdram_timing=sdram_timing;

    sdram_object.sdram_init(&sdram_object);
    IS42S32800J_ClockEnable(&sdram_object,RegMode.TargetBank);
    HAL_Delay(1);
    IS42S32800J_Precharge(&sdram_object,RegMode.TargetBank);
    IS42S32800J_RefreshMode(&sdram_object,RegMode.TargetBank,RegMode.RefreshMode);
    IS42S32800J_ModeRegConfig(&sdram_object,&RegMode);
    IS42S32800J_RefreshRate(&sdram_object,RegMode.RefreshRate);
}

void IS42S32800J_ClockEnable(sdram_objectTypeDef *object,uint32_t Interface) 
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode            = IS42S32800J_CLK_ENABLE_CMD;
    Command.CommandTarget          = Interface;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    object->sdram_writeCmd(object,&Command);
}

void IS42S32800J_Precharge(sdram_objectTypeDef *object, uint32_t Interface) 
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode            = IS42S32800J_PALL_CMD;
    Command.CommandTarget          = Interface;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;
    object->sdram_writeCmd(object,&Command);
}

void IS42S32800J_ModeRegConfig(sdram_objectTypeDef *object, IS42S32800J_Context_t *pRegMode) 
{
    uint32_t tmpmrd;
    FMC_SDRAM_CommandTypeDef Command;
    /* Program the external memory mode register */
    tmpmrd = (uint32_t)pRegMode->BurstLength   |\
                        pRegMode->BurstType     |\
                        pRegMode->CASLatency    |\
                        pRegMode->OperationMode |\
                        pRegMode->WriteBurstMode;

    Command.CommandMode            = IS42S32800J_LOAD_MODE_CMD;
    Command.CommandTarget          = pRegMode->TargetBank;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    object->sdram_writeCmd(object,&Command);
}

void IS42S32800J_RefreshMode(sdram_objectTypeDef *object, uint32_t Interface, uint32_t RefreshMode) 
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode            = RefreshMode;
    Command.CommandTarget          = Interface;
    Command.AutoRefreshNumber      = 8;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    object->sdram_writeCmd(object,&Command);
}

void IS42S32800J_RefreshRate(sdram_objectTypeDef *object, uint32_t RefreshCount) 
{
    /* Set the device refresh rate */
    if(HAL_SDRAM_ProgramRefreshRate(&object->hsdram, RefreshCount) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
}

void IS42S32800J_EnterPowerMode(sdram_objectTypeDef *object, uint32_t Interface) 
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode            = IS42S32800J_POWERDOWN_MODE_CMD;
    Command.CommandTarget          = Interface;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    object->sdram_writeCmd(object,&Command);
}

void IS42S32800J_ExitPowerMode(sdram_objectTypeDef *object, uint32_t Interface) 
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode            = IS42S32800J_NORMAL_MODE_CMD;
    Command.CommandTarget          = Interface;
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;

    /* Send the command */
    object->sdram_writeCmd(object,&Command);
}