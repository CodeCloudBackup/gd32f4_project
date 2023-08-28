#include "mqtt_app.h"
#include "usart.h"

MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTString topicString = MQTTString_initializer;

int buflen = 200;
char* payload = "iob/iob/job/x";
int payloadlen = 0;

extern u8 revicebuf[200];

void MQTT_Init(void)
{	
	data.clientID.cstring = "124";//客户端ID
	data.keepAliveInterval = 60;//设置心跳包间隔时间
	data.cleansession = 1;//清除会话
	data.username.cstring = "124";//用户名
	data.password.cstring = "124";//密码
	payloadlen = strlen(payload);
}

int transport_getdata(unsigned char* buf, int count)
{
	memcpy(revicebuf, (void*)&buf[g_mqttReadLen],count);
	g_mqttReadLen += count;
	return count;
}



u8 Mqtt_Connack_Deserialize(u8* buf)
{
	unsigned char sessionPresent, connack_rc;

	if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
	{
		printf("Unable to connect, return code %d\n", connack_rc);
		return 1;
	}
		return 0;
}

u8 Mqtt_Suback_Deserialize(u8* buf)
{
	unsigned short submsgid;
		int subcount;
		int granted_qos;

		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			printf("granted qos != 0, %d\n", granted_qos);
			return 1;
		}
		return 0;
}

u8 Mqtt_Publish_Deserialize(u8* buf)
{
		unsigned char dup;
		int qos;
		unsigned char retained;
		unsigned short msgid;
		int payloadlen_in;
		unsigned char* payload_in;
		MQTTString receivedTopic;

		MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
				&payload_in, &payloadlen_in, buf, buflen);
		printf("message arrived %.*s\n", payloadlen_in, payload_in); //消息到达
		return 0;	
}

u8 Mqtt_Deserialize_Handle(u8 msg_type, u8* buf)
{
	u8 ret=0;
	switch(msg_type)
	{
		case CONNACK:
			ret = Mqtt_Connack_Deserialize(buf);
		break;
		case SUBACK:
			ret = Mqtt_Suback_Deserialize(buf);
		break;
		case PUBLISH:
			ret = Mqtt_Publish_Deserialize(buf);
		break;
		default:// 状态配置执行完毕
		{
			printf("err message type %d",msg_type);
		}
	}
	return ret;
}
