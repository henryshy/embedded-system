#include "stm32f107.h"
#include "main.h"
#include "flash.h"
static uint32_t CurReadAddr;	/* 当前读的地址 */
static uint32_t CurWriteAddr;	/* 当前写页地址	*/

uint8_t Flash_ReadWriteByte(uint8_t data)		
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI1, data);
    
  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);                                              
}

uint8_t SSTF016B_RD(uint32_t Dst, uint8_t* RcvBufPt ,uint32_t NByte)
{
	uint32_t i = 0;
	if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (ERROR);	 /*	检查入口参数 */
	
    SPI_FLASH_CS_LOW();
	Flash_ReadWriteByte(0x0B); 						/* 发送读命令 */
	Flash_ReadWriteByte(((Dst & 0xFFFFFF) >> 16));	/* 发送地址信息:该地址由3个字节组成	*/
	Flash_ReadWriteByte(((Dst & 0xFFFF) >> 8));
	Flash_ReadWriteByte(Dst & 0xFF);
	Flash_ReadWriteByte(0xFF);						/* 发送一个哑字节以读取数据	*/
	for (i = 0; i < NByte; i++)		
	{
       RcvBufPt[i] = Flash_ReadWriteByte(0xFF);		
	}
    SPI_FLASH_CS_HIGH();
	return (ENABLE);
}

uint8_t SSTF016B_RdID(idtype IDType,uint32_t* RcvbufPt)
{
	uint32_t temp = 0;
              
	if (IDType == Jedec_ID)
	{
		SPI_FLASH_CS_LOW();	
				
		Flash_ReadWriteByte(0x9F);		 		         /* 发送读JEDEC ID命令(9Fh)	*/
    	        
        temp = (temp | Flash_ReadWriteByte(0x00)) << 8;  /* 接收数据 */
		temp = (temp | Flash_ReadWriteByte(0x00)) << 8;	
        temp = (temp | Flash_ReadWriteByte(0x00)); 	     /* 在本例中,temp的值应为0xBF2541 */

        SPI_FLASH_CS_HIGH();

		*RcvbufPt = temp;
		return (ENABLE);
	}
	
	if ((IDType == Manu_ID) || (IDType == Dev_ID) )
	{
	    SPI_FLASH_CS_LOW();	
		
		Flash_ReadWriteByte(0x90);				/* 发送读ID命令 (90h or ABh) */
    	Flash_ReadWriteByte(0x00);				/* 发送地址	*/
		Flash_ReadWriteByte(0x00);				/* 发送地址	*/
		Flash_ReadWriteByte(IDType);		    /* 发送地址 - 不是00H就是01H */
		temp = Flash_ReadWriteByte(0x00);	    /* 接收获取的数据字节 */

        SPI_FLASH_CS_HIGH();

		*RcvbufPt = temp;
		return (ENABLE);
	}
	else
	{
		return (ERROR);	
	}
}
uint8_t SSTF016B_WR(uint32_t Dst, uint8_t* SndbufPt,uint32_t NByte)
{
	uint8_t temp = 0,StatRgVal = 0;
	uint32_t i = 0;
	if (( (Dst+NByte-1 > MAX_ADDR)||(NByte == 0) ))
	{
		return (ERROR);	 /*	检查入口参数 */
	}

	
	SPI_FLASH_CS_LOW();				 
	Flash_ReadWriteByte(0x05);							    /* 发送读状态寄存器命令	*/
	temp = Flash_ReadWriteByte(0xFF);					    /* 保存读得的状态寄存器值 */
	SPI_FLASH_CS_HIGH();								

	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(0x50);							    /* 使状态寄存器可写	*/
	SPI_FLASH_CS_HIGH();	
		
	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(0x01);							    /* 发送写状态寄存器指令 */
	Flash_ReadWriteByte(0);								    /* 清0BPx位，使Flash芯片全区可写 */
	SPI_FLASH_CS_HIGH();			
	
	for(i = 0; i < NByte; i++)
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* 发送写使能命令 */
		SPI_FLASH_CS_HIGH();			

		SPI_FLASH_CS_LOW();					
		Flash_ReadWriteByte(0x02); 						    /* 发送字节数据烧写命令	*/
		Flash_ReadWriteByte((((Dst+i) & 0xFFFFFF) >> 16));  /* 发送3个字节的地址信息 */
		Flash_ReadWriteByte((((Dst+i) & 0xFFFF) >> 8));
		Flash_ReadWriteByte((Dst+i) & 0xFF);
		Flash_ReadWriteByte(SndbufPt[i]);					/* 发送被烧写的数据	*/
		SPI_FLASH_CS_HIGH();			

		do
		{
		  	SPI_FLASH_CS_LOW();					 
			Flash_ReadWriteByte(0x05);					    /* 发送读状态寄存器命令 */
			StatRgVal = Flash_ReadWriteByte(0xFF);			/* 保存读得的状态寄存器值 */
			SPI_FLASH_CS_HIGH();							
  		}
		while (StatRgVal == 0x03 );					          /* 一直等待，直到芯片空闲	*/

	}

	SPI_FLASH_CS_LOW();					
	Flash_ReadWriteByte(0x06);							    /* 发送写使能命令 */
	SPI_FLASH_CS_HIGH();			

	SPI_FLASH_CS_LOW();					
	Flash_ReadWriteByte(0x50);							    /* 使状态寄存器可写	*/
	SPI_FLASH_CS_HIGH();
			
	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(0x01);							    /* 发送写状态寄存器指令 */
	Flash_ReadWriteByte(temp);						     	/* 恢复状态寄存器设置信息 */
	SPI_FLASH_CS_HIGH();

	return (ENABLE);		
}


uint8_t SSTF016B_Erase(uint32_t sec1, uint32_t sec2)
{
	uint8_t  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32_t SecnHdAddr = 0;	  			
	uint32_t no_SecsToEr = 0;				   			    /* 要擦除的扇区数目 */
	uint32_t CurSecToEr = 0;	  						    /* 当前要擦除的扇区号 */
	
	/*  检查入口参数 */
	if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))	
	{
		return (ERROR);	
	}
   	
   	SPI_FLASH_CS_LOW();			 
	Flash_ReadWriteByte(0x05);								/* 发送读状态寄存器命令	*/
	temp1 = Flash_ReadWriteByte(0x00);						/* 保存读得的状态寄存器值 */
	SPI_FLASH_CS_HIGH();								

	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x50);								/* 使状态寄存器可写	*/
	SPI_FLASH_CS_HIGH();			

	SPI_FLASH_CS_LOW();								  	
	Flash_ReadWriteByte(0x01);								/* 发送写状态寄存器指令	*/
	Flash_ReadWriteByte(0);									/* 清0BPx位，使Flash芯片全区可写 */
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x06);								/* 发送写使能命令 */
	SPI_FLASH_CS_HIGH();			

	/* 如果用户输入的起始扇区号大于终止扇区号，则在内部作出调整 */
	if (sec1 > sec2)
	{
	   temp2 = sec1;
	   sec1  = sec2;
	   sec2  = temp2;
	} 
	/* 若起止扇区号相等则擦除单个扇区 */
	if (sec1 == sec2)	
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* 发送写使能命令 */
		SPI_FLASH_CS_HIGH();			

	    SecnHdAddr = SEC_SIZE * sec1;				          /* 计算扇区的起始地址	*/
	    SPI_FLASH_CS_LOW();	
    	Flash_ReadWriteByte(0x20);							  /* 发送扇区擦除指令 */
	    Flash_ReadWriteByte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* 发送3个字节的地址信息 */
   		Flash_ReadWriteByte(((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_ReadWriteByte(SecnHdAddr & 0xFF);
  		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* 发送读状态寄存器命令 */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* 保存读得的状态寄存器值	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);				              /* 一直等待，直到芯片空闲	*/
		return (ENABLE);			
	}
	
    /* 根据起始扇区和终止扇区间距调用最快速的擦除功能 */	
	
	if (sec2 - sec1 == SEC_MAX)	
	{
		SPI_FLASH_CS_LOW();			
		Flash_ReadWriteByte(0x60);							  /* 发送芯片擦除指令(60h or C7h) */
		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* 发送读状态寄存器命令 */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* 保存读得的状态寄存器值	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);					          /* 一直等待，直到芯片空闲	*/
   		return (ENABLE);
	}
	
	no_SecsToEr = sec2 - sec1 +1;					          /* 获取要擦除的扇区数目 */
	CurSecToEr  = sec1;								          /* 从起始扇区开始擦除	*/
	
	/* 若两个扇区之间的间隔够大，则采取8扇区擦除算法 */
	while (no_SecsToEr >= 8)
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* 发送写使能命令 */
		SPI_FLASH_CS_HIGH();			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			          /* 计算扇区的起始地址 */
	    SPI_FLASH_CS_LOW();	
	    Flash_ReadWriteByte(0x52);							  /* 发送32KB擦除指令 */
	    Flash_ReadWriteByte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* 发送3个字节的地址信息 */
   		Flash_ReadWriteByte(((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_ReadWriteByte(SecnHdAddr & 0xFF);
  		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* 发送读状态寄存器命令 */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* 保存读得的状态寄存器值	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);					          /* 一直等待，直到芯片空闲	*/
		CurSecToEr  += 8;
		no_SecsToEr -=  8;
	}
	/* 采用扇区擦除算法擦除剩余的扇区 */
	while (no_SecsToEr >= 1)
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* 发送写使能命令 */
		SPI_FLASH_CS_HIGH();			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			          /* 计算扇区的起始地址 */
	    SPI_FLASH_CS_LOW();	
    	Flash_ReadWriteByte(0x20);							  /* 发送扇区擦除指令 */
	    Flash_ReadWriteByte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* 发送3个字节的地址信息 */
   		Flash_ReadWriteByte(((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_ReadWriteByte(SecnHdAddr & 0xFF);
  		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* 发送读状态寄存器命令 */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* 保存读得的状态寄存器值	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);					          /* 一直等待，直到芯片空闲 */
		CurSecToEr  += 1;
		no_SecsToEr -=  1;
	}
    /* 擦除结束,恢复状态寄存器信息 */
	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x06);								  /* 发送写使能命令 */
	SPI_FLASH_CS_HIGH();			

	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x50);								  /* 使状态寄存器可写 */
	SPI_FLASH_CS_HIGH();			
	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x01);								  /* 发送写状态寄存器指令 */
	Flash_ReadWriteByte(temp1);								  /* 恢复状态寄存器设置信息 */
	SPI_FLASH_CS_HIGH();    
	return (ENABLE);
}


void SPI_FLASH_Test(void)
{
    uint32_t  ChipID = 0;
  	SSTF016B_RdID(Jedec_ID, &ChipID);                                 


	if (ChipID != 0xBF2541) {											/*  ID不正确进入死循环          */
           while(1);
    }

}
void df_read_open(uint32_t addr)
{
		CurReadAddr=addr;	/* 当前读的地址	*/
}
void df_write_open(uint32_t addr)
{
		CurWriteAddr=addr;	/* 当前写地址 */
}
void df_read(uint8_t *buf,uint16_t size)
{
  if( CurReadAddr + size <= MAX_ADDR )
  {
    SSTF016B_RD(CurReadAddr,buf,size);
    CurReadAddr+=size;
  }
}
void df_write(uint8_t *buf,uint16_t size)
{
  if( CurWriteAddr + size <= MAX_ADDR )
  {
    SSTF016B_WR(CurWriteAddr,buf,size);
    CurWriteAddr+=size;
  }
}
void df_read_seek(uint32_t addr)
{

 df_read_open(addr);
}
void df_write_seek(uint32_t addr)
{

 df_write_open(addr); 
}
