#include "main.h"




GPIO_InitTypeDef GPIO_InitStructure;
TaskHandle_t ledtask1_handler;
TaskHandle_t ledtask2_handler;
TaskHandle_t starttask_handler;
int main(void)
{	

  LED_configuration();
  xTaskCreate((TaskFunction_t)start_task,
							(const char*)		"start",
							(uint16_t)			start_task_stk_size,
							(void*)					NULL,
							(UBaseType_t)   start_task_prio,
							(TaskHandle_t*) &starttask_handler);
            
    vTaskStartScheduler();  
  
  
  while(1);     
}
void start_task(void* parameter){
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t)ledtask1,
							(const char*)		"led1",
							(uint16_t)			task1_stk_size,
							(void*)					NULL,
							(UBaseType_t)   task1_prio,
							(TaskHandle_t*) &ledtask1_handler);
							
	xTaskCreate((TaskFunction_t)ledtask2,
							(const char*)		"led2",
							(uint16_t)			task2_stk_size,
							(void*)					NULL,
							(UBaseType_t)   task2_prio,
							(TaskHandle_t*) &ledtask2_handler);
	
	
	vTaskDelete(starttask_handler);
	taskEXIT_CRITICAL();
}
void ledtask1(void* parameter)
{	
    while (1)
    {
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);
				vTaskDelay(500);		
				GPIO_SetBits(GPIOC,GPIO_Pin_0);	
				vTaskDelay(500);			
    }
}
void ledtask2(void* parameter){
	 while(1){
		    GPIO_ResetBits(GPIOC,GPIO_Pin_6);
				vTaskDelay(500);		
				GPIO_SetBits(GPIOC,GPIO_Pin_6);	
				vTaskDelay(500);	
	 }
	
}

void LED_configuration(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
}