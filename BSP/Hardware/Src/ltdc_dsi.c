#include "../../system.h"

//Todo:lcd 初始化參考到原本檔案的line 625

/*
**************************************************************************************
*   架構
*   1.DSI(CLK->paramter->cmd->lpcmd)
*   2.LTDC
*   3.GPIO
*   因LTDC相關的初始化參數較多，因此將設置參數直接放在初始化中處理
**************************************************************************************
*/

static const uint32_t Buffers[] = 
{
  LCD_FRAME_BUFFER,
  LCD_FRAME_BUFFER + (800*480*4),  
};

ltdc_dsi_objectTypeDef *object_temp;
void DSI_IO_WRITE(ltdc_dsi_objectTypeDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
void DSI_IO_READ(ltdc_dsi_objectTypeDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size);
void DSI_Refresh(ltdc_dsi_objectTypeDef *object);
void copy_buffer_M2M(ltdc_dsi_objectTypeDef *object,uint32_t *src,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize);
void copy_buffer_R2M(ltdc_dsi_objectTypeDef *object,uint32_t *dst,uint16_t xsize,uint16_t ysize,uint16_t offline,uint32_t color);

void LTDC_DSI_object_Init(ltdc_dsi_objectTypeDef *object)
{
  DSI_PLLInitTypeDef PLLInit = {0};
  DSI_HOST_TimeoutTypeDef HostTimeouts = {0};
  DSI_PHY_TimerTypeDef PhyTimings = {0};
  DSI_LPCmdTypeDef LPCmd = {0};
  DSI_CmdCfgTypeDef CmdCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  object_temp=object;
  object->pend_buffer=-1;

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

  //  DMA2D Init

  // object->hdma2d.Instance=DMA2D;
  // object->hdma2d.Init.Mode = DMA2D_M2M;
  // object->hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB888;
  // object->hdma2d.Init.OutputOffset = 0;
  // if (HAL_DMA2D_Init(&object->hdma2d) != HAL_OK)
  // {
  //   Error_Handler(__FILE__, __LINE__);
  // }

  //  DSI HOST Init

  HAL_GPIO_WritePin(GPIOG , GPIO_PIN_3 , GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(GPIOG , GPIO_PIN_3, GPIO_PIN_SET);/* Deactivate XRES */
  HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */

  object->hdsi.Instance = DSI;
  object->hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
  object->hdsi.Init.TXEscapeCkdiv = 4;
  object->hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  PLLInit.PLLNDIV = 100;
  PLLInit.PLLIDF = DSI_PLL_IN_DIV5;
  PLLInit.PLLODF = DSI_PLL_OUT_DIV1;
  if (HAL_DSI_Init(&object->hdsi, &PLLInit) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  HostTimeouts.TimeoutCkdiv = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 0;
  HostTimeouts.LowPowerReceptionTimeout = 0;
  HostTimeouts.HighSpeedReadTimeout = 0;
  HostTimeouts.LowPowerReadTimeout = 0;
  HostTimeouts.HighSpeedWriteTimeout = 0;
  HostTimeouts.HighSpeedWritePrespMode = DSI_HS_PM_DISABLE;
  HostTimeouts.LowPowerWriteTimeout = 0;
  HostTimeouts.BTATimeout = 0;
  if (HAL_DSI_ConfigHostTimeouts(&object->hdsi, &HostTimeouts) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  if (HAL_DSI_ConfigPhyTimer(&object->hdsi, &PhyTimings) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

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
  if (HAL_DSI_ConfigCommand(&object->hdsi, &LPCmd) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  if (HAL_DSI_ConfigAdaptedCommandMode(&object->hdsi, &CmdCfg) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_DSI_SetGenericVCID(&object->hdsi, 0) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  //  LTDC init
  object->hltdc.Instance = LTDC;
  object->hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  object->hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  object->hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  object->hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  object->hltdc.Init.HorizontalSync = HSYNC;
  object->hltdc.Init.VerticalSync = VSYNC;
  object->hltdc.Init.AccumulatedHBP = HSYNC+HBP;
  object->hltdc.Init.AccumulatedVBP = VSYNC+VBP;
  object->hltdc.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  object->hltdc.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  object->hltdc.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  object->hltdc.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  object->hltdc.Init.Backcolor.Blue = 0;
  object->hltdc.Init.Backcolor.Green = 0;
  object->hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&object->hltdc) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  HAL_DSI_Start(&object->hdsi);

  __HAL_DSI_WRAPPER_DISABLE(&object->hdsi);
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = Xsize;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = Ysize;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = Xsize;
  pLayerCfg.ImageHeight = Ysize;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&object->hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  __HAL_DSI_WRAPPER_ENABLE(&object->hdsi);
  /* USER CODE BEGIN LTDC_Init 2 */
  
  /* Configure DSI PHY HS2LP and LP2HS timings */
    
  

  object->dsi_IO_write=DSI_IO_WRITE;
  object->dsi_IO_read=DSI_IO_READ;
  object->dsi_refresh=DSI_Refresh;
  object->copy_buffer_M2M=copy_buffer_M2M;
  object->copy_buffer_R2M=copy_buffer_R2M;
}

void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d)
{
  if(hdma2d->Instance==DMA2D)
  {
    __HAL_RCC_DMA2D_CLK_ENABLE();
    /* DMA2D interrupt Init */
    HAL_NVIC_SetPriority(DMA2D_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(DMA2D_IRQn);
  }
}

void HAL_DSI_MspInit(DSI_HandleTypeDef* hdsi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hdsi->Instance==DSI)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DSI;
    PeriphClkInitStruct.DsiClockSelection = RCC_DSICLKSOURCE_PHY;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    /* Peripheral clock enable */
    __HAL_RCC_DSI_CLK_ENABLE();

    __HAL_RCC_GPIOJ_CLK_ENABLE();
    /**DSIHOST GPIO Configuration
    DSI_D1P     ------> DSIHOST_D1P
    DSI_D1N     ------> DSIHOST_D1N
    DSI_CKP     ------> DSIHOST_CKP
    DSI_CKN     ------> DSIHOST_CKN
    DSI_D0P     ------> DSIHOST_D0P
    DSI_D0N     ------> DSIHOST_D0N
    PJ2         ------> DSIHOST_TE
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DSI;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct); 

    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_12, GPIO_PIN_RESET);
    /* DSI interrupt Init */
    HAL_NVIC_SetPriority(DSI_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(DSI_IRQn);
  }
}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hltdc->Instance==LTDC)
  {
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLL3.PLL3M = 5;    
    PeriphClkInitStruct.PLL3.PLL3N = 160;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
    PeriphClkInitStruct.PLL3.PLL3P = 2;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;  
    PeriphClkInitStruct.PLL3.PLL3R = 19;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler(__FILE__, __LINE__);
    }

    /* Peripheral clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();
    /* LTDC interrupt Init */
    HAL_NVIC_SetPriority(LTDC_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
  }

}

void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&object_temp->hltdc);
}

void DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&object_temp->hdma2d);
}

void DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&object_temp->hdsi);
}

void copy_buffer_M2M(ltdc_dsi_objectTypeDef *object,uint32_t *src,uint16_t x,uint16_t y,uint16_t xsize,uint16_t ysize)
{
  uint32_t destination = (uint32_t)LCD_FRAME_BUFFER + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)src;
  
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  object->hdma2d.Init.Mode         = DMA2D_M2M;
  object->hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  object->hdma2d.Init.OutputOffset = 800 - xsize;
  object->hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  object->hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */   
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  object->hdma2d.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  object->hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  object->hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  object->hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  object->hdma2d.LayerCfg[1].InputOffset = 0;
  object->hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  object->hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */
  
  object->hdma2d.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&object->hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&object->hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&object->hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&object->hdma2d, 100);
      }
    }
  }   
}

void copy_buffer_R2M(ltdc_dsi_objectTypeDef *object,uint32_t *dst,uint16_t xsize,uint16_t ysize,uint16_t offline,uint32_t color)
{
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  object->hdma2d.Init.Mode         = DMA2D_R2M;
  object->hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  object->hdma2d.Init.OutputOffset = offline;
  object->hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/  
  object->hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */   
  
  object->hdma2d.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&object->hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&object->hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&object->hdma2d, color, (uint32_t*)dst, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&object->hdma2d, 100);
      }
    }
  }   
}

void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef* hdsi)
{

}

void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  // if(pend_buffer >= 0)
  // { 
  // /* Disable DSI Wrapper */
  //   __HAL_DSI_WRAPPER_DISABLE(hdsi);
  //   /* Update LTDC configuaration */
  //   LTDC_LAYER(hdsi, 0)->CFBAR = ((uint32_t)Buffers[pend_buffer]);
  //   __HAL_LTDC_RELOAD_CONFIG(&object_temp->hltdc);
  //   /* Enable DSI Wrapper */
  //   __HAL_DSI_WRAPPER_ENABLE(hdsi);
    
  //   front_buffer = pend_buffer;  
  //   pend_buffer = -1;
  // }
}

void DSI_IO_WRITE(ltdc_dsi_objectTypeDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size)
{
  if (size <= 1U)
  {
      if (HAL_DSI_ShortWrite(&object->hdsi, chNbr, DSI_DCS_SHORT_PKT_WRITE_P1, reg, (uint32_t)data[size]) != HAL_OK)
      {
          Error_Handler(__FILE__, __LINE__);
      }
  }
  else
  {
      if (HAL_DSI_LongWrite(&object->hdsi, chNbr, DSI_DCS_LONG_PKT_WRITE, size, (uint32_t)reg, data) != HAL_OK)
      {
          Error_Handler(__FILE__, __LINE__);
      }
  }
}

void DSI_IO_READ(ltdc_dsi_objectTypeDef *object,uint16_t chNbr, uint16_t reg, uint8_t* data, uint16_t size)
{
  if (HAL_DSI_Read(&object->hdsi, chNbr, data, size, DSI_DCS_SHORT_PKT_READ, reg, data) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}

void DSI_Refresh(ltdc_dsi_objectTypeDef *object)
{
  if (HAL_DSI_Refresh(&object->hdsi) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}
