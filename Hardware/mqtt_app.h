#ifndef __MQTT_APP_H
#define __MQTT_APP_H

#include "systick.h"
#include "MQTTPacket.h"

extern MQTTPacket_connectData data ;
extern MQTTString topicString;
extern char* payload;
extern int payloadlen;

void MQTT_Init(void);
int transport_getdata(unsigned char* buf, int count);
u8 Mqtt_Connack_Deserialize(u8* buf);
u8 Mqtt_Suback_Deserialize(u8* buf);
u8 Mqtt_Publish_Deserialize(u8* buf);
u8 Mqtt_Deserialize_Handle(u8 msg_type, u8* buf);
u8 HM609A_Mqtt_Program(char* addr, int port);
#endif
