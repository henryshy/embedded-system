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
	struct ip4_addr ipaddr,netmask,gw;//ip地址
	mem_init();
	memp_init();
	IP4_ADDR(&ipaddr, 192, 168, 1, 2);//设置ipv4地址
  IP4_ADDR(&netmask, 255, 255, 255, 0);//设置子网掩码
  IP4_ADDR(&gw, 192, 168, 1, 1);//设置默认网关
	
	
	netif_add(&nt, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);//将netif添加到网络接口列表中

  netif_set_default(&nt);//设为默认（暂时不是很懂默认是啥
	
  netif_set_up(&nt);//初始化完后必须调用（应该是类似开启功能的意思？
}