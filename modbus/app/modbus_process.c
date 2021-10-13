#include "modbus.h"

uint16_t crc_check(uint8_t Rx[],uint8_t length){//crc校验
	
    uint8_t len = length - 2;
    uint16_t crc_result = 0xffff;
    int crc_num =0;
    int xor_flag=0;
	  int i;
	  int m;
    for( i=0;i<len;i++)
    {
        crc_result ^= Rx[i];
        crc_num = (crc_result&0x0001);
			
        for(m=0;m<8;m++)
        {
            if(crc_num==1)
                xor_flag = 1;
            else
                xor_flag = 0;
            crc_result >>= 1;
            if(xor_flag)
                crc_result ^= 0xa001;
            crc_num = (crc_result & 0x0001);
        }
    }
    return crc_result;
}


void modbus(uint8_t Rx[],uint8_t length){
	 uint8_t Rx_addr=Rx[0]; //读取设备地址
	 uint8_t Rx_func=Rx[1]; //读取功能码
   uint16_t Rx_crc = Rx[length-2]<<8 | Rx[length-1]; //读取crc码
	 uint16_t crc=crc_check(Rx,length); //获得crc校验码
	 uint16_t out_amt=Rx[4]<<8|Rx[5];

		 if(Rx_crc==crc && Rx_addr==local_addr){	  //检验设备地址, crc校验
		  rx_rec_cnt++;
		  switch(Rx_func){ //根据功能码进行操作
				
			 case 0x01:  //读取离散输出开关量
				  modbus_0x01(Rx,length);
            break;
			 case 0x02: //读取离散输入开关量
				  modbus_0x02(Rx,length);
				    break;
			 case 0x03://读取保持寄存器
				  modbus_0x03(Rx,length); 
				   break;
			 case 0x04: //读取输出寄存器
				   modbus_0x04(Rx,length);
			     break;
			 case 0x05: //写单个输出开关量
				   modbus_0x05(Rx,length);
			     break;
			 case 0x06: //写单个输出寄存器
				   modbus_0x06(Rx,length);
			     break;
			 case 0x15: //写多个输出开关量
				   modbus_0x0f(Rx,length);
			     break;
			 default: 
				   func_error(Rx,length);
		   }	 
	   }
		 else{
			 clear_rec();
		 }
	 
	 tx_flag=1;
}
void modbus_0x01(uint8_t Rx[],uint8_t length){ //读取离散输出开关量
	 uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_data_amt=Rx[4];  //数据数量高8位
	 uint8_t lo_data_amt=Rx[5];  //数据数量低8位
	
	 uint16_t data_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr; 
   uint16_t data_amt=((uint16_t)hi_data_amt<<8)|lo_data_amt;  //读取的数据位数
	
	 uint16_t out_char_amt=data_amt/8; //输出字节数
	
    if(data_amt%8!=0) out_char_amt+=1;
	 
	 
    if((data_addr+data_amt/32)<100 && (data_addr>=0 && data_addr <100)){//长度范围判断 地址范围判断
		 tx_rec[0]=Rx[0];
		 tx_rec[1]=Rx[1];
		 tx_rec[2]=out_char_amt;
		 tx_rec_length=3+out_char_amt;
		 uint32_t i;
		for(i=3;i<3+out_char_amt;i++){
			tx_rec[i]=(*out_reg[data_addr+(i-3)/4])>>((i-3)*8)&0xff;
		}
	 }
	 else{
		 addr_amt_error(Rx,length);
	 }
}
void modbus_0x02(uint8_t Rx[],uint8_t length){ //读取离散输入开关量
   uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_data_amt=Rx[4];  //数据数量高8位
	 uint8_t lo_data_amt=Rx[5];  //数据数量低8位
	
	 uint16_t data_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr; 
   uint16_t data_amt=((uint16_t)hi_data_amt<<8)|lo_data_amt;  //读取的数据位数
	
	 uint16_t out_char_amt=data_amt/8; //输出字节数
	
    if(data_amt%8!=0) out_char_amt+=1;
	 
    if((data_addr+data_amt/32)<100 && (data_addr>=0 && data_addr <100)){//长度范围判断 地址范围判断
		  tx_rec[0]=Rx[0];
		  tx_rec[1]=Rx[1];
		  tx_rec[2]=out_char_amt;
		  tx_rec_length=3+out_char_amt;
		  uint32_t i;
		 for(i=3;i<3+out_char_amt;i++){
			 tx_rec[i]=(*in_reg[data_addr+(i-3)/4])>>((i-3)*8)&0xff;
	  	}
	   }
	  else{
		  addr_amt_error(Rx,length);
	  }
}
void modbus_0x03(uint8_t Rx[],uint8_t length){ //读取多个保持寄存器
	 uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_data_amt=Rx[4];  //数据数量高8位
	 uint8_t lo_data_amt=Rx[5];  //数据数量低8位
	
	 uint16_t rec_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr; 
   uint16_t rec_amt=((uint16_t)hi_data_amt<<8)|lo_data_amt;  //读取的寄存器个数
	 
    if((rec_addr+rec_amt)<100 && (rec_addr>=0 && rec_addr <100)){//长度范围判断 地址范围判断
		  tx_rec[0]=Rx[0];
		  tx_rec[1]=Rx[1];
		  tx_rec[2]=rec_amt*4;
		  tx_rec_length=3+rec_amt*4;
		  uint32_t i;
		  for(i=3;i<3+rec_amt*4;i++){
        tx_rec[i]=(*hold_reg[rec_addr+(i-3)/4])>>(((i-3)%4)*8)&0xff;
			}
	   }
	  else{
		  addr_amt_error(Rx,length);
	  }
	
}
void modbus_0x04(uint8_t Rx[],uint8_t length){//读取多个输出寄存器(协议规定为输入寄存器 这里方便调试做点小修改)
   uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_data_amt=Rx[4];  //数据数量高8位
	 uint8_t lo_data_amt=Rx[5];  //数据数量低8位
	
	 uint16_t rec_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr; 
   uint16_t rec_amt=((uint16_t)hi_data_amt<<8)|lo_data_amt;  //读取的寄存器个数
	 
    if((rec_addr+rec_amt)<100 && (rec_addr>=0 && rec_addr <100)){//长度范围判断 地址范围判断
		  tx_rec[0]=Rx[0];
		  tx_rec[1]=Rx[1];
		  tx_rec[2]=rec_amt*4;
		  tx_rec_length=3+rec_amt*4;
		  uint32_t i;
		  for(i=3;i<3+rec_amt*4;i++){
        tx_rec[i]=(*out_reg[rec_addr+(i-3)/4])>>(((i-3)%4)*8)&0xff;
			}
	   }
	  else{
		  addr_amt_error(Rx,length);
	  }
	
}
void modbus_0x05(uint8_t Rx[],uint8_t length){//写单个输出开关量
	uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_sw_flag=Rx[4];  //开关信号高8位
	 uint8_t lo_sw_flag=Rx[5];  //开关信号低8位
	 
	uint16_t sw_flag=((uint16_t)hi_sw_flag<<8)|lo_sw_flag;
	uint16_t rec_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr;
	
	 if(sw_flag==0x0000 || sw_flag==0xff00){
		 if(rec_addr>=0 && rec_addr<100){
		 switch(sw_flag){
			 case 0x0000:   //写单个输出开关量为开
				 tx_rec[0]=Rx[0];
			   tx_rec[1]=Rx[1];
			   tx_rec[2]=Rx[2];
			   tx_rec[3]=Rx[3];
			   tx_rec[4]=Rx[4];
			   tx_rec[5]=Rx[5];
			   tx_rec_length=6;
			   *out_reg[rec_addr]=0x00;
				 break;
			 case 0xff00:  //写单个输出开关量为开
				 tx_rec[0]=Rx[0];
			   tx_rec[1]=Rx[1];
			   tx_rec[2]=Rx[2];
			   tx_rec[3]=Rx[3];
			   tx_rec[4]=Rx[4];
			   tx_rec[5]=Rx[5];
			   tx_rec_length=6;
			   *out_reg[rec_addr]=0x01;
				 break;
		 }
	  }
		else{
			 addr_amt_error(Rx,length);
		}
	 }
   else{
		 sw_flag_error(Rx,length); 
	 }
}

void modbus_0x06(uint8_t Rx[],uint8_t length){//写单个输出寄存器 
	 uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint16_t hi_write_value=((uint16_t)Rx[4]<<8)|Rx[5];  //开关信号高16位
	 uint16_t lo_write_value=((uint16_t)Rx[6]<<8)|Rx[7];  //开关信号低16位
	 
	uint32_t write_value=((uint32_t)hi_write_value<<16)|lo_write_value;
	uint16_t rec_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr;
	
	 if(write_value<=0xffffffff && write_value>=0){
		 if(rec_addr>=0 && rec_addr<100){
		   *out_reg[rec_addr]=write_value;
			  tx_rec[0]=Rx[0];
			   tx_rec[1]=Rx[1];
			   tx_rec[2]=Rx[2];
			   tx_rec[3]=Rx[3];
			   tx_rec[4]=Rx[4];
			   tx_rec[5]=Rx[5];
	  }
		else{
			 addr_amt_error(Rx,length);
		}
	 }
   else{
		 value_error(Rx,length);
	 }
}
void modbus_0x0f(uint8_t Rx[],uint8_t length){ //写多个输出开关量(协议规定写多个线圈)
	uint8_t hi_data_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_data_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_sw_amt=Rx[4];  //开关数量高8位
	 uint8_t lo_sw_amt=Rx[5];  //开关数量低8位
	uint8_t char_amt =Rx[6];  //操作线圈字节数
	 
	uint16_t sw_amt=((uint16_t)hi_sw_amt<<8)|lo_sw_amt;
	uint16_t sw_addr=((uint16_t)hi_data_addr<<8)|lo_data_addr;
	uint16_t rec_addr=sw_addr/32+1;
	
		 if(rec_addr>=0 && rec_addr<100 &&(rec_addr+sw_amt<100)){
		   int i;
			 for(i=0;i<char_amt;i++){
				 *out_reg[rec_addr+i/4]|=Rx[7+i]<<(8*(i%4));				 
			 }
			 tx_rec[0]=Rx[0];
			 tx_rec[1]=Rx[1];
			 tx_rec[2]=Rx[2];
			 tx_rec[3]=Rx[3];
			 tx_rec[4]=Rx[4];
			 tx_rec[5]=Rx[5];
			 tx_rec_length=6;
		 }
		else{
			 addr_amt_error(Rx,length);
		}

} 
void modbus_0x10(uint8_t Rx[],uint8_t length){ //写多个输出寄存器
	 uint8_t hi_rec_addr=Rx[2]; //起始数据地址高8位
	 uint8_t lo_rec_addr=Rx[3]; //起始数据地址低8位
	 uint8_t hi_rec_amt=Rx[4];  //寄存器数量高8位
	 uint8_t lo_rec_amt=Rx[5];  //寄存器数量低8位
	uint8_t char_amt =Rx[6];  //操作寄存器的字节数
	 
	uint16_t rec_amt=((uint16_t)hi_rec_amt<<8)|lo_rec_amt;
	uint16_t rec_addr=((uint16_t)hi_rec_addr<<8)|lo_rec_addr;
	
		 if(rec_addr>=0 && rec_addr<100 &&(rec_addr+rec_amt<100) && char_amt*4==rec_amt){
		   int i;
			 for(i=0;i<char_amt;i++){
				 *out_reg[rec_addr+i/4]|=Rx[7+i]<<(8*(i%4));				 
			 }
			 tx_rec[0]=Rx[0];
			 tx_rec[1]=Rx[1];
			 tx_rec[2]=Rx[2];
			 tx_rec[3]=Rx[3];
			 tx_rec[4]=Rx[4];
			 tx_rec[5]=Rx[5];
			 tx_rec_length=6;
		 }
		else{
			 addr_amt_error(Rx,length);
		}

}
	
void func_error(uint8_t Rx[],uint8_t length){ //功能码错误
     tx_rec[0]=Rx[0];
	   tx_rec[1]=Rx[1]+0x80;
	   tx_rec[2]=0x01;
	   tx_rec[3]=Rx[length-2];
     tx_rec[4]=Rx[length-1];
	   tx_rec_length=5;
}
void amt_error(uint8_t Rx[],uint8_t length){ //数量错误
	   tx_rec[0]=Rx[0];
	   tx_rec[1]=Rx[1]+0x80;
	   tx_rec[2]=0x03;
		 tx_rec[3]=Rx[length-2];
     tx_rec[4]=Rx[length-1];
	   tx_rec_length=5;
}
void sw_flag_error(uint8_t Rx[],uint8_t length){ //开关信号错误
	   tx_rec[0]=Rx[0];
	   tx_rec[1]=Rx[1]+0x80;
	   tx_rec[2]=0x03;
		 tx_rec[3]=Rx[length-2];
     tx_rec[4]=Rx[length-1];
	   tx_rec_length=5;
}
void addr_amt_error(uint8_t Rx[],uint8_t length){ //地址或数量错误
	   tx_rec[0]=Rx[0];
	   tx_rec[1]=Rx[1]+0x80;
	   tx_rec[2]=0x02;
	   tx_rec[3]=Rx[length-2];
     tx_rec[4]=Rx[length-1];
	   tx_rec_length=5;
}
void value_error(uint8_t Rx[],uint8_t length){  //值错误
	   tx_rec[0]=Rx[0];
	   tx_rec[1]=Rx[1]+0x80;
	   tx_rec[2]=0x03;
		 tx_rec[3]=Rx[length-2];
     tx_rec[4]=Rx[length-1];
	   tx_rec_length=5;
}

	



