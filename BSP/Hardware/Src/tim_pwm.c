#include "../../system.h"

/*
**************************************************************************
*       TIM引腳
*       PWM
*           BL_CTRL ------------------> PB3		(TIM2_CH2)
**************************************************************************
*/

uint32_t objectAddr=0;

void pwm_tim_init(pwm_tim_objectTypeDef *object);
void pwm_tim_setFrequency(pwm_tim_objectTypeDef *object,uint32_t frequency);
void pwm_tim_setDuty(pwm_tim_objectTypeDef *object,uint8_t channel,uint16_t duty);
void pwm_tim_setPulse(pwm_tim_objectTypeDef *object,uint8_t channel,uint16_t pulse);
void pwm_tim_set_state(pwm_tim_objectTypeDef *object,uint8_t channel,uint8_t state);

void pwm_tim_objectInit(pwm_tim_objectTypeDef *object,pwm_tim_objectInitAttr attr)
{
	object->init_attr=attr;
	object->object_attr.setFrequency=attr.frequency;

	object->pwm_tim_init=pwm_tim_init;
	object->pwm_tim_setFrequency=pwm_tim_setFrequency;
	object->pwm_tim_setDuty=pwm_tim_setDuty;
	object->pwm_tim_set_state=pwm_tim_set_state;
	object->pwm_tim_setPulse=pwm_tim_setPulse;

	object->init_attr.channel_list[0]=TIM_CHANNEL_1;
	object->init_attr.channel_list[1]=TIM_CHANNEL_2;
	object->init_attr.channel_list[2]=TIM_CHANNEL_3;
	object->init_attr.channel_list[3]=TIM_CHANNEL_4;
	object->init_attr.channel_list[4]=TIM_CHANNEL_5;
	object->init_attr.channel_list[5]=TIM_CHANNEL_6;

	object->pwm_tim_init(object);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_TIM2_CLK_ENABLE();
		/**TIM1 GPIO Configuration
		PB3     ------> TIM2_CH2
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM1)
  {
    __HAL_RCC_TIM1_CLK_DISABLE();
  }
  else if(htim_pwm->Instance==TIM3)
  {
    __HAL_RCC_TIM3_CLK_DISABLE();
  }
  else if(htim_pwm->Instance==TIM4)
  {
    __HAL_RCC_TIM4_CLK_DISABLE();
  }
}

void pwm_tim_init(pwm_tim_objectTypeDef *object)
{
	objectAddr=object;

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	uint32_t TIMx_CLK;

	TIMx_CLK = HAL_RCC_GetHCLKFreq();
	if (object->init_attr.frequency < 100)
	{
		object->object_attr.prescaler=24000-1;					
		object->object_attr.period=(TIMx_CLK/24000)/object->init_attr.frequency-1;	
	}
	else if (object->init_attr.frequency<4000)
	{
		object->object_attr.prescaler=240-1;				
		object->object_attr.period=(TIMx_CLK/240)/object->init_attr.frequency-1;	
	}
	else	/* 大於4k頻率無需分頻 */
	{
		object->object_attr.prescaler=0;				
		object->object_attr.period=TIMx_CLK/object->init_attr.frequency-1;	
	}

	object->htim.Instance = object->init_attr.instance;
	object->htim.Init.Prescaler = object->object_attr.prescaler;
	object->htim.Init.CounterMode = object->init_attr.CounterMode;
	object->htim.Init.Period = object->object_attr.period;
	object->htim.Init.ClockDivision = object->init_attr.ClockDivision;
	object->htim.Init.AutoReloadPreload = object->init_attr.AutoReloadPreload;
	if (HAL_TIM_PWM_Init(&object->htim) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&object->htim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
	sConfigOC.OCMode = object->init_attr.OCMode;
	sConfigOC.Pulse = object->object_attr.pulse;
	sConfigOC.OCPolarity = object->init_attr.OCPolarity;
	sConfigOC.OCFastMode = object->init_attr.OCFastMode;

	for(int i=0;i<6;i++)
	{
		if(object->init_attr.enabled_channel[i]==CHANNEL_ENABLED)
		{
			if (HAL_TIM_PWM_ConfigChannel(&object->htim, &sConfigOC, object->init_attr.channel_list[i]) != HAL_OK)
			{
				object->object_attr.channel_state[i]=CHANNEL_ERROR;
				Error_Handler(__FILE__, __LINE__);
			}
			object->object_attr.channel_state[i]=CHANNEL_READY;
		}
	}
}

uint32_t pwmtim_obj_addr_inSF=0;

void pwm_tim_setFrequency(pwm_tim_objectTypeDef *object,uint32_t frequency)
{
	HAL_TIM_PWM_DeInit(&object->htim);
	object->object_attr.setFrequency=frequency;
	object->init_attr.frequency=frequency;
    object->pwm_tim_init(&object->htim);
}

void pwm_tim_setDuty(pwm_tim_objectTypeDef *object,uint8_t channel,uint16_t duty)
{
	if(channel<1||channel>6)
	{
		return;
	}

	object->object_attr.setDuty[channel-1]=duty;
	object->object_attr.pulse[channel-1]=(object->object_attr.setDuty[channel-1]*object->object_attr.period)/10000;

	if((object->init_attr.enabled_channel[channel-1]==CHANNEL_RESET)||(object->init_attr.enabled_channel[channel-1]==CHANNEL_ERROR))
	{
		object->object_attr.channel_state[channel-1]=CHANNEL_ERROR;
		return;
	}

	if(channel==1)
	{
		object->init_attr.instance->CCR1=object->object_attr.pulse[channel-1];
	}
	else if(channel==2)
	{
		object->init_attr.instance->CCR2=object->object_attr.pulse[channel-1];
	}
	else if(channel==3)
	{
		object->init_attr.instance->CCR3=object->object_attr.pulse[channel-1];
	}
	else if(channel==4)
	{
		object->init_attr.instance->CCR4=object->object_attr.pulse[channel-1];
	}
	else if(channel==5)
	{
		object->init_attr.instance->CCR5=object->object_attr.pulse[channel-1];
	}
	else if(channel==6)
	{
		object->init_attr.instance->CCR6=object->object_attr.pulse[channel-1];
	}
}

void pwm_tim_setPulse(pwm_tim_objectTypeDef *object,uint8_t channel,uint16_t pulse)
{
	if(channel<1||channel>6)
	{
		return;
	}

	object->object_attr.pulse[channel-1]=pulse;
	object->object_attr.setDuty[channel-1]=((double)(object->object_attr.pulse[channel-1])/(object->object_attr.period))*10000;

	if((object->init_attr.enabled_channel[channel-1]==CHANNEL_RESET)||(object->init_attr.enabled_channel[channel-1]==CHANNEL_ERROR))
	{
		object->object_attr.channel_state[channel-1]=CHANNEL_ERROR;
		return;
	}

	if(channel==1)
	{
		object->init_attr.instance->CCR1=object->object_attr.pulse[channel-1];
	}
	else if(channel==2)
	{
		object->init_attr.instance->CCR2=object->object_attr.pulse[channel-1];
	}
	else if(channel==3)
	{
		object->init_attr.instance->CCR3=object->object_attr.pulse[channel-1];
	}
	else if(channel==4)
	{
		object->init_attr.instance->CCR4=object->object_attr.pulse[channel-1];
	}
	else if(channel==5)
	{
		object->init_attr.instance->CCR5=object->object_attr.pulse[channel-1];
	}
	else if(channel==6)
	{
		object->init_attr.instance->CCR6=object->object_attr.pulse[channel-1];
	}
}

void pwm_tim_set_state(pwm_tim_objectTypeDef *object,uint8_t channel,uint8_t state)
{
	if(channel<1||channel>6)
	{
		return;
	}

	if(state==0)
	{
		if(object->object_attr.channel_state[channel-1]!=CHANNEL_READY)
		{
			object->object_attr.channel_state[channel-1]=CHANNEL_READY;
			HAL_TIM_PWM_Stop(&object->htim,object->init_attr.channel_list[channel-1]);
		}
	}
	else if(state==1)
	{
		if(object->object_attr.channel_state[channel-1]!=CHANNEL_BUSY)
		{
			object->object_attr.channel_state[channel-1]=CHANNEL_BUSY;
			HAL_TIM_PWM_Start(&object->htim,object->init_attr.channel_list[channel-1]);
		}
	}
}