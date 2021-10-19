#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

#include "lwip/apps/mqtt.h"
 #include "lwip/apps/mqtt_priv.h"
#define mqtt_subscribe(client,topic,qos,cb,arg)		  mqtt_sub_unsub(client, topic, qos, cb, arg, 1)

#define mqtt_unsubscribe(client,topic,cb,arg )		   mqtt_sub_unsub(client, topic, 0, cb, arg, 0)

#define clientid "eth123123|securemode=3,signmethod=hmacsha1,timestamp=789|"
#define username "eth123123&gft7mcQNNGo"
#define password "bb9ef4dd7395791fd2b7a435fa95815b6971cb2e"
extern mqtt_client_t static_client;


void start_mqtt(unsigned char a,unsigned char b,unsigned char c,unsigned char d);
void example_do_connect(mqtt_client_t *client,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, uint32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
void example_publish(mqtt_client_t *client, void *arg);
static void mqtt_sub_request_cb(void *arg, err_t result);
static void mqtt_pub_request_cb(void *arg, err_t result);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

#endif
