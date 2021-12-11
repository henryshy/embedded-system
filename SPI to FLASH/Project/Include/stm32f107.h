/**
  ******************************************************************************
  * @file    stm32f107.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   This file contains all the functions prototypes for the STM32F107 
  *          file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F107_H
#define __STM32F107_H
#include "stm32f10x.h"
#ifdef __cplusplus
 extern "C" {
#endif
typedef enum ERTYPE{Sec1,Sec8,Sec16,Chip} ErType;  
typedef enum IDTYPE{Manu_ID,Dev_ID,Jedec_ID} idtype;
/* Includes ------------------------------------------------------------------*/
#define DATASIZE 24
#define MAXSIZE  50

#define MAX_ADDR		0x1FFFFF	/* 定义芯片内部最大地址 */
#define	SEC_MAX     	511         /* 定义最大扇区号 */
#define SEC_SIZE		0x1000      /* 扇区大小	*/
	 
#define RCC_APB2Periph_GPIO_SPI_FLASH_CS      RCC_APB2Periph_GPIOB
#define SPI_FALSH_CS_PORT                     GPIOB 
#define SPI_FALSH_CS_PIN                      GPIO_Pin_0 

#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_0)
extern volatile unsigned int flag;
extern volatile unsigned char rec[MAXSIZE];
extern volatile  int length;
	 
void USART2_configuration(int baud);
void System_Setup(void);
void SPI_FLASH_Init(void);//SPI1 config
void usart_sendstr(uint8_t sendbuff[], int length);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10F107_H */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
