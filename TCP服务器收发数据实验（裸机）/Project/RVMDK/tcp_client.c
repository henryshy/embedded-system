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

void Delay_s(unsigned long ulVal) /* 利用循环产生一定的延时 */
{
	while ( --ulVal != 0 );
}


//***********以下为tcp测试程序*************//
/******* 这是一个回调函数，当TCP客户端请求的连接建立时被调用********/
err_t TcpCli_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_write(pcb,rec_buf,sizeof(rec_buf),0); /* 发送数据 */
	tcp_close(pcb);
	return ERR_OK;
}
void TCP_Client_Init_1(void)
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	struct ip_addr ipaddr_1;
	IP4_ADDR(&ipaddr,192,168,1,2);
	Clipcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,1025); /* 绑定本地IP地址和端口号 */
	tcp_connect(Clipcb,&ipaddr,1027,TcpCli_Connected);	
}

void TCP_Client_Init_2()
{
	struct tcp_pcb *Clipcb;
	struct ip_addr ipaddr;
	IP4_ADDR(&ipaddr,192,168,1,2);
	Clipcb = tcp_new(); /* 建立通信的TCP控制块(Clipcb) */
	tcp_bind(Clipcb,IP_ADDR_ANY,1028); /* 绑定本地IP地址和端口号 */
	tcp_connect(Clipcb,&ipaddr,1026,TcpCli_Connected);
}