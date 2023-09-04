#include "mqtt_app.h"
#include "usart.h"
#include "hm609a.h"
#include "malloc.h"
#include <stdlib.h>

MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTString topicString = MQTTString_initializer;

u16 g_mqttHeartbeatNum=0;  //心跳计数
u32 g_mqttConnTim=0;     //注册失败重发延时
u8 g_mqttPublishFlag=0;
u8 g_mqttSubscribeFlag=0;
char *g_barCode="20230824900001";
char* payload = NULL;
int payloadlen = 0;

Byte8 MqttSubscrTopFlag1;
Byte8 MqttSubscrTopFlag2;

u16 buflen=400;
u8 *p  = NULL;
u8 *revicebuf = NULL;
u8 *publishbuf = NULL;
u8 msg_type;

void MQTT_Publish_Analysis_Json(u8* buf, cJSON *json)
{
	char *out;
	json=cJSON_Parse((char*)buf);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		myfree(SRAMIN,out);
	}
}



void MQTT_Init(char* chip_id)
{	
	data.clientID.cstring=chip_id;
	data.keepAliveInterval = 60;//设置心跳包间隔时间
	data.cleansession = 1;//清除会话
	data.username.cstring = "124";//用户名
	data.password.cstring = "124";//密码
	payloadlen = strlen(payload);
	if(p == NULL)
		p = mymalloc(SRAMIN,400);
	if(revicebuf == NULL)
		revicebuf = mymalloc(SRAMIN,200);
	if(publishbuf == NULL)
		publishbuf = mymalloc(SRAMIN,400);
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
		return 0;
	}
		return 1;
}

u8 Mqtt_Suback_Deserialize( u8* buf)
{
		u16 submsgid;
		int subcount;
		int granted_qos;

		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			printf("granted qos != 0, %d\n", granted_qos);
			return 0;
		}
		return 1;
}

u8 Mqtt_Publish_Deserialize( u8* buf, u8* out)
{
		u8 dup;
		int qos;
		u8 retained;
		u16 msgid;
		int payloadlen_in=0;
		u8* payload_in=NULL;
		char* topic=NULL;
		u8 topicLen=0;
		MQTTString receivedTopic;
		MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
				&payload_in, &payloadlen_in, buf, buflen);
		printf("receivedTopic:%s,%d",receivedTopic.lenstring.data,receivedTopic.lenstring.len);
		topic=receivedTopic.lenstring.data;
		topicLen=receivedTopic.lenstring.len;
		if(strstr((const char *)topic, "iob/iob/") != NULL)
		{
			MQTT_FLAG_REVICE=1;
			MQTT_FLAG_PHOTO=1;
		}else if(strstr((const char *)topic, "iob/s2d/log")!= NULL)
		{
			MQTT_FLAG_REVICE=1;
			if(strstr((const char *)topic, "logfile/")!= NULL)
			{
				MQTT_FLAG_LOG_LIST=1;
				
			}else if(strstr((const char *)topic, "logfile/")!= NULL)
			{
				MQTT_FLAG_LOG_INFO=1;
			}
		}else if(strstr((const char *)topic, "iob/update/")!= NULL)
		{
			MQTT_FLAG_REVICE=1;
			MQTT_FLAG_UPGRADE=1;
		}else if(strstr((const char *)topic, "iob/s2d/")!= NULL)
		{
			MQTT_FLAG_REVICE=1;
			if(strstr((const char *)topic, "open/")!= NULL)
			{
				MQTT_FLAG_DELIVER_OPEN=1;
				
			}else if(strstr((const char *)topic, "close/")!= NULL)
			{
				MQTT_FLAG_DELIVER_CLOSE=1;
			}else if(strstr((const char *)topic, "openLock/")!= NULL)
			{
				MQTT_FLAG_LOCK_OPEN=1;
			}else if(strstr((const char *)topic, "closeLock/")!= NULL)
			{
				MQTT_FLAG_LOCK_CLOSE=1;
			}else if(strstr((const char *)topic, "update_param/")!= NULL)
			{
				MQTT_FLAG_CONFIG_DOWNLOAD=1;
			}else if(strstr((const char *)topic, "get_params/")!= NULL)
			{
				MQTT_FLAG_CONFIG_UPLOAD=1;
			}else if(strstr((const char *)topic, "reset/")!= NULL)
			{
				MQTT_FLAG_RESTART=1;
			}
		}
		memcpy(out,payload_in,payloadlen_in);
		printf("message arrived: %.*s\n", payloadlen_in, payload_in); //消息到达
		return 1;	
}

u8 Mqtt_Pingresp_Deserialize(u8* buf)
{
	if(buf[0] == 0xD0&&buf[1] == 0x00){
		printf("heart ack %x %x",buf[0],buf[1]);
		return 1;
	}
	return 0;
}

char res[20];
u8 Mqtt_Deserialize_Handle(u8* msg_type,const u8* buf, u8* out)
{
	u8 ret=0;
	int len=0;
	char *pp;
	int size=0;
	u8 res_mqtt[400] = {0};
	memset(res,0,20);
	sprintf(res, "+IPURC: \"recv\",2,");
	pp = strstr((const char *)buf, (const char *)res);
	if( pp != NULL)
	{
		memset(out,0,300);
		pp += strlen(res);
		size = atoi(pp);
		pp = strstr((const char *)pp, "\n");
		memcpy(res_mqtt, pp+1, size);			
	
	if(hm609a_mqtt_conn_flag){
		if(*msg_type == SUBACK)
		{
			ret = Mqtt_Suback_Deserialize(res_mqtt);
			if(ret)
				*msg_type=PUBLISH ;
		} else if (*msg_type == PUBLISH)
		{
			ret = Mqtt_Publish_Deserialize(res_mqtt, out);
		} else if (*msg_type == PINGRESP)
		{
			ret = Mqtt_Pingresp_Deserialize(res_mqtt);
		} else {
			printf("err message type %d",*msg_type);
		}
	}
	else
	{
		ret = Mqtt_Connack_Deserialize(res_mqtt);
		hm609a_mqtt_conn_flag=ret;
	}
	}
	return ret;
}

void Mqtt_TIM_10ms(void)
{
	if((!hm609a_mqtt_conn_flag)&&g_mqttConnTim)g_mqttConnTim--;
}

void MQTT_Publish(void)
{
	int len=0;
	if(g_mqttHeartbeatNum == 20) {
		char  *buf = "{\"iob_job_result\": \"True\", \"id\": \"0\", \"cam_ID\": \"/dev/video0\", \"device_ID\": \"20210312000155\", \"cap_time\": \"2021-03-18 18:03:03\", \"exposure\": 5000, \"format\": \"YUYV\", \"size\": 1843200, \"resolution\": \"1280x720\", \"brightness\": 16, \"frame_diff\": 80, \"error_code\": 1, \"power_remain\": -28, \"file_name\":\"20210312000155_devvideo0_20210318180303\", \"method\": 2, \"gradient\": 0}";
		payloadlen = strlen(buf);
		if(payload==NULL)
			payload=mymalloc(SRAMIN,payloadlen);
		memcpy(payload,buf,payloadlen);
		topicString.cstring = "iob/job_result/x";
		g_mqttPublishFlag=1;
		g_mqttHeartbeatNum=21;
	}
	if(g_mqttPublishFlag){
		g_mqttPublishFlag=0;
		g_mqttConnTim=200;
		memset(p,0,buflen);
		msg_type = PUBLISH;
		if(payload!= NULL && payloadlen)
			len = MQTTSerialize_publish(p, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
		HM609A_Send_Data(2,p,len,0);
		if(payload!=NULL){
			myfree(SRAMIN,payload);
			payload=NULL;
		}
	}
}

void MQTT_HeartBeat(void)
{
	int len=0;
	if(g_mqttHeartbeatNum == 55){
			printf("MQTT heart");
			msg_type = PINGRESP;
			g_mqttHeartbeatNum = 0;
			g_mqttConnTim=200;
			len = MQTTSerialize_pingreq(p, buflen);//发送心跳
			HM609A_Send_Data(2,p,len,0);
	}
}

void MQTT_Subscribe(void)
{
	int len=0;
	static int req_qos = 0, msgid = 1;//QOS
	if(g_mqttSubscribeFlag){	
				g_mqttSubscribeFlag=0;
				memset(p,0,buflen);
				g_mqttConnTim=200;
				topicString.cstring = "iob/#";
				msg_type = SUBACK;
				len=MQTTSerialize_subscribe(p, buflen, 0, msgid, 1, &topicString, &req_qos);
				HM609A_Send_Data(2,p,len,0);
		}
}

u8 HM609A_Mqtt_Program(char* addr, int port)
{
	u16 i;
	u8 buf[500];
	int len;
	static u16 count=0;
	
	if(hm609a_connect_flag)//TCP连接建立
	{   
			i = USART1_Revice(buf);
			if(i)
      {
				printf("\r\nRecvMqtt:%s",buf);
				Mqtt_Deserialize_Handle(&msg_type, buf, publishbuf);
				if(hm609a_mqtt_conn_flag && msg_type == CONNACK) g_mqttSubscribeFlag=1;
			}
			if(!hm609a_mqtt_conn_flag )
			{
				if(g_mqttConnTim==0){
					if(count>3)//是注册失败超过次数后断开TCP重新连接
					{
						hm609a_connect_flag=0;
						hm609a_mqtt_conn_flag=0;
						count=0;
					}
					else
					{
						count++;
						g_mqttConnTim=200;
						memset(p,0,buflen);
						msg_type = CONNACK;
						len = MQTTSerialize_connect(p, buflen, &data);
						HM609A_Send_Data(2,p,len,0);
					}
				}		
			}
			else
			{
				if(count)count=0;
				MQTT_Subscribe();
				MQTT_HeartBeat();
				MQTT_Publish();
			}
	}		
	return 0;
}	

