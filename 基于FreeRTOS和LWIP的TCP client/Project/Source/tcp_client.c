
#include "stm32_eth.h"
#include "lwip/tcp.h"
#include "TCP_CLIENT.h"

struct tcp_pcb *tcp_client_pcb;
unsigned char connect_flag = 0;

void Delay_s(unsigned long ulVal) /* 利用循环产生一定的延时 */
{
	while ( --ulVal != 0 );
}


err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length)
{
	err_t err;

	err = tcp_write(cpcb,buff,length,TCP_WRITE_FLAG_COPY);	//发送数据
	tcp_output(cpcb);
	//tcp_close(cpcb);				//发送完数据关闭连接,根据具体情况选择使用	
	return err;					
}

struct tcp_pcb *Check_TCP_Connect(void)
{
	struct tcp_pcb *cpcb = 0;
	connect_flag = 0;
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
	//	if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == TCP_SERVER_PORT)		//如果TCP_LOCAL_PORT端口指定的连接没有断开
		if(cpcb -> state == ESTABLISHED) 
		{
			connect_flag = 1;  						//连接标志
			break;							   	
		}
	}

	if(connect_flag == 0)  							// TCP_LOCAL_PORT指定的端口未连接或已断开
	{
		cpcb = 0;
	}	
	if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == TCP_SERVER_PORT)		//如果TCP_LOCAL_PORT端口指定的连接没有断开
	{
		cpcb=0;
	}
	return cpcb;	
}

err_t TCP_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	uint8_t txdata[]="192.168.31.66 connected\n";
	TCP_Client_Send_Data(pcb,txdata,sizeof(txdata));
	return ERR_OK;
}

err_t  TCP_Client_Recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{

	if(p != NULL)
	{
		tcp_recved(pcb, p->tot_len);				//获取数据长度 tot_len：tcp数据块的长度
		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY);
		tcp_output(pcb);
		pbuf_free(p); 							    
	}
	else	 										//如果服务器断开连接，则客户端也应断开
	{
		tcp_close(pcb); 						
	}
	err = ERR_OK;
	return err;	
}

void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{

	struct ip4_addr ipaddr;
	err_t err;
	IP4_ADDR(&ipaddr,a,b,c,d);           //服务器IP地址
	tcp_client_pcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	if (!tcp_client_pcb)
	{
		return ;
	}	
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port); /* 绑定本地IP地址和端口号 ，本地ip地址在LwIP_Init()中已经初始化*/
    if(err != ERR_OK)
	{
		//tcp_close(tcp_client_pcb);
		return ;
	}
	tcp_connect(tcp_client_pcb,&ipaddr,remote_port,TCP_Connected);//注册回调函数
	tcp_recv(tcp_client_pcb,TCP_Client_Recv); 				/* 设置tcp接收回调函数 */
}


