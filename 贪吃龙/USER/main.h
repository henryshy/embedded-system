#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "sram.h"   
#include "malloc.h" 
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"	
#include "string.h"	
#include "math.h"	
#include "rs485.h"
#include "touch.h"
#include "flash.h"
#include "timer.h"
#include "snake.h"


extern 	u16 *picindextbl;	//图片索引表 　
extern FILINFO picfileinfo;//文件信息　;

#endif