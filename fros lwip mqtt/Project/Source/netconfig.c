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
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/dhcp.h"
struct netif netif;
xTaskHandle dhcp_tsk_handle;

void LWIP_INIT(){
	struct ip4_addr ipaddr,netmask,gw;
	mem_init();
	memp_init();
	
	
   #if LWIP_DHCP 
		ipaddr.addr=0;
	  netmask.addr=0;
	  gw.addr=0;
		#else
	 IP4_ADDR(&ipaddr, 192,168,31,12);
   IP4_ADDR(&netmask, 255, 255, 255, 0);
   IP4_ADDR(&gw, 192, 168, 31, 1);
		#endif
    tcpip_init(NULL,NULL);

	  netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

   netif_set_default(&netif);
	
	#if LWIP_DHCP
	  dhcp_start(&netif);
		#endif


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
//#if LWIP_DHCP

//void create_dhcp_task(void){
//	
//	xTaskCreate((TaskFunction_t)dhcp_task,
//							(const char*)		"start_dhcp",
//							(uint16_t)			50,
//							(void*)					NULL,
//							(UBaseType_t)   5,
//							(TaskHandle_t*) &dhcp_tsk_handle);
//	
//}
//void dhcp_task(void){
//	uint32_t ip,gw,netmask;
//	dhcp_start(&netif);
//	while(1){
//		ip=netif.ip_addr.addr;
//		netmask=netif.netmask.addr;
//		gw=netif.gw.addr;
//		if(ip!=0){
//			printf("dhcp success, ip = %d,%d,%d,%d\n",(uint8_t)(ip>>24),(uint8_t)(ip>>16),(uint8_t)(ip>>8),(uint8_t)ip);
//		}else{
//			printf("dhcp fail, retrying dhcp\n");
//		}
//		vTaskDelay(500);
//	}
//}
//#endif
