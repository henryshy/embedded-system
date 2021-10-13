#include "main.h"
#include "usart2conf.h"


 volatile unsigned int flag=0;
 volatile uint8_t rec[MAXSIZE]={'\0'};
 volatile unsigned int length=0;

void USART2_configuration(int baud){
	USART_InitTypeDef USART_InitStucture;
  NVIC_InitTypeDef NVIC_InitStructure;	
  GPIO_InitTypeDef GPIO_InitStructure;


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,	ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	USART_InitStucture.USART_BaudRate=baud;
	USART_InitStucture.USART_WordLength=USART_WordLength_8b;
	USART_InitStucture.USART_StopBits=USART_StopBits_1;
	USART_InitStucture.USART_Parity=USART_Parity_No;
	USART_InitStucture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStucture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStucture);
	
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
	USART_Cmd(USART2,ENABLE);
}
