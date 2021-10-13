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
	//���õײ���������־���
	aiot_sysdep_set_portfile(&g_aiot_sysdep_portfile); 
	aiot_state_set_logcb(demo_state_logcb);
	//����MQTT�ͻ���ʵ��������ʼ��Ĭ�ϲ���
	mqtt_handle = (core_mqtt_handle_t*)aiot_mqtt_init();
  if (mqtt_handle == NULL) {
      return;
  }
	char *product_key       = "gft7D4ecukE";
    char *device_name       = "aiot_test";
    char *device_secret     = "7576f150b10e55856d9c8fb68e193eaa";
    char  mqtt_host[] = "iot-06z00c9i4829fe5.mqtt.iothub.aliyuncs.com";
    uint16_t port =443;
    aiot_sysdep_network_cred_t cred; /* ��ȫƾ�ݽṹ��, ���Ҫ��TLS, ����ṹ��������CA֤��Ȳ��� */
  
    memset(&cred, 0, sizeof(aiot_sysdep_network_cred_t));
    cred.option = AIOT_SYSDEP_NETWORK_CRED_NONE;
    
  
  
    /* ����MQTT��������ַ�� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_HOST, (void *)mqtt_host);
    /* ����MQTT�������˿ڡ� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PORT, (void *)&port);
    /* �����豸ProductKey�� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_PRODUCT_KEY, (void *)product_key);
    /* �����豸DeviceName�� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_NAME, (void *)device_name);
    /* �����豸DeviceSecret�� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_DEVICE_SECRET, (void *)device_secret);
    /* �����������ӵİ�ȫƾ�ݡ� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_NETWORK_CRED, (void *)&cred);

  /* ����MQTTĬ����Ϣ���ջص������� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_RECV_HANDLER, (void *)mqtt_recv_handler);
    /* ����MQTT�¼��ص������� */
    aiot_mqtt_setopt(mqtt_handle, AIOT_MQTTOPT_EVENT_HANDLER, (void *)mqtt_event_handler);
  
   int32_t res = aiot_mqtt_connect(mqtt_handle);
    if (res < STATE_SUCCESS) {
        /* ���Խ�������ʧ��, ����MQTTʵ��, ������Դ�� */
        aiot_mqtt_deinit((void **)&mqtt_handle);
        return ;
    }
  

}

void mqtt_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata){
	switch (packet->type) {
        case AIOT_MQTTRECV_HEARTBEAT_RESPONSE: {
          
            /* TODO: ����������������Ļ�Ӧ, һ�㲻���� */
        }
        break;

        case AIOT_MQTTRECV_SUB_ACK: {
           
            /* TODO: ����������Զ�������Ļ�Ӧ, һ�㲻���� */
        }
        break;

        case AIOT_MQTTRECV_PUB: {
            
            /* TODO: ����������·���ҵ���ġ� */
        }
        break;

        case AIOT_MQTTRECV_PUB_ACK: {
            
            /* TODO: �����������QoS=1�ϱ���Ϣ�Ļ�Ӧ, һ�㲻���� */
        }
        break;

        default: {

        }
    }
	
}
void mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata){

	 switch (event->type) {
        /* ������aiot_mqtt_connect()�ӿ�, ��MQTT�������������ӡ� */
        case AIOT_MQTTEVT_CONNECT: {
        
            /* TODO: ����SDK�������ӳɹ�, �����ڴ˵��ú�ʱ�ϳ������������� */
        }
        break;

        /* SDK������״�������Ͽ����Ӻ�, �ɹ��Զ����������� */
        case AIOT_MQTTEVT_RECONNECT: {
            
            /* TODO: ����SDK�����ɹ�, �����ڴ˵��ú�ʱ�ϳ������������� */
        }
        break;

        /* SDK������״�������Ͽ�������, network�ײ��дʧ��, heartbeatû�а�Ԥ�ڵõ����������Ӧ�� */
        case AIOT_MQTTEVT_DISCONNECT: {
            
            /* TODO: ����SDK�����Ͽ�����, �����ڴ˵��ú�ʱ�ϳ������������� */
        }
        break;

        default: {

        }
    }
}

