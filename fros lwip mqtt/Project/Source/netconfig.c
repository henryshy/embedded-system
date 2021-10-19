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
#include "mqtt_client.h"
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
	 IP4_ADDR(&ipaddr, 192,168,31,18);
   IP4_ADDR(&netmask, 255, 255, 255, 0);
   IP4_ADDR(&gw, 192, 168, 31, 1);
		#endif
    tcpip_init(NULL,NULL);
	  netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
   netif_set_default(&netif);
		 if (netif_is_link_up(&netif))
    {
        netif_set_up(&netif);
     }
     else
		{
      netif_set_down(&netif);
    }

		
}
#if LWIP_DHCP

void create_dhcp_task(void){
	
	xTaskCreate((TaskFunction_t)dhcp_task,
							(const char*)		"start_dhcp",
							(uint16_t)			300,
							(void*)					NULL,
							(UBaseType_t)   5,
							(TaskHandle_t*) &dhcp_tsk_handle);
	
}
void dhcp_task(void){
	uint32_t ip,gw,netmask;
	dhcp_start(&netif);
	while(1){
		ip=netif.ip_addr.addr;
		netmask=netif.netmask.addr;
		gw=netif.gw.addr;
		if(ip!=0){
			printf("dhcp success, ip = %d,%d,%d,%d\n",(uint8_t)ip,(uint8_t)(ip>>8),(uint8_t)(ip>>16),(uint8_t)ip);
			printf("dhcp success, netmask = %d,%d,%d,%d\n",(uint8_t)netmask,(uint8_t)(netmask>>8),(uint8_t)(netmask>>16),(uint8_t)netmask);
			printf("dhcp success, gw = %d,%d,%d,%d\n",(uint8_t)gw,(uint8_t)(gw>>8),(uint8_t)(gw>>16),(uint8_t)gw);
			TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP); 
			start_mqtt(MQTT_SERVER_IP);
			vTaskDelete(dhcp_tsk_handle);
		}else{
			printf("dhcp fail, retrying dhcp\n");
		}
		vTaskDelay(1000);
	}
}
#endif
