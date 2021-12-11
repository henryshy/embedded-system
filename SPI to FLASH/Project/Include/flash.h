#ifndef _FLASH_H 
#define _FLASH_H 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f107.h"

/* Private function prototypes -----------------------------------------------*/
uint8_t Flash_ReadWriteByte(uint8_t data);
uint8_t SSTF016B_RD(uint32_t Dst, uint8_t* RcvBufPt ,uint32_t NByte);
uint8_t SSTF016B_RdID(idtype IDType,uint32_t* RcvbufPt);
uint8_t SSTF016B_WR(uint32_t Dst, uint8_t* SndbufPt,uint32_t NByte);
uint8_t SSTF016B_Erase(uint32_t sec1, uint32_t sec2);
void SPI_FLASH_Test(void);
void df_read_open(uint32_t addr);
void df_write_open(uint32_t addr);
void df_read(uint8_t *buf,uint16_t size);
void df_write(uint8_t *buf,uint16_t size);
void df_read_seek(uint32_t addr);
void df_write_seek(uint32_t addr);
void df_read_close(void);
void df_write_close(void);

#endif 
