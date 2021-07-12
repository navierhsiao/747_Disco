#include "system.h"

#define DEVICE_IS_CUT_2_1()      (HAL_GetREVID() & 0x21ff) ? 1 : 0
#define DEVICE_IS_CUT_2_X()      (HAL_GetREVID() & 0x2fff) ? 1 : 0
#define DEVICE_IS_CUT_1_2()      (HAL_GetREVID() & 0x12ff) ? 1 : 0

static ADC_HandleTypeDef    AdcHandle;

static void SystemClock_Config(void);
static void MPU_Config(void);
static void BSP_Init(void);
static uint32_t TEMP_SENSOR_Init(void);

void Hardware_Init(void)
{
    if(!DEVICE_IS_CUT_2_X())
    {
        if(!DEVICE_IS_CUT_1_2())
        {
            *((uint32_t*)0x51046108) = 1; /* DMA2D issue with cut 1.1 - fixed in cut 1.2 */
        }
        *((uint32_t*)0x51008108) = 1; /* AXI SRAM issue with cut1.2 - fixed in cut 2.0 */
    }

    /* Enable RTC back-up registers access */
    HAL_PWR_EnableBkUpAccess();

    MPU_Config();
    HAL_Init();

    SystemClock_Config();
    __HAL_RCC_CRC_CLK_ENABLE();

    BSP_Init();
}

static void BSP_Init(void)
{
    //QSPI init
    //SDRAM init
    //MCU internal temperature sensing init
    TEMP_SENSOR_Init();
    //RTC init
    //GPIO init
    //Enable L1 cache
    //LCD init
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    /** Macro to configure the PLL clock source
     */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                                |RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 64;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 13;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }

    /* PLL3 for USB Clock */
    PeriphClkInitStruct.PLL3.PLL3M = 25;
    PeriphClkInitStruct.PLL3.PLL3N = 336;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 7;

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler(__FILE__, __LINE__);
    }
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);\

    __HAL_RCC_CSI_ENABLE() ;

    /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;

    /* Enables the I/O Compensation Cell */    
    HAL_EnableCompensationCell();  
}

static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    HAL_MPU_Disable();

   /* Configure unused area of QSPI region as strongly ordered.
    * This is *important* to avoid unintentional fetches from illegal
    * addresses due to cache/speculation which would halt the MCU.
    */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as WT for QSPI */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup SDRAM in Write-through (framebuffer) */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0xD0000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup AXI SRAM, SRAM1 and SRAM2 in Write-through */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x24000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE/*MPU_ACCESS_BUFFERABLE*/;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER3;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE/*MPU_INSTRUCTION_ACCESS_DISABLE*/;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x10000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER4;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Setup SRAM3 in Write-through */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x10040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER5;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
