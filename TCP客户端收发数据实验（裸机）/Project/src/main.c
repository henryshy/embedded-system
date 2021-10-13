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
#include "stm32_eth.h"
#include "netconf.h"
#include "main.h"
#include "helloworld.h"
#include "httpd.h"
#include "tftpserver.h"
#include "lwip/tcp.h"
#include "TCP_CLIENT.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
/* Private function prototypes -----------------------------------------------*/
void System_Periodic_Handle(void);
void Delay(uint32_t nCount);
void Delay_s(unsigned int t);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
   unsigned char tcp_data[] = "tcp 客户端实验!\n";
   struct tcp_pcb *pcb;
  /* Setup STM32 system (clocks, Ethernet, GPIO, NVIC) and STM3210C-EVAL resources */
  	System_Setup();       
  /* Initilaize the LwIP satck ip地址设置，mac设置，*/
    LwIP_Init();
	  /* Initilaize the webserver module */
	TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); //tcp客户端初始化
	while(1)
	{    
		pcb = Check_TCP_Connect();
		if(pcb != 0)
		{	
			TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//该函数为主动向服务器发送函数，
		}
		Delay_s(0xfffff);											//必要的延时
		System_Periodic_Handle();									//作为服务器的例行事件服务，主要是更新TCP timers
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  Handles the periodic tasks of the system
  * @param  None
  * @retval None
  */
void System_Periodic_Handle(void)
{
  /* Update the LCD display and the LEDs status */
  /* Manage the IP address setting */
  Display_Periodic_Handle(LocalTime);
  
  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(LocalTime);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
