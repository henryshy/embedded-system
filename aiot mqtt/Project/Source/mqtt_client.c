#include "mqtt_client.h"



extern 	aiot_sysdep_portfile_t g_aiot_sysdep_portfile;
static int32_t demo_state_logcb(int32_t code, char *message)
{
  //  printf("%s", message);
    return 0;
}
void mqtt_start(){

	aiot_state_logcb_t demo_state_logcb;
	core_mqtt_handle_t* mqtt_handle;
	//配置底层依赖和日志输出
	aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile); 
	aiot_state_set_logcb(demo_state_logcb);
	//创建MQTT客户端实例，并初始化默认参数
	mqtt_handle = (core_mqtt_handle_t*)aiot_mqtt_init();
  if (mqtt_handle == NULL) {
      return;
  }
	char *product_key       = "gft7D4ecukE";
    char *device_name       = "aiot_test";
    char *device_secret     = "7576f150b10e55856d9c8fb68e193eaa";
    char  mqtt_host[] = "iot-06z00c9i4829fe5.mqtt.iothub.aliyuncs.com";
    uint16_t port =443;
    aiot_sysdep_network_cred_t cred; /* 安全凭据结构体, 如果要用TLS, 这个结构体中配置CA证书等参数 */
  
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_NONE;
    
  
  
    /* 配置MQTT服务器地址。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)mqtt_host);
    /* 配置MQTT服务器端口。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* 配置设备ProductKey。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    /* 配置设备DeviceName。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    /* 配置设备DeviceSecret。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    /* 配置网络连接的安全凭据。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);

  /* 配置MQTT默认消息接收回调函数。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)mqtt_recv_handler);
    /* 配置MQTT事件回调函数。 */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)mqtt_event_handler);
  
   int32_t res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* 尝试建立连接失败, 销毁MQTT实例, 回收资源。 */
        aiot_mqtt_deinit((void **)&mqtt_handle);
        return ;
    }
  

}

void mqtt_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata){
	switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
          
            /* TODO: 处理服务器对心跳的回应, 一般不处理。 */
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
           
            /* TODO: 处理服务器对订阅请求的回应, 一般不处理。 */
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            
            /* TODO: 处理服务器下发的业务报文。 */
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            
            /* TODO: 处理服务器对QoS=1上报消息的回应, 一般不处理。 */
        }
        break;

        default: {

        }
    }
	
}
void mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata){

	 switch (event->type) {
        /* 调用了aiot_mqtt_connect()接口, 与MQTT服务器建立连接。 */
        case AIOT_MQTTEVT_CONNECT: {
        
            /* TODO: 处理SDK建立连接成功, 不可在此调用耗时较长的阻塞函数。 */
        }
        break;

        /* SDK因网络状况被动断开连接后, 成功自动发起重连。 */
        case AIOT_MQTTEVT_RECONNECT: {
            
            /* TODO: 处理SDK重连成功, 不可在此调用耗时较长的阻塞函数。 */
        }
        break;

        /* SDK因网络状况被动断开了连接, network底层读写失败, heartbeat没有按预期得到服务端心跳应答。 */
        case AIOT_MQTTEVT_DISCONNECT: {
            
            /* TODO: 处理SDK被动断开连接, 不可在此调用耗时较长的阻塞函数。 */
        }
        break;

        default: {

        }
    }
}

