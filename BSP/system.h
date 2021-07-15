#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"

#include "Components/Inc/lcd.h"
#include "Components/Inc/touch.h"
#include "Components/Inc/is42s32800j.h"

#include "Hardware/Inc/ltdc_dsi.h"
#include "Hardware/Inc/i2c.h"
#include "Hardware/Inc/qspi.h"
#include "Hardware/Inc/sdram.h"
#include "Hardware/Inc/uart.h"

void Hardware_Init(void);
void Error_Handler(char *file, uint32_t line);

#endif