 /***********************************************************************
�ļ����ƣ�TCP_CLIENT.H
��    �ܣ����TCP�������շ�
��дʱ�䣺2013.4.25
�� д �ˣ���
ע    �⣺
***********************************************************************/
#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_



#define TCP_LOCAL_PORT     		80
#define TCP_SERVER_PORT    		80

#define TCP_SERVER_IP   192,168,31,115////������IP
 #define MQTT_SERVER_IP TCP_SERVER_IP

extern struct tcp_pcb *tcp_client_pcb;
extern unsigned char connect_flag;
void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length);
struct tcp_pcb *Check_TCP_Connect(void);
#endif

