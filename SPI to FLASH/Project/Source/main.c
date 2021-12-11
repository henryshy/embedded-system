/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32f107.h"
#include "flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void usart_sendstr(uint8_t sendbuff[], int length){
		for(int i = 0;i < length;i ++)
	{			
		USART_SendData(USART2,sendbuff[i]);
		while((USART2->SR&0X40)==0);	
	}	
}
int main(void)
{

	uint8_t readbuff[256];
	uint8_t writebuff[256];
	uint8_t flash_start_addr=0;
	System_Setup();

	for(int i=0;i<255;i++){
		writebuff[i]=i;
	}
	SPI_FLASH_Test();
	SSTF016B_Erase( flash_start_addr/SEC_SIZE , ( flash_start_addr + sizeof(writebuff) )/SEC_SIZE +1 );
	df_write_open( flash_start_addr );    /* ��Ҫд�ĳ�ʼ��ַ */
	df_write( (unsigned char*)writebuff,256); //��WriteBufferָ���256���ֽ�д��flash
	df_read_open(flash_start_addr);    /* ��ȡ�ĳ�ʼ��ַ */ 
	df_read(readbuff, 256); //��ȡflash�е�256���ֽڷŵ�ReadBufferָ���ĵ�ַ
	usart_sendstr(readbuff,256);

}
