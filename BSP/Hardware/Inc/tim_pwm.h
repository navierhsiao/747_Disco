#ifndef __TIM_PWM_H
#define __TIM_PWM_H

#include "stm32h7xx_hal.h"

enum TIM_CHANNEL_STATE{
   CHANNEL_RESET     =0,
   CHANNEL_ENABLED   =1,
   CHANNEL_READY     =2,
   CHANNEL_BUSY      =3,
   CHANNEL_ERROR     =4
};

typedef struct 
{
   TIM_TypeDef          *instance;
   uint32_t             frequency;
   uint32_t             CounterMode;     
   uint32_t             ClockDivision;  
   uint32_t             AutoReloadPreload; 

   uint32_t             enabled_channel[6];
   uint32_t             OCMode;       
   uint32_t             OCPolarity;  
   uint32_t             OCFastMode;   
   uint32_t             channel_list[6];
}pwm_tim_objectInitAttr;

typedef struct
{
   uint32_t setDuty[6];
   uint32_t setFrequency;
   uint32_t period;    
   uint32_t prescaler; 
   uint16_t pulse[6];    
   uint32_t channel_state[6];  
}pwm_tim_objectAttr;


typedef struct pwm_tim_structDef
{
   __IO TIM_HandleTypeDef       htim;
   __IO pwm_tim_objectInitAttr  init_attr;
   __IO pwm_tim_objectAttr      object_attr;

   void (*pwm_tim_init)             (struct pwm_tim_structDef *object);
   void (*pwm_tim_setFrequency)     (struct pwm_tim_structDef *object,uint32_t frequency);
   void (*pwm_tim_setDuty)          (struct pwm_tim_structDef *object,uint8_t channel,uint16_t duty);
   void (*pwm_tim_setPulse)         (struct pwm_tim_structDef *object,uint8_t channel,uint16_t pulse);
   void (*pwm_tim_set_state)        (struct pwm_tim_structDef *object,uint8_t channel,uint8_t state);
}pwm_tim_objectTypeDef;

void pwm_tim_objectInit(pwm_tim_objectTypeDef *object,pwm_tim_objectInitAttr attr);
#endif