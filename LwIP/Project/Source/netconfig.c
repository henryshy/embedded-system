#include "ethernetif.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "netconfig.h"
#include "ethernetif.h"
#include <stdio.h>
#include "main.h"

struct netif netif;

void LWIP_INIT(){
	struct ip_addr ipaddr;//ip��ַ
	struct ip_addr netmask;//��������
	struct ip_addr wangguan;//Ĭ������
	
	u8 macaddr[6]={0,0,0,0,0,1};//����mac��ַ
	
	mem_init();
	memp_init();//��ʼ���ڴ����
	IP4_ADDR(&ipaddr, 192, 168, 1, 252);//����ipv4��ַ
  IP4_ADDR(&netmask, 255, 255, 255, 0);//������������
  IP4_ADDR(&wangguan, 192, 168, 1, 1);//����Ĭ������
	
	Set_MAC_Address(macaddr);//��mac��ַ��netif��
	
	netif_add(&netif, &ipaddr, &netmask, &wangguan, NULL, &ethernetif_init, &ethernet_input);//��netif��ӵ�����ӿ��б���

  netif_set_default(&netif);//��ΪĬ�ϣ���ʱ���Ǻܶ�Ĭ����ɶ
	
  netif_set_up(&netif);//��ʼ����������ã�Ӧ�������ƿ������ܵ���˼��
}