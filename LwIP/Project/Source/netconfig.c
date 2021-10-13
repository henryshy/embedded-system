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
	struct ip_addr ipaddr;//ip地址
	struct ip_addr netmask;//子网掩码
	struct ip_addr wangguan;//默认网关
	
	u8 macaddr[6]={0,0,0,0,0,1};//设置mac地址
	
	mem_init();
	memp_init();//初始化内存分配
	IP4_ADDR(&ipaddr, 192, 168, 1, 252);//设置ipv4地址
  IP4_ADDR(&netmask, 255, 255, 255, 0);//设置子网掩码
  IP4_ADDR(&wangguan, 192, 168, 1, 1);//设置默认网关
	
	Set_MAC_Address(macaddr);//将mac地址与netif绑定
	
	netif_add(&netif, &ipaddr, &netmask, &wangguan, NULL, &ethernetif_init, &ethernet_input);//将netif添加到网络接口列表中

  netif_set_default(&netif);//设为默认（暂时不是很懂默认是啥
	
  netif_set_up(&netif);//初始化完后必须调用（应该是类似开启功能的意思？
}