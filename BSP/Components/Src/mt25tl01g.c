#include "../../system.h"

void MT25TL01G_ResetMemory(QSPI_objectTypeDef *object);
void MT25TL01G_DummyCyclesCfg(QSPI_objectTypeDef *object);
void MT25TL01G_EnableMemoryMappedMode(QSPI_objectTypeDef *object);

void MT25TL01G_Enter4BytesAddressMode(QSPI_objectTypeDef *object, uint8_t Mode);
void MT25TL01G_ResetEnable(QSPI_objectTypeDef *object,uint8_t mode);
void MT25TL01G_ResetMemoryCMD(QSPI_objectTypeDef *object,uint8_t mode);
void MT25TL01G_AutoPollingMemReady(QSPI_objectTypeDef *object);
void MT25TL01G_WriteEnable(QSPI_objectTypeDef *object,uint8_t mode);
void MT25TL01G_EnterQPIMode(QSPI_objectTypeDef *object);
void MT25TL01G_ExitQPIMode(QSPI_objectTypeDef *object);
void MT25TL01G_EnableMemoryMappedModeSTR(QSPI_objectTypeDef *object,uint8_t mode);
void MT25TL01G_EnableMemoryMappedModeDTR(QSPI_objectTypeDef *object,uint8_t mode);

QSPI_objectAttr qspi_attr={
    .Instance               = QUADSPI,
    //STR:1，DTR：3
    .ClockPrescaler         = QSPI_DTR_TRANSFER,
    .FifoThreshold          = 1,
    //若為STR使用:QSPI_SAMPLE_SHIFTING_HALFCYCLE
    .SampleShifting         = QSPI_SAMPLE_SHIFTING_NONE,
    .FlashSize              = 0,
    .ChipSelectHighTime     = QSPI_CS_HIGH_TIME_4_CYCLE, /* Min 50ns for nonRead */
    .ClockMode              = QSPI_CLOCK_MODE_0,
    .FlashID                = QSPI_FLASH_ID_1,
    .DualFlash              = QSPI_DUALFLASH_ENABLE,
    .transferRate           = QSPI_DTR_TRANSFER
};

void mt25tl01g_Init(void)
{
    QSPI_objectTypeDef qspi_object;
    const uint32_t size=(uint32_t)POSITION_VAL((uint32_t)MT25TL01G_FLASH_SIZE) - 1U;
    qspi_attr.FlashSize=size;

    QSPI_object_Init(&qspi_object,qspi_attr);
    qspi_object.qspi_init(&qspi_object);

    MT25TL01G_ResetEnable(&qspi_object,0);
    MT25TL01G_ResetMemory(&qspi_object);
    MT25TL01G_AutoPollingMemReady(&qspi_object);
    MT25TL01G_Enter4BytesAddressMode(&qspi_object,0);
    MT25TL01G_DummyCyclesCfg(&qspi_object);
    MT25TL01G_ExitQPIMode(&qspi_object);
    MT25TL01G_EnterQPIMode(&qspi_object);

    MT25TL01G_EnableMemoryMappedMode(&qspi_object);
}

void MT25TL01G_ResetMemory(QSPI_objectTypeDef *object)
{
    MT25TL01G_ResetEnable(object,0);
    MT25TL01G_ResetMemoryCMD(object,0);
    MT25TL01G_AutoPollingMemReady(object);
    MT25TL01G_ResetEnable(object,1);
    MT25TL01G_ResetMemoryCMD(object,1);
}

void MT25TL01G_DummyCyclesCfg(QSPI_objectTypeDef *object)
{
    QSPI_CommandTypeDef command;
    uint16_t reg=0;

    /* Initialize the read volatile configuration register command */
    command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    command.Instruction       = MT25TL01G_READ_VOL_CFG_REG_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_4_LINES;
    command.DummyCycles       = 0;
    command.NbData            = 2;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    object->qspi_writeCmd(object,&command);
    object->qspi_readData(object,(uint8_t*)(&reg));
    MT25TL01G_WriteEnable(object,0);
    command.Instruction = MT25TL01G_WRITE_VOL_CFG_REG_CMD;
    MODIFY_REG(reg, 0xF0F0, ((MT25TL01G_DUMMY_CYCLES_READ_QUAD << 4) |
                               (MT25TL01G_DUMMY_CYCLES_READ_QUAD << 12)));

    object->qspi_writeCmd(object,&command);
    object->qspi_writeData(object,(uint8_t*)(&reg));
}

void MT25TL01G_EnableMemoryMappedMode(QSPI_objectTypeDef *object)
{
    if(object->object_attr.transferRate==QSPI_STR_TRANSFER)
    {
        MT25TL01G_EnableMemoryMappedModeSTR(object,3);
    }
    else if(object->object_attr.transferRate==QSPI_DTR_TRANSFER)
    {
        MT25TL01G_EnableMemoryMappedModeDTR(object,3);
    }
}

/*
*********************************************************************************************
*                       hardware command setting
*           mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
*********************************************************************************************
*/

//mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
void MT25TL01G_Enter4BytesAddressMode(QSPI_objectTypeDef *object, uint8_t mode)
{
    QSPI_CommandTypeDef command;

    /* Initialize the command */
    command.InstructionMode   = (mode==0)?QSPI_INSTRUCTION_4_LINES:QSPI_INSTRUCTION_1_LINE;
    command.Instruction       = MT25TL01G_ENTER_4_BYTE_ADDR_MODE_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_NONE;
    command.DummyCycles       = 0;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /*write enable */
    MT25TL01G_WriteEnable(object,mode);

    /* Send the command */
    object->qspi_writeCmd(object,&command);

    /* Configure automatic polling mode to wait the memory is ready */
    MT25TL01G_AutoPollingMemReady(object);
}

//mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
void MT25TL01G_ResetEnable(QSPI_objectTypeDef *object,uint8_t mode)
{
    QSPI_CommandTypeDef command;

    /* Initialize the reset enable command */
    //mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
    command.InstructionMode   = (mode==0)?QSPI_INSTRUCTION_4_LINES:QSPI_INSTRUCTION_1_LINE;
    command.Instruction       = MT25TL01G_RESET_ENABLE_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_NONE;
    command.DummyCycles       = 0;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    object->qspi_writeCmd(object,&command);
}

//mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
void MT25TL01G_ResetMemoryCMD(QSPI_objectTypeDef *object,uint8_t mode)
{
    QSPI_CommandTypeDef command;

    /* Initialize the reset enable command */
    //mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
    command.InstructionMode   = (mode==0)?QSPI_INSTRUCTION_4_LINES:QSPI_INSTRUCTION_1_LINE;
    command.Instruction       = MT25TL01G_RESET_MEMORY_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_NONE;
    command.DummyCycles       = 0;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    object->qspi_writeCmd(object,&command);
}

void MT25TL01G_AutoPollingMemReady(QSPI_objectTypeDef *object)
{
    QSPI_CommandTypeDef     command;
    QSPI_AutoPollingTypeDef config;

    /* Configure automatic polling mode to wait for memory ready */
    command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    command.Instruction       = MT25TL01G_READ_STATUS_REG_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_4_LINES;
    command.DummyCycles       = 2;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    config.Match           = 0;
    config.MatchMode       = QSPI_MATCH_MODE_AND;
    config.Interval        = 0x10;
    config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
    config.Mask            = MT25TL01G_SR_WIP | (MT25TL01G_SR_WIP <<8);
    config.StatusBytesSize = 2;

    object->qspi_autoPolling(object,&command,&config);
}

//mode=0:QSPI_INSTRUCTION_4_LINES，mode=1:QSPI_INSTRUCTION_1_LINE
void MT25TL01G_WriteEnable(QSPI_objectTypeDef *object,uint8_t mode)
{
    QSPI_CommandTypeDef     command;
    QSPI_AutoPollingTypeDef config;

    /* Enable write operations */
    command.InstructionMode   = (mode==0)?QSPI_INSTRUCTION_4_LINES:QSPI_INSTRUCTION_1_LINE;
    command.Instruction       = MT25TL01G_WRITE_ENABLE_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_NONE;
    command.DummyCycles       = 0;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    object->qspi_writeCmd(object,&command);

    /* Configure automatic polling mode to wait for write enabling */
    config.Match           = MT25TL01G_SR_WREN | (MT25TL01G_SR_WREN << 8);
    config.Mask            = MT25TL01G_SR_WREN | (MT25TL01G_SR_WREN << 8);
    config.MatchMode       = QSPI_MATCH_MODE_AND;
    config.StatusBytesSize = 2;
    config.Interval        = 0x10;
    config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    command.Instruction    = MT25TL01G_READ_STATUS_REG_CMD;
    command.DataMode       = (mode==0)?QSPI_INSTRUCTION_4_LINES:QSPI_INSTRUCTION_1_LINE;

    object->qspi_autoPolling(object,&command,&config);
}

void MT25TL01G_EnterQPIMode(QSPI_objectTypeDef *object)
{
    QSPI_CommandTypeDef command;

    command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    command.Instruction       = MT25TL01G_ENTER_QUAD_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_NONE;
    command.DummyCycles       = 0;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    object->qspi_writeCmd(object,&command);
}

void MT25TL01G_ExitQPIMode(QSPI_objectTypeDef *object)
{
    QSPI_CommandTypeDef command;

    command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    command.Instruction       = MT25TL01G_EXIT_QUAD_CMD;
    command.AddressMode       = QSPI_ADDRESS_NONE;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DataMode          = QSPI_DATA_NONE;
    command.DummyCycles       = 0;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    object->qspi_writeCmd(object,&command);
}

/*
********************************************************************************
*   mode 0  =   SPI_MODE
*   mode 1  =   SPI_2IO_MODE
*   mode 2  =   SPI_4IO_MODE
*   mode 3  =   QPI_MODE
********************************************************************************
*/
void MT25TL01G_EnableMemoryMappedModeSTR(QSPI_objectTypeDef *object,uint8_t mode)
{
    QSPI_CommandTypeDef      command;
    QSPI_MemoryMappedTypeDef mem_mapped_cfg;
    switch(mode)
    {
        case 0:     /* 1-1-1 read commands */
            command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
            command.Instruction       = MT25TL01G_FAST_READ_4_BYTE_ADDR_CMD;
            command.AddressMode       = QSPI_ADDRESS_1_LINE;
            command.DataMode          = QSPI_DATA_1_LINE;
            break;

        case 1:     /* 1-2-2 read commands */

            command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
            command.Instruction       = MT25TL01G_DUAL_INOUT_FAST_READ_4_BYTE_ADDR_CMD;
            command.AddressMode       = QSPI_ADDRESS_2_LINES;
            command.DataMode          = QSPI_DATA_2_LINES;
            break;

        case 2:     /* 1-4-4 read commands */

            command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
            command.Instruction       = MT25TL01G_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD;
            command.AddressMode       = QSPI_ADDRESS_4_LINES;
            command.DataMode          = QSPI_DATA_4_LINES;
            break;

        case 3:     /* 4-4-4 commands */
            command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
            command.Instruction       = MT25TL01G_QUAD_INOUT_FAST_READ_CMD;
            command.AddressMode       = QSPI_ADDRESS_4_LINES;
            command.DataMode          = QSPI_DATA_4_LINES;
            break;
    }
    /* Configure the command for the read instruction */
    command.DummyCycles       = MT25TL01G_DUMMY_CYCLES_READ;
    command.AddressSize       = QSPI_ADDRESS_32_BITS;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* Configure the memory mapped mode */
    mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mem_mapped_cfg.TimeOutPeriod     = 0;

    object->qspi_memoryMapped(object,&command,&mem_mapped_cfg);
}

/*
********************************************************************************
*   mode 0  =   SPI_MODE
*   mode 1  =   SPI_2IO_MODE
*   mode 2  =   SPI_4IO_MODE
*   mode 3  =   QPI_MODE
********************************************************************************
*/
void MT25TL01G_EnableMemoryMappedModeDTR(QSPI_objectTypeDef *object,uint8_t mode)
{
    QSPI_CommandTypeDef      command;
    QSPI_MemoryMappedTypeDef mem_mapped_cfg;
    switch(mode)
    {
        case 0:                /* 1-1-1 commands, Power on H/W default setting */
            command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
            command.Instruction       = MT25TL01G_FAST_READ_4_BYTE_DTR_CMD;
            command.AddressMode       = QSPI_ADDRESS_1_LINE;
            command.DataMode          = QSPI_DATA_1_LINE;
            break;

        case 1:           /* 1-1-2 read commands */
            command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
            command.Instruction       = MT25TL01G_DUAL_OUT_FAST_READ_DTR_CMD;
            command.AddressMode       = QSPI_ADDRESS_1_LINE;
            command.DataMode          = QSPI_DATA_2_LINES;
        break;

        case 2:             /* 1-4-4 read commands */
            command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
            command.Instruction       = MT25TL01G_QUAD_INOUT_FAST_READ_4_BYTE_DTR_CMD;
            command.AddressMode       = QSPI_ADDRESS_4_LINES;
            command.DataMode          = QSPI_DATA_4_LINES;
            break;

        case 3:                 /* 4-4-4 commands */
            command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
            command.Instruction       = MT25TL01G_QUAD_INOUT_FAST_READ_DTR_CMD;
            command.AddressMode       = QSPI_ADDRESS_4_LINES;
            command.DataMode          = QSPI_DATA_4_LINES;
            break;
    }
    /* Configure the command for the read instruction */
    command.AddressSize       = QSPI_ADDRESS_32_BITS;
    command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    command.DummyCycles       = MT25TL01G_DUMMY_CYCLES_READ_QUAD_DTR;
    command.DdrMode           = QSPI_DDR_MODE_ENABLE;
    command.DdrHoldHalfCycle  = QSPI_DDR_HHC_HALF_CLK_DELAY;
    command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* Configure the memory mapped mode */
    mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mem_mapped_cfg.TimeOutPeriod     = 0;

    object->qspi_memoryMapped(object,&command,&mem_mapped_cfg);
}