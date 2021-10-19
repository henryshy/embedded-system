#include "lwip/ip_addr.h"
#include "lwip/mem.h"
#include "lwip/err.h"
#include "mqtt_client.h"
#include "string.h"
#include "TCP_CLIENT.h"
#include "main.h"
mqtt_client_t static_client;

void start_mqtt(unsigned char a,unsigned char b,unsigned char c,unsigned char d){
	example_do_connect(&static_client,MQTT_SERVER_IP);
}

void example_do_connect(mqtt_client_t *client,unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
  struct mqtt_connect_client_info_t ci;
  err_t err;
  struct ip4_addr ip_addr;
	IP4_ADDR(&ip_addr, a,b,c,d);
  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));
  
  /* Minimal amount of information required is client identifier, so set it here */ 
  ci.client_id = "stm32";
	ci.keep_alive=60;
	//ci.client_pass = "";
	ci.client_user = "Henry";
  /* Initiate client and connect to server, if this fails immediately an error code is returned
     otherwise mqtt_connection_cb will be called with connection result after attempting 
     to establish a connection with the server. 
     For now MQTT version 3.1.1 is always used */
  
  err = mqtt_client_connect(client, &ip_addr, MQTT_PORT, mqtt_connection_cb, 0, &ci);
  
  /* For now just print the result code if something goes wrong*/
  if(err != ERR_OK) {
    printf("mqtt_connect return %d\n", err);
  }
}
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  err_t err;
  if(status == MQTT_CONNECT_ACCEPTED) {
  //  printf("mqtt_connection_cb: Successfully connected\n");
    
    /* Setup callback for incoming publish requests */
		
    /* Subscribe to a topic named "my_stm32" with QoS level 1, call mqtt_sub_request_cb with result */ 
    err = mqtt_subscribe(client, "my_stm32", 1, mqtt_sub_request_cb, "my_stm32");
	  mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, "my_stm32");
    example_publish(&static_client,"new client connected");
    printf("mqtt server connected\n");

    if(err != ERR_OK) {
      printf("mqtt_subscribe return: %d\n", err);
    }
  } else {
    printf("mqtt_connection_cb: Disconnected, reason: %d\n", status);
    printf("connect failed,reconnecting to mqttserver\n");
    /* Its more nice to be connected, so try to reconnect */
    example_do_connect(client,MQTT_SERVER_IP);
  }  
}


static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity, 
     normal behaviour would be to take some action if subscribe fails like 
     notifying user, retry subscribe or disconnect from server */
	if(result==ERR_OK){
			printf("topic %s subscribed\n",(char*)arg);;
	}
}

static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{

  /* Decode topic string into a user defined reference */
  if(strcmp(topic, "my_stm32") == 0) {
		 printf("Incoming publish at topic %s with total length %u\n", (const char*)arg, (unsigned int)tot_len);
		inpub_id=1;
  } 
  else {
		inpub_id=0;
    /* For all other topics */
  }
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{

	
  if(flags & MQTT_DATA_FLAG_LAST) {
    /* Last fragment of payload received (or whole part if payload fits receive buffer
       See MQTT_VAR_HEADER_BUFFER_LEN)  */
      if(inpub_id==1){
				   printf("Incoming publish payload from %s with length %d, flags %u\n",(const char*)arg, len, (unsigned int)flags);

				printf("incoming data %s\n",data);
				if(strcmp((const char*)data,"closelight") ==0){
						GPIO_SetBits(GPIOC,GPIO_Pin_0);
					GPIO_SetBits(GPIOC,GPIO_Pin_6);
					GPIO_SetBits(GPIOC,GPIO_Pin_7);
					GPIO_SetBits(GPIOC,GPIO_Pin_8);
					 example_publish(&static_client,"closelight done");
           
				}
				if(strcmp((const char*)data,"openlight") ==0){
						GPIO_ResetBits(GPIOC,GPIO_Pin_0);
					GPIO_ResetBits(GPIOC,GPIO_Pin_6);
					GPIO_ResetBits(GPIOC,GPIO_Pin_7);
					GPIO_ResetBits(GPIOC,GPIO_Pin_8);
					  example_publish(&static_client,"openlight done");
				}		
			}
			
    /* Call function or do action depending on reference, in this case inpub_id */
	}
   else {
    /* Handle fragmented payload, store in buffer, write to file or whatever */
  }
	 memset((void *)data,'\0', strlen((const char *)data));
}

void example_publish(mqtt_client_t *client, void *arg)
{
  const char *pub_payload= arg;
  err_t err;
  u8_t qos = 1; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  err = mqtt_publish(client, "stm32_return", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, (void *)pub_payload);
  if(err == ERR_OK){
		printf("%s\n",pub_payload);
	}
	else  {
    printf("Publish err: %d\n", err);
  }
}

/* Called when publish is complete either with sucess or failure */

static void mqtt_pub_request_cb(void *arg, err_t result)
{
	const char* pub_data=(const char* )arg;
  if(result != ERR_OK) {
     printf("Publish result: %d\n", result);
  }
	else{
		printf("publish with data:%s success\n",pub_data);
	}
}
