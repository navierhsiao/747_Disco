#include "system.h"
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"
#include "Components/Src/Fonts/fonts.h"

#define DEVICE_IS_CUT_2_1()      (HAL_GetREVID() & 0x21ff) ? 1 : 0
#define DEVICE_IS_CUT_2_X()      (HAL_GetREVID() & 0x2fff) ? 1 : 0
#define DEVICE_IS_CUT_1_2()      (HAL_GetREVID() & 0x12ff) ? 1 : 0

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

static ADC_HandleTypeDef    AdcHandle;
static void SystemClock_Config(void);
static void MPU_Config(void);
static void BSP_Init(void);
static void PeriphCommonClock_Config(void);
static void CPU_CACHE_Enable(void);
static void CRC_Init(void);
static void GPIO_Init(void);
static uint32_t TEMP_SENSOR_Init(void);

lcd_objectTypeDef otm8009a_obj;

void Hardware_Init(void)
{
  int32_t timeout = 0xFFFF;

  MPU_Config();
  //Enable L1 cache
  CPU_CACHE_Enable();

  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler(__FILE__, __LINE__);
  }

  HAL_Init();
  SystemClock_Config();

  __HAL_RCC_HSEM_CLK_ENABLE();
  /*Take HSEM */
  HAL_HSEM_FastTake(HSEM_ID_0);
  /*Release HSEM in order to notify the CPU2(CM4)*/
  HAL_HSEM_Release(HSEM_ID_0,0);
  /* wait until CPU2 wakes up from stop mode */
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler(__FILE__, __LINE__);
  }

  BSP_Init();
}

static void BSP_Init(void)
{
  //QSPI init
  mt25tl01g_Init();
  //SDRAM init
  IS42S32800J_Init();
  //MCU internal temperature sensing init
  TEMP_SENSOR_Init();
  //RTC init
  //GPIO init
  GPIO_Init();
  //LCD init
  lcd_init(&otm8009a_obj,OTM8009A_FORMAT_RGB888,OTM8009A_ORIENTATION_LANDSCAPE);
  //CRC init
  // CRC_Init();
  otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,0,800,480,LCD_COLOR_DARKGRAY);
  otm8009a_obj.lcd_draw_line(&otm8009a_obj,5,0,5,200,LCD_COLOR_WHITE);
  otm8009a_obj.lcd_showString(&otm8009a_obj,20,100,&Font24,LCD_COLOR_ORANGE,"test %d %f",1234,12.37);
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  
/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

 /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
          (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)       
          The I/O Compensation Cell activation  procedure requires :
        - The activation of the CSI clock
        - The activation of the SYSCFG clock
        - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
 */
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xD0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 2;
  PeriphClkInitStruct.PLL2.PLL2N = 12;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}

static void CPU_CACHE_Enable(void)
{
  /* Enable D-Cache */
  SCB_EnableDCache();

  /* Enable I-Cache */
  SCB_EnableICache();
}

static void CRC_Init(void)
{
  __HAL_RCC_CRC_CLK_DISABLE();
  CRC_HandleTypeDef hcrc;
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}

static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : LCD_BL_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_RESET_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

static uint32_t TEMP_SENSOR_Init(void)
{
  uint32_t ret = HAL_OK;

  ADC_ChannelConfTypeDef sConfig;

   /* ADCx Periph clock enable */
   __HAL_RCC_ADC3_CLK_ENABLE();

  memset(&AdcHandle, 0, sizeof(AdcHandle));
  memset(&sConfig, 0, sizeof(sConfig));

  /* Configure the ADC peripheral */
  AdcHandle.Instance                      = ADC3;

  /* Clear all control registers */
  ADC_CLEAR_COMMON_CONTROL_REGISTER(&AdcHandle);

  AdcHandle.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;
  AdcHandle.Init.Resolution               = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode             = ENABLE;
  AdcHandle.Init.ContinuousConvMode       = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode    = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T1_CC1;
  AdcHandle.Init.EOCSelection             = ADC_EOC_SEQ_CONV;
  AdcHandle.Init.NbrOfConversion          = 1;
  AdcHandle.Init.NbrOfDiscConversion      = 1;
  AdcHandle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  AdcHandle.Init.LowPowerAutoWait         = DISABLE;
  AdcHandle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;

  *((uint32_t *) (ADC12_COMMON_BASE)) |= 0xC00000; // enable TSEN bit in  ADC CCR in common register

  ret = HAL_ADC_Init(&AdcHandle);
  if (ret == HAL_OK)
  {
    /* Configure ADC Temperature Sensor Channel */
    sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;                /* Sampled channel number */
    sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
    sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;   /* Sampling time (number of clock cycles unit) */
    sConfig.Offset       = 0;                           /* Parameter discarded because offset correction is disabled */
    sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */

    ret = HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
  }

  return ret;
}

uint32_t TEMP_SENSOR_Start(void)
{
  return HAL_ADC_Start(&AdcHandle);
}

uint32_t TEMP_SENSOR_Stop(void)
{
  return HAL_ADC_Stop(&AdcHandle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
}


void Error_Handler(char *file, uint32_t line)
{
	if (line == 0)
	{
		return;
	}
	
	while(1)
	{
	}
}
