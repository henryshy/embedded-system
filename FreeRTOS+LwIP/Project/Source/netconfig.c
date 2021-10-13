#include "ethernetif.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "netconfig.h"
#include "ethernetif.h"
#include <stdio.h>
#include "main.h"
#include "netif.h"
#include "lwip/ip_addr.h"

struct netif nt;

void LWIP_INIT(){
	struct ip4_addr ipaddr,netmask,gw;//ip��ַ
	mem_init();
	memp_init();
	IP4_ADDR(&ipaddr, 192, 168, 1, 2);//����ipv4��ַ
  IP4_ADDR(&netmask, 255, 255, 255, 0);//������������
  IP4_ADDR(&gw, 192, 168, 1, 1);//����Ĭ������
	
	
	netif_add(&nt, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);//��netif��ӵ�����ӿ��б���

  netif_set_default(&nt);//��ΪĬ�ϣ���ʱ���Ǻܶ�Ĭ����ɶ
	
  netif_set_up(&nt);//��ʼ����������ã�Ӧ�������ƿ������ܵ���˼��
}