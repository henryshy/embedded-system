#include "timconf.h"

void TIM_conf(void){
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
   TIM_TimeBaseStructure.TIM_Period = 29; //3.5个字节的计数间隔,计数约为29次
   TIM_TimeBaseStructure.TIM_Prescaler =7200; //0.0001s计数一次
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
   TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);  
	 TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	 TIM_ClearFlag(TIM7,TIM_FLAG_Update);
	
	 NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;  
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
   NVIC_Init(&NVIC_InitStructure);  
	
 }
