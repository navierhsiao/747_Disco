#include "../../system.h"

//Todo:lcd 初始化參考到原本檔案的line 625

/*
**************************************************************************************
*   架構
*   1.DSI(CLK->paramter->cmd->lpcmd)
*   2.LTDC
*   3.GPIO
*
*
*
*
*
*
**************************************************************************************
*/

LTDC_HandleTypeDef hltdc;
DSI_HandleTypeDef hdsi;
DMA2D_HandleTypeDef hdma2d;

static DSI_CmdCfgTypeDef CmdCfg;
static DSI_LPCmdTypeDef LPCmd;
static DSI_PLLInitTypeDef dsiPllInit;
static RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

static void LCD_Reset(void);
static void LCD_MspInit(void);

void DSI_Config_Channel(DSI_LPCmdTypeDef *cmd)
{
    LPCmd=*cmd;
    HAL_DSI_ConfigCommand(&hdsi, &LPCmd);
}


void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
    if (NbrParams <= 1)
        HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
    else
        HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams); 
}

int DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int ret = 0;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(&hdsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = -1;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(&hdsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = -1;
    }
  }

  return ret;
}

int DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int ret = 0;

  if(HAL_DSI_Read(&hdsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = -1;
  }

  return ret;
}

void LTDC_DSI_Init(void)
{
    GPIO_InitTypeDef GPIO_Init_Structure;
    DSI_PHY_TimerTypeDef PhyTimings;

    LCD_Reset();
    LCD_MspInit();

    /* Enable GPIOJ clock */
    __HAL_RCC_GPIOJ_CLK_ENABLE();
  
    /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
    /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
    /* This pin is used only when the LCD and DSI link is configured in command mode    */
    GPIO_Init_Structure.Pin       = GPIO_PIN_2;
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
    HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);

    /* LCD clock configuration */
    /* LCD clock configuration */
    /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
    /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
    /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/21 = 38.095 Mhz */
    /* LTDC clock frequency = PLLLCDCLK = 38.095 Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 5;
    PeriphClkInitStruct.PLL3.PLL3N = 160;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 21;

    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
    hdsi.Instance = DSI;

    HAL_DSI_DeInit(&hdsi);

    dsiPllInit.PLLNDIV = 100;
    dsiPllInit.PLLIDF = DSI_PLL_IN_DIV5;
    dsiPllInit.PLLODF = DSI_PLL_OUT_DIV1;

    hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
    hdsi.Init.TXEscapeCkdiv = 0x4;
    HAL_DSI_Init(&hdsi, &dsiPllInit);

    /* Configure the DSI for Command mode */
    CmdCfg.VirtualChannelID      = 0;
    CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
    CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
    CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
    CmdCfg.CommandSize           = HACT;
    CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
    CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
    CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
    CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
    CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
    CmdCfg.ColorCoding           = DSI_RGB888;

    HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg);

    LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
    LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
    LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
    LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
    LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
    LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
    LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
    LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
    LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
    LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
    LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
    HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

    /* Configure DSI PHY HS2LP and LP2HS timings */
    PhyTimings.ClockLaneHS2LPTime = 35;
    PhyTimings.ClockLaneLP2HSTime = 35;
    PhyTimings.DataLaneHS2LPTime = 35;
    PhyTimings.DataLaneLP2HSTime = 35;
    PhyTimings.DataLaneMaxReadTime = 0;
    PhyTimings.StopWaitTime = 10;
    HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings);

    /* Initialize LTDC */
    /* DeInit */
    HAL_LTDC_DeInit(&hltdc);

    /* LTDC Config */
    /* Timing and polarity */
    hltdc.Init.HorizontalSync = HSYNC;
    hltdc.Init.VerticalSync = VSYNC;
    hltdc.Init.AccumulatedHBP = HSYNC + HBP;
    hltdc.Init.AccumulatedVBP = VSYNC + VBP;
    hltdc.Init.AccumulatedActiveH = VSYNC + VBP + VACT;
    hltdc.Init.AccumulatedActiveW = HSYNC + HBP + HACT;
    hltdc.Init.TotalHeigh = VSYNC + VBP + VACT + VFP;
    hltdc.Init.TotalWidth = HSYNC + HBP + HACT + HFP;

    /* background value */
    hltdc.Init.Backcolor.Blue = 0;
    hltdc.Init.Backcolor.Green = 0;
    hltdc.Init.Backcolor.Red = 0;

    /* Polarity */
    hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hltdc.Instance = LTDC;

    HAL_LTDC_Init(&hltdc);
    __HAL_LTDC_DISABLE(&hltdc);

    /* Start DSI */
    HAL_DSI_Start(&hdsi);
}

void LTDC_Enable(void)
{
  __HAL_LTDC_ENABLE(&hltdc);
}

void LTDC_Set_Pitch(uint32_t linePitchInPixel,uint32_t layerIndex)
{
  HAL_LTDC_SetPitch(&hltdc, linePitchInPixel, layerIndex);
}

void LCD_LayerInit(uint16_t LayerIndex, uint32_t Address, int LCD_Xsize,int LCD_Ysize)
{
    LTDC_LayerCfgTypeDef Layercfg;

    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = LCD_Ysize;
    Layercfg.FBStartAdress = Address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.ImageHeight = LCD_Ysize;

    Layercfg.WindowX1 = LCD_Xsize / 2; //Note: Div2 due to screen being divided into 2 areas.
    Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = LCD_Xsize / 2; //Note: Div2 due to screen being divided into 2 areas.

    HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, LayerIndex);
}

static void LCD_Reset(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Configure the GPIO on PG3 */
    gpio_init_structure.Pin   = GPIO_PIN_3;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(GPIOG, &gpio_init_structure);

    /* Activate XRES active low */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);

    HAL_Delay(20); /* wait 20 ms */

    /* Desactivate XRES */
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
    
    /* Wait for 10ms after releasing XRES before sending commands */
    HAL_Delay(10);
}

static void LCD_MspInit(void)
{
    /** @brief Enable the LTDC clock */
    __HAL_RCC_LTDC_CLK_ENABLE();

    /** @brief Toggle Sw reset of LTDC IP */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();

    /** @brief Enable the DMA2D clock */
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /** @brief Toggle Sw reset of DMA2D IP */
    __HAL_RCC_DMA2D_FORCE_RESET();
    __HAL_RCC_DMA2D_RELEASE_RESET();

    /** @brief Enable DSI Host and wrapper clocks */
    __HAL_RCC_DSI_CLK_ENABLE();

    /** @brief Soft Reset the DSI Host and wrapper */
    __HAL_RCC_DSI_FORCE_RESET();
    __HAL_RCC_DSI_RELEASE_RESET();

    /** @brief NVIC configuration for LTDC interrupt that is now enabled */
    HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);

    /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
    HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA2D_IRQn);

    /** @brief NVIC configuration for DSI interrupt that is now enabled */
    HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DSI_IRQn);
}
