#include "main.h"
#include "usart2conf.h"

void usart_sendstr(uint8_t sendbuff[], int length){
	int i;
		for( i = 0;i < length;i ++)
	{			
		USART_SendData(USART2,sendbuff[i]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	
	}	
}
int main(void)
{
    USART2_configuration(9600);
	while(1){
		if(flag==1){
			flag=0;
			usart_sendstr(rec,length);
			length=0;
		}
	}
	
}	
		
		
