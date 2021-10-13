#include "main.h"
#include "usart2conf.h"
#include "timconf.h"
#include "stdio.h"
#include "gpioconf.h"

volatile uint32_t tx_flag=0;
	
volatile uint8_t rx_rec[50]={'\0'};
volatile uint8_t rx_rec_length=0;
volatile uint8_t rx_rec_cnt=0;

volatile uint8_t tx_rec[50]={'\0'};
volatile uint8_t tx_rec_length=0;
volatile uint8_t tx_rec_cnt=0;

volatile uint32_t* in_reg[100];//ÊäÈë¼Ä´æÆ÷
volatile uint32_t* out_reg[100];//Êä³ö¼Ä´æÆ÷
volatile uint32_t* hold_reg[100];//±£³Ö¼Ä´æÆ÷

volatile uint32_t in_testdata1=0x00001111;//³õÊ¼»¯¼Ä´æÆ÷²âÊÔÊı¾İ
volatile uint32_t in_testdata2=1325;
volatile uint32_t in_testdata3=4532;

volatile uint32_t out_testdata1=2143;
volatile uint32_t out_testdata2=0x11111111;
volatile uint32_t out_testdata3=3214;
volatile uint32_t out_testdata4=4554;

volatile uint32_t hold_testdata1=2143;
volatile uint32_t hold_testdata2=0x00001111;
volatile uint32_t hold_testdata3=3544;
volatile uint32_t hold_testdata4=3514;
volatile uint32_t hold_testdata5=4434;

void reg_init(void){
	in_reg[0]= &GPIOD->BSRR; //KEY1,2,3µÄ¼Ä´æÆ÷  gpioc_pin 10,11,12 
	in_reg[1]= &GPIOC->BSRR; //KEY4µÄ¼Ä´æÆ÷   gpiod_pin 3
	in_reg[2]=&in_testdata1;
	in_reg[3]=&in_testdata2;
	in_reg[4]=&in_testdata3;
	
	out_reg[0]=&GPIOC->BSRR; //LED1,2,3,4µÄ¼Ä´æÆ÷ gpioc_pin 0,6,7,8
	out_reg[1]=&out_testdata1;
	out_reg[2]=&out_testdata2;
	out_reg[3]=&out_testdata3;
	out_reg[4]=&out_testdata4;
	
	hold_reg[0]=&hold_testdata1; //³õÊ¼»¯±£´æ¼Ä´æÆ÷
	hold_reg[1]=&hold_testdata2;
	hold_reg[2]=&hold_testdata3;
	hold_reg[3]=&hold_testdata4;
	hold_reg[4]=&hold_testdata4;
}
void clear_rec(void){
	int i;
			 for(i=0;i<50;i++){
				rx_rec[i]='\0';
				tx_rec[i]='\0';
			}
			 rx_rec_length=0;
			tx_rec_length=0;
}
int main(void){
  USART2_configuration(usart_baud);
	TIM_conf();
	gpio_conf();
	reg_init();
	USART_SendData(USART2,'s');
	while(1){
		if(tx_flag){
			tx_flag=0;
			uint32_t i;
			for(i=0;i<tx_rec_length;i++){
				USART_SendData(USART2,tx_rec[i]);
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
			}
			clear_rec();
		}
	}
}	
		
		
