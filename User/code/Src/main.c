#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "../../../BSP/system.h"

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

uint32_t array_addr=0;
lcd_objectTypeDef   lcd_obj;
touch_objectTypeDef touch_object;
sdmmc_objectTypeDef *sd_object;

void StartDefaultTask(void *argument);
void touchTask(void *argument);
void sdTask(void *argument);

uint32_t *start_addr=LCD_FRAME_BUFFER;

int main(void)
{
  Hardware_Init();
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
  
  lcd_init(&lcd_obj,0,0);
  touch_object_init(&touch_object,0);
  
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
  uint8_t count=0;
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END 5 */
}

void touchTask(void *argument)
{
  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_12, GPIO_PIN_SET);
  while(1)
  {
    lcd_obj.lcd_draw_rect(&lcd_obj,0,0,480,854,LCD_COLOR_DARKGRAY);
    if(touch_object.chip_id!=0)
    {
      touch_object.touch_scanState(&touch_object);
      lcd_obj.lcd_showString(&lcd_obj,0,0,&Font24,LCD_COLOR_WHITE,"touch state=%d",touch_object.touch_state);
      lcd_obj.lcd_showString(&lcd_obj,0,24,&Font24,LCD_COLOR_WHITE,"touch x=%d,y=%d",touch_object.x[0],touch_object.y[0]);

      if(IS_SD_INSERTED==0)
      {
        if(sd_object!=NULL)
        {
          if(sd_object->is_card_detected)
          {
            lcd_obj.lcd_showString(&lcd_obj,0,48,&Font24,LCD_COLOR_WHITE,"sd card inserted");
            lcd_obj.lcd_showString(&lcd_obj,0,72,&Font24,LCD_COLOR_WHITE,"sd card capacity: %d MB",(uint32_t)((sd_object->cardInfo.LogBlockNbr)>>11));
          }
          else
          {
            lcd_obj.lcd_showString(&lcd_obj,0,48,&Font24,LCD_COLOR_WHITE,"no sd card inserted");
          }
        }
      }
      
      
      lcd_obj.dsi_object.dsi_refresh(&lcd_obj.dsi_object);
    }
    osDelay(100);
  }
}

void sdTask(void *argument)
{
  uint8_t read_flag=0;
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
  /*

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
