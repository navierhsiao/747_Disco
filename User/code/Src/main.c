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

void StartDefaultTask(void *argument);
uint8_t test_array[200];
int main(void)
{
  Hardware_Init();
  osKernelInitialize();
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  osKernelStart();

  while (1);
}

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */

  for(;;)
  {
    // for(int i=0;i<128;i++)
    // {
    //   test[i]=i;
    // }

    // MT25TL01G_WriteBuffer(test,0x90000000,256);
    // test++;
    for(int i=0;i<200;i++)
    {
      uint32_t *addr=NULL;
      addr=0x90000000+4*i;
      test_array[i]=(uint8_t)(*addr);
    }
    osDelay(100);
  }
  /* USER CODE END 5 */
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
