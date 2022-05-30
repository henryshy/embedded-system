
#include "stm32_eth.h"
#include "lwip/tcp.h"
#include "TCP_CLIENT.h"

struct tcp_pcb *tcp_client_pcb;
unsigned char connect_flag = 0;

void Delay_s(unsigned long ulVal) /* ����ѭ������һ������ʱ */
{
	while ( --ulVal != 0 );
}


err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length)
{
	err_t err;

	err = tcp_write(cpcb,buff,length,TCP_WRITE_FLAG_COPY);	//��������
	tcp_output(cpcb);
	//tcp_close(cpcb);				//���������ݹر�����,���ݾ������ѡ��ʹ��	
	return err;					
}

struct tcp_pcb *Check_TCP_Connect(void)
{
	struct tcp_pcb *cpcb = 0;
	connect_flag = 0;
	for(cpcb = tcp_active_pcbs;cpcb != NULL; cpcb = cpcb->next)
	{
	//	if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == TCP_SERVER_PORT)		//���TCP_LOCAL_PORT�˿�ָ��������û�жϿ�
		if(cpcb -> state == ESTABLISHED) 
		{
			connect_flag = 1;  						//���ӱ�־
			break;							   	
		}
	}

	if(connect_flag == 0)  							// TCP_LOCAL_PORTָ���Ķ˿�δ���ӻ��ѶϿ�
	{
		cpcb = 0;
	}	
	if(cpcb->local_port == TCP_LOCAL_PORT && cpcb->remote_port == TCP_SERVER_PORT)		//���TCP_LOCAL_PORT�˿�ָ��������û�жϿ�
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
		tcp_recved(pcb, p->tot_len);				//��ȡ���ݳ��� tot_len��tcp���ݿ�ĳ���
		tcp_write(pcb,p->payload,p->tot_len,TCP_WRITE_FLAG_COPY);
		tcp_output(pcb);
		pbuf_free(p); 							    
	}
	else	 										//����������Ͽ����ӣ���ͻ���ҲӦ�Ͽ�
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
	IP4_ADDR(&ipaddr,a,b,c,d);           //������IP��ַ
	tcp_client_pcb = tcp_new(); /* ����ͨ�ŵ�TCP���ƿ�(Clipcb) */
	if (!tcp_client_pcb)
	{
		return ;
	}	
	err = tcp_bind(tcp_client_pcb,IP_ADDR_ANY,local_port); /* �󶨱���IP��ַ�Ͷ˿ں� ������ip��ַ��LwIP_Init()���Ѿ���ʼ��*/
    if(err != ERR_OK)
	{
		//tcp_close(tcp_client_pcb);
		return ;
	}
	tcp_connect(tcp_client_pcb,&ipaddr,remote_port,TCP_Connected);//ע��ص�����
	tcp_recv(tcp_client_pcb,TCP_Client_Recv); 				/* ����tcp���ջص����� */
}


