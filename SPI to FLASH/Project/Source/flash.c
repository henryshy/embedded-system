#include "stm32f107.h"
#include "main.h"
#include "flash.h"
static uint32_t CurReadAddr;	/* ��ǰ���ĵ�ַ */
static uint32_t CurWriteAddr;	/* ��ǰдҳ��ַ	*/

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
	if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	return (ERROR);	 /*	�����ڲ��� */
	
    SPI_FLASH_CS_LOW();
	Flash_ReadWriteByte(0x0B); 						/* ���Ͷ����� */
	Flash_ReadWriteByte(((Dst & 0xFFFFFF) >> 16));	/* ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����	*/
	Flash_ReadWriteByte(((Dst & 0xFFFF) >> 8));
	Flash_ReadWriteByte(Dst & 0xFF);
	Flash_ReadWriteByte(0xFF);						/* ����һ�����ֽ��Զ�ȡ����	*/
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
				
		Flash_ReadWriteByte(0x9F);		 		         /* ���Ͷ�JEDEC ID����(9Fh)	*/
    	        
        temp = (temp | Flash_ReadWriteByte(0x00)) << 8;  /* �������� */
		temp = (temp | Flash_ReadWriteByte(0x00)) << 8;	
        temp = (temp | Flash_ReadWriteByte(0x00)); 	     /* �ڱ�����,temp��ֵӦΪ0xBF2541 */

        SPI_FLASH_CS_HIGH();

		*RcvbufPt = temp;
		return (ENABLE);
	}
	
	if ((IDType == Manu_ID) || (IDType == Dev_ID) )
	{
	    SPI_FLASH_CS_LOW();	
		
		Flash_ReadWriteByte(0x90);				/* ���Ͷ�ID���� (90h or ABh) */
    	Flash_ReadWriteByte(0x00);				/* ���͵�ַ	*/
		Flash_ReadWriteByte(0x00);				/* ���͵�ַ	*/
		Flash_ReadWriteByte(IDType);		    /* ���͵�ַ - ����00H����01H */
		temp = Flash_ReadWriteByte(0x00);	    /* ���ջ�ȡ�������ֽ� */

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
		return (ERROR);	 /*	�����ڲ��� */
	}

	
	SPI_FLASH_CS_LOW();				 
	Flash_ReadWriteByte(0x05);							    /* ���Ͷ�״̬�Ĵ�������	*/
	temp = Flash_ReadWriteByte(0xFF);					    /* ������õ�״̬�Ĵ���ֵ */
	SPI_FLASH_CS_HIGH();								

	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(0x50);							    /* ʹ״̬�Ĵ�����д	*/
	SPI_FLASH_CS_HIGH();	
		
	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(0x01);							    /* ����д״̬�Ĵ���ָ�� */
	Flash_ReadWriteByte(0);								    /* ��0BPxλ��ʹFlashоƬȫ����д */
	SPI_FLASH_CS_HIGH();			
	
	for(i = 0; i < NByte; i++)
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* ����дʹ������ */
		SPI_FLASH_CS_HIGH();			

		SPI_FLASH_CS_LOW();					
		Flash_ReadWriteByte(0x02); 						    /* �����ֽ�������д����	*/
		Flash_ReadWriteByte((((Dst+i) & 0xFFFFFF) >> 16));  /* ����3���ֽڵĵ�ַ��Ϣ */
		Flash_ReadWriteByte((((Dst+i) & 0xFFFF) >> 8));
		Flash_ReadWriteByte((Dst+i) & 0xFF);
		Flash_ReadWriteByte(SndbufPt[i]);					/* ���ͱ���д������	*/
		SPI_FLASH_CS_HIGH();			

		do
		{
		  	SPI_FLASH_CS_LOW();					 
			Flash_ReadWriteByte(0x05);					    /* ���Ͷ�״̬�Ĵ������� */
			StatRgVal = Flash_ReadWriteByte(0xFF);			/* ������õ�״̬�Ĵ���ֵ */
			SPI_FLASH_CS_HIGH();							
  		}
		while (StatRgVal == 0x03 );					          /* һֱ�ȴ���ֱ��оƬ����	*/

	}

	SPI_FLASH_CS_LOW();					
	Flash_ReadWriteByte(0x06);							    /* ����дʹ������ */
	SPI_FLASH_CS_HIGH();			

	SPI_FLASH_CS_LOW();					
	Flash_ReadWriteByte(0x50);							    /* ʹ״̬�Ĵ�����д	*/
	SPI_FLASH_CS_HIGH();
			
	SPI_FLASH_CS_LOW();				
	Flash_ReadWriteByte(0x01);							    /* ����д״̬�Ĵ���ָ�� */
	Flash_ReadWriteByte(temp);						     	/* �ָ�״̬�Ĵ���������Ϣ */
	SPI_FLASH_CS_HIGH();

	return (ENABLE);		
}


uint8_t SSTF016B_Erase(uint32_t sec1, uint32_t sec2)
{
	uint8_t  temp1 = 0,temp2 = 0,StatRgVal = 0;
    uint32_t SecnHdAddr = 0;	  			
	uint32_t no_SecsToEr = 0;				   			    /* Ҫ������������Ŀ */
	uint32_t CurSecToEr = 0;	  						    /* ��ǰҪ������������ */
	
	/*  �����ڲ��� */
	if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))	
	{
		return (ERROR);	
	}
   	
   	SPI_FLASH_CS_LOW();			 
	Flash_ReadWriteByte(0x05);								/* ���Ͷ�״̬�Ĵ�������	*/
	temp1 = Flash_ReadWriteByte(0x00);						/* ������õ�״̬�Ĵ���ֵ */
	SPI_FLASH_CS_HIGH();								

	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x50);								/* ʹ״̬�Ĵ�����д	*/
	SPI_FLASH_CS_HIGH();			

	SPI_FLASH_CS_LOW();								  	
	Flash_ReadWriteByte(0x01);								/* ����д״̬�Ĵ���ָ��	*/
	Flash_ReadWriteByte(0);									/* ��0BPxλ��ʹFlashоƬȫ����д */
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x06);								/* ����дʹ������ */
	SPI_FLASH_CS_HIGH();			

	/* ����û��������ʼ�����Ŵ�����ֹ�����ţ������ڲ��������� */
	if (sec1 > sec2)
	{
	   temp2 = sec1;
	   sec1  = sec2;
	   sec2  = temp2;
	} 
	/* ����ֹ���������������������� */
	if (sec1 == sec2)	
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* ����дʹ������ */
		SPI_FLASH_CS_HIGH();			

	    SecnHdAddr = SEC_SIZE * sec1;				          /* ������������ʼ��ַ	*/
	    SPI_FLASH_CS_LOW();	
    	Flash_ReadWriteByte(0x20);							  /* ������������ָ�� */
	    Flash_ReadWriteByte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* ����3���ֽڵĵ�ַ��Ϣ */
   		Flash_ReadWriteByte(((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_ReadWriteByte(SecnHdAddr & 0xFF);
  		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* ���Ͷ�״̬�Ĵ������� */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* ������õ�״̬�Ĵ���ֵ	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);				              /* һֱ�ȴ���ֱ��оƬ����	*/
		return (ENABLE);			
	}
	
    /* ������ʼ��������ֹ��������������ٵĲ������� */	
	
	if (sec2 - sec1 == SEC_MAX)	
	{
		SPI_FLASH_CS_LOW();			
		Flash_ReadWriteByte(0x60);							  /* ����оƬ����ָ��(60h or C7h) */
		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* ���Ͷ�״̬�Ĵ������� */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* ������õ�״̬�Ĵ���ֵ	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);					          /* һֱ�ȴ���ֱ��оƬ����	*/
   		return (ENABLE);
	}
	
	no_SecsToEr = sec2 - sec1 +1;					          /* ��ȡҪ������������Ŀ */
	CurSecToEr  = sec1;								          /* ����ʼ������ʼ����	*/
	
	/* ����������֮��ļ���������ȡ8���������㷨 */
	while (no_SecsToEr >= 8)
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* ����дʹ������ */
		SPI_FLASH_CS_HIGH();			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			          /* ������������ʼ��ַ */
	    SPI_FLASH_CS_LOW();	
	    Flash_ReadWriteByte(0x52);							  /* ����32KB����ָ�� */
	    Flash_ReadWriteByte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* ����3���ֽڵĵ�ַ��Ϣ */
   		Flash_ReadWriteByte(((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_ReadWriteByte(SecnHdAddr & 0xFF);
  		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* ���Ͷ�״̬�Ĵ������� */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* ������õ�״̬�Ĵ���ֵ	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);					          /* һֱ�ȴ���ֱ��оƬ����	*/
		CurSecToEr  += 8;
		no_SecsToEr -=  8;
	}
	/* �������������㷨����ʣ������� */
	while (no_SecsToEr >= 1)
	{
		SPI_FLASH_CS_LOW();				
		Flash_ReadWriteByte(0x06);						    /* ����дʹ������ */
		SPI_FLASH_CS_HIGH();			

	    SecnHdAddr = SEC_SIZE * CurSecToEr;			          /* ������������ʼ��ַ */
	    SPI_FLASH_CS_LOW();	
    	Flash_ReadWriteByte(0x20);							  /* ������������ָ�� */
	    Flash_ReadWriteByte(((SecnHdAddr & 0xFFFFFF) >> 16)); /* ����3���ֽڵĵ�ַ��Ϣ */
   		Flash_ReadWriteByte(((SecnHdAddr & 0xFFFF) >> 8));
   		Flash_ReadWriteByte(SecnHdAddr & 0xFF);
  		SPI_FLASH_CS_HIGH();			
		do
		{
		  	SPI_FLASH_CS_LOW();			 
			Flash_ReadWriteByte(0x05);						  /* ���Ͷ�״̬�Ĵ������� */
			StatRgVal = Flash_ReadWriteByte(0x00);			  /* ������õ�״̬�Ĵ���ֵ	*/
			SPI_FLASH_CS_HIGH();								
  		}
		while (StatRgVal == 0x03);					          /* һֱ�ȴ���ֱ��оƬ���� */
		CurSecToEr  += 1;
		no_SecsToEr -=  1;
	}
    /* ��������,�ָ�״̬�Ĵ�����Ϣ */
	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x06);								  /* ����дʹ������ */
	SPI_FLASH_CS_HIGH();			

	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x50);								  /* ʹ״̬�Ĵ�����д */
	SPI_FLASH_CS_HIGH();			
	SPI_FLASH_CS_LOW();			
	Flash_ReadWriteByte(0x01);								  /* ����д״̬�Ĵ���ָ�� */
	Flash_ReadWriteByte(temp1);								  /* �ָ�״̬�Ĵ���������Ϣ */
	SPI_FLASH_CS_HIGH();    
	return (ENABLE);
}


void SPI_FLASH_Test(void)
{
    uint32_t  ChipID = 0;
  	SSTF016B_RdID(Jedec_ID, &ChipID);                                 


	if (ChipID != 0xBF2541) {											/*  ID����ȷ������ѭ��          */
           while(1);
    }

}
void df_read_open(uint32_t addr)
{
		CurReadAddr=addr;	/* ��ǰ���ĵ�ַ	*/
}
void df_write_open(uint32_t addr)
{
		CurWriteAddr=addr;	/* ��ǰд��ַ */
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
