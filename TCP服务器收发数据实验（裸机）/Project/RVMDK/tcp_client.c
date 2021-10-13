#include "tcp_client.h"
#include "stm32_eth.h"
#include "netconf.h"
#include "main.h"
#include "helloworld.h"
#include "httpd.h"
#include "tftpserver.h"
#include "lwip/tcp.h"

unsigned char tcp_rec_flag = 0;
extern unsigned char rec_buf[1];
unsigned char TCP_TestData[]="TCP TEST!\r\n";

void Delay_s(unsigned long ulVal) /* ����ѭ������һ������ʱ */
{
	while ( --ulVal != 0 );
}


//***********����Ϊtcp���Գ���*************//
/******* ����һ���ص���������TCP�ͻ�����������ӽ���ʱ������********/
err_t TcpCli_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_write(pcb,rec_buf,sizeof(rec_buf),0); /* �������� */
	tcp_close(pcb);
	return ERR_OK;
}
void TCP_Client_Init_1(void)
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	struct ip_addr ipaddr_1;
	IP4_ADDR(&ipaddr,192,168,1,2);
	Clipcb = tcp_new(); /* ����ͨ�ŵ�TCP���ƿ�(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,1025); /* �󶨱���IP��ַ�Ͷ˿ں� */
	tcp_connect(Clipcb,&ipaddr,1027,TcpCli_Connected);	
}

void TCP_Client_Init_2()
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,192,168,1,2);
	Clipcb = tcp_new(); /* ����ͨ�ŵ�TCP���ƿ�(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,1028); /* �󶨱���IP��ַ�Ͷ˿ں� */
	tcp_connect(Clipcb,&ipaddr,1026,TcpCli_Connected);
}