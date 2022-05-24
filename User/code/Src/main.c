#include "main.h"
#include "cmsis_os.h"
#include "../../../BSP/system.h"
#include "usb_device.h"

osThreadId_t defaultTaskHandle;
osThreadId_t touchTaskID;
osThreadId_t sdTaskID;

const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t touchTask_attributes = {
  .name = "touchTask",
  .stack_size = 1024,
  .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t sdTask_attributes = {
  .name = "SDTask",
  .stack_size = 1024,
  .priority = (osPriority_t) osPriorityNormal2,
};

lcd_objectTypeDef otm8009a_obj;
touch_objectTypeDef touch_object;
sdmmc_objectTypeDef *sd_object;

void StartDefaultTask(void *argument);
void touchTask(void *argument);
void sdTask(void *argument);

int main(void)
{
  Hardware_Init();
  lcd_init(&otm8009a_obj,OTM8009A_FORMAT_RGB888,OTM8009A_ORIENTATION_LANDSCAPE);
  touch_object_init(&touch_object,OTM8009A_ORIENTATION_LANDSCAPE);

  osKernelInitialize();
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  touchTaskID=osThreadNew(touchTask, NULL, &touchTask_attributes);
  sdTaskID=osThreadNew(sdTask, NULL, &sdTask_attributes);
  osKernelStart();

  while (1);
}

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

void touchTask(void *argument)
{
  while(1)
  {
    if(touch_object.chip_id!=0)
    {
      touch_object.touch_scanState(&touch_object);
      otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,0,800,480,LCD_COLOR_DARKGRAY);
      otm8009a_obj.lcd_showString(&otm8009a_obj,0,0,&Font24,LCD_COLOR_WHITE,"touch state=%d",touch_object.touch_state);
      otm8009a_obj.lcd_showString(&otm8009a_obj,0,24,&Font24,LCD_COLOR_WHITE,"touch x=%d,y=%d",touch_object.x[0],touch_object.y[0]);
      otm8009a_obj.dsi_object.dsi_refresh(&otm8009a_obj.dsi_object);  
      if(IS_SD_INSERTED==0)
      {
        if(sd_object!=NULL)
        {
            otm8009a_obj.lcd_showString(&otm8009a_obj,0,48,&Font24,LCD_COLOR_WHITE,"sd card inserted");
            otm8009a_obj.lcd_showString(&otm8009a_obj,0,72,&Font24,LCD_COLOR_WHITE,"sd card capacity: %d MB",(uint32_t)((sd_object->cardInfo.LogBlockNbr)>>11));
        }
      }
    }
    osDelay(10);
  }
}

void sdTask(void *argument)
{
  uint8_t read_flag=0;
  // HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_1, GPIO_PIN_SET);
  MX_USB_DEVICE_Init();
  for(;;)
  {
    if(sd_object!=NULL)
    {
      sd_object->sdmmc_scan_card_state(sd_object);
      if(sd_object->is_card_detected)
      {
        if(read_flag==0)
        {
          sd_object->sdmmc_get_cardInfo(sd_object);
          read_flag=1;
        }
      }
      else
      {
        sd_object=NULL;
        read_flag=0;
      }
    }
    else
    {
      if(IS_SD_INSERTED==0)
      {
        sd_object=sdmmc_object_init();
      }
    }
    
    osDelay(100);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
