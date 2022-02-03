#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "../../../BSP/system.h"

osThreadId_t defaultTaskHandle;
osThreadId_t touchTaskID;

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

uint32_t array_addr=0;
lcd_objectTypeDef otm8009a_obj;
touch_objectTypeDef touch_object;

void StartDefaultTask(void *argument);
void touchTask(void *argument);
uint32_t *start_addr=LCD_FRAME_BUFFER;

int main(void)
{
  Hardware_Init();
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
  
  lcd_init(&otm8009a_obj,0,0);
  // touch_object_init(&touch_object,0);

  osKernelInitialize();
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  touchTaskID=osThreadNew(touchTask, NULL, &touchTask_attributes);
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
  uint16_t x_count=0;
  uint16_t y_count=0;
  while(1)
  {
    // if(touch_object.chip_id!=0)
    // {
    //   touch_object.touch_scanState(&touch_object);
    //   otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,0,800,480,LCD_COLOR_DARKGRAY);
    //   otm8009a_obj.lcd_showString(&otm8009a_obj,0,0,&Font24,LCD_COLOR_WHITE,"touch state=%d",touch_object.touch_state);
    //   otm8009a_obj.lcd_showString(&otm8009a_obj,0,24,&Font24,LCD_COLOR_WHITE,"touch x=%d,y=%d",touch_object.x[0],touch_object.y[0]);
    //   otm8009a_obj.dsi_object.dsi_refresh(&otm8009a_obj.dsi_object);  
    // }
    
    // otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,0,480,854,LCD_COLOR_DARKGRAY);

    otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,0,480,854,LCD_COLOR_DARKGRAY);
    // otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,0,480,50,LCD_COLOR_WHITE);

    // otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,200,480,200,LCD_COLOR_DARKRED);
    // otm8009a_obj.lcd_draw_rect(&otm8009a_obj,0,500,480,854,LCD_COLOR_BLUE);
    // otm8009a_obj.dsi_object.dsi_refresh(&otm8009a_obj.dsi_object);  
    osDelay(100);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
