#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

#include "aiot_state_api.h"
#include "aiot_sysdep_api.h"
#include "aiot_mqtt_api.h"
#include "core_mqtt.h"



void mqtt_recv_handler(void *handle, const aiot_mqtt_recv_t *packet, void *userdata);
void mqtt_event_handler(void *handle, const aiot_mqtt_event_t *event, void *userdata);

#endif
