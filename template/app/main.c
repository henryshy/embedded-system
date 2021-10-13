#include <stdio.h>
#include <stdlib.h>
#include <stm32f10x.h>
#include <stm32f10x_it.h>
#include <stm32f10x_conf.h>
#define led0 PCout(6)
void GPIO_Config(void)                            
{
	//led
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//key
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
		
}     
void rcc_config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

}
#define LED_ON GPIO_ResetBits(GPIOC, GPIO_Pin_6 )     
#define LED_OFF GPIO_SetBits(GPIOC, GPIO_Pin_6 )     
void delay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD=9000*nms;
	SysTick->VAL=0X00;
	SysTick->CTRL=0x01;
	
	do{
		temp=SysTick->CTRL;
	}while(!(temp&(1<<16)));
	SysTick->CTRL=0X00;
	SysTick->VAL=0X00;
	
}
int main()
{rcc_config();
	GPIO_Config();
	
	while(1){
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)==1){
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)==1){
				
				led0=~led0;
			}
		}
	}
   

}