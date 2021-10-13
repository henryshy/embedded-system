#include "ethernetif.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "netconfig.h"
#include "ethernetif.h"
#include <stdio.h>
#include "main.h"
#include "netif.h"
#include "lwip/ip_addr.h"
#include "tcpip.h"
#include "TCP_CLIENT.h"
struct netif netif;

void LWIP_INIT(){
	struct ip4_addr ipaddr,netmask,gw;
	mem_init();
	memp_init();
	memp_init();
	memp_init();
	memp_init();
	memp_init();
	IP4_ADDR(&ipaddr, 192,168,1,12);
  IP4_ADDR(&netmask, 255, 255, 255, 0);
  IP4_ADDR(&gw, 192, 168, 1, 1);
	 tcpip_init(NULL, NULL);
	netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  netif_set_default(&netif);
	
   if (netif_is_link_up(&netif))
    {
			//使能网络接口
//			 printf("connect success!");
        netif_set_up(&netif);
     }
     else
		{
         /* When the netif link is down this function must be called */
//			printf("failed!");
      netif_set_down(&netif);
    }
}
