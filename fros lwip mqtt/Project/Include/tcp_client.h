 /***********************************************************************
文件名称：TCP_CLIENT.H
功    能：完成TCP的数据收发
编写时间：2013.4.25
编 写 人：赵
注    意：
***********************************************************************/
#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_



#define TCP_LOCAL_PORT     		80
#define TCP_SERVER_PORT    		80

#define TCP_SERVER_IP   192,168,31,115////服务器IP
 #define MQTT_SERVER_IP TCP_SERVER_IP

extern struct tcp_pcb *tcp_client_pcb;
extern unsigned char connect_flag;
void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length);
struct tcp_pcb *Check_TCP_Connect(void);
#endif

