#include "../../system.h"

QSPI_objectAttr temp={
    // .Instance               = QUADSPI,
    // .ClockPrescaler         = Config->ClockPrescaler,
    // .FifoThreshold          = 1,
    // .SampleShifting         = Config->SampleShifting,
    // .FlashSize              = (uint32_t)POSITION_VAL((uint32_t)pInfo.FlashSize) - 1U,
    // .ChipSelectHighTime     = QSPI_CS_HIGH_TIME_4_CYCLE, /* Min 50ns for nonRead */
    // .ClockMode              = QSPI_CLOCK_MODE_0,
    // .FlashID                = QSPI_FLASH_ID_1,
    // .DualFlash              = Config->DualFlashMode
};

void QSPI_object_Init(QSPI_objectTypeDef *object,QSPI_objectAttr attr)
{

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

    HAL_QSPI_Init(&object->hqspi);
}

void QSPI_Write_Command(QSPI_objectTypeDef *object)
{

}