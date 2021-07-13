#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "../../../BSP/system.h"

osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

uint32_t test=0;

void PeriphCommonClock_Config(void);

void StartDefaultTask(void *argument);

int main(void)
{
  Hardware_Init();
  osKernelInitialize();
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  osKernelStart();

  while (1)
  {
  }
}

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    test++;
    osDelay(1);
  }
  /* USER CODE END 5 */
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
