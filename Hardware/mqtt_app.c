#include "mqtt_app.h"
#include "usart.h"
#include "hm609a.h"
#include "malloc.h"
#include <stdlib.h>
#include "http_app.h"
#include "json_parse.h"

MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

u16 g_mqttHeartbeatNum=0;  //心跳计数
u32 g_mqttConnTim=0;     //注册失败重发延时
u32 g_mqttHeartTim=0;
u8 g_mqttPublishFlag=0;
u8 g_mqttInitUp=0;
char *g_barCode="20230824900001";
char g_publictTopic[50];
int payloadlen = 0;
int g_qos=0;


Byte8 MqttSubscrTopFlag1;
Byte8 MqttSubscrTopFlag2;
Byte8 MqttSubscrTopFlag3;

u16 g_publishBufLen=1024;
u8 *g_sPublishBuf  = NULL;
u8 *revicebuf = NULL;
u8 *g_rPublishbuf = NULL;
#define None 254
u8 g_msgType=None;

void MQTT_TIM_10ms(void)
{
	if(hm609a_mqtt_conn_flag&&g_mqttConnTim)g_mqttConnTim--;
	if(g_mqttHeartTim)g_mqttHeartTim--;
	if(g_mqttInitUp) {
		g_mqttInitUp=0;
		MQTT_FLAG_UP_DELY_STA=1;
		MQTT_FLAG_UP_LOCK_STA=1;
		MQTT_FLAG_UP_DEVICE_STA=1;
		MQTT_FLAG_UP_CAMERA_STA=1;
		MQTT_FLAG_UP_APPVERSION=1;
	}
	
	if(g_mqttHeartbeatNum==30){
		g_mqttPublishFlag=1;
		g_mqttHeartbeatNum+=1;
		MQTT_FLAG_UP_DEVICE_STA=1;
	}
}


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



void MQTT_Init(char* chip_id, const char* pwd)
{	
	data.clientID.cstring=chip_id;
	data.keepAliveInterval = 60;//设置心跳包间隔时间
	data.cleansession = 1;//清除会话
	data.username.cstring = "124";//用户名
	data.password.cstring = "124";//密码
	if(g_sPublishBuf == NULL)
		g_sPublishBuf = mymalloc(SRAMIN,g_publishBufLen);
	if(revicebuf == NULL)
		revicebuf = mymalloc(SRAMIN,200);
	if(g_rPublishbuf == NULL)
		g_rPublishbuf = mymalloc(SRAMIN,g_publishBufLen);
}

int transport_getdata(u8* buf, int count)
{
	memcpy(revicebuf, (void*)&buf[g_mqttReadLen],count);
	g_mqttReadLen += count;
	return count;
}

static u8 Mqtt_Connack_Deserialize(u8* buf)
{
	unsigned char sessionPresent, connack_rc;

	if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, g_publishBufLen) != 1 || connack_rc != 0)
	{
		printf("Unable to connect, return code %d\n", connack_rc);
		return 0;
	}
		return 1;
}

static u8 Mqtt_Suback_Deserialize( u8* buf)
{
		u16 submsgid;
		int subcount;
		int granted_qos;

		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, g_publishBufLen);
		if (granted_qos != 0)
		{
			printf("granted qos != 0, %d\n", granted_qos);
		}
		return 1;
}

static u8 Mqtt_Puback_Deserialize(u8* buf, u8 qos)
{
	if(qos == 0)
	{
		return 1;
	}
	else if(qos == 1)
	{	
		if(buf[0]!=0x40)
			return 0;
	}
	else if(qos == 2)
	{	
		if(buf[0]!=0x50)
			return 0;
	}
	return 1;
}


static u8 Mqtt_Publish_Deserialize( u8* buf, u8* out)
{
		u8 dup,ret=0;
		int qos;
		u8 retained;
		u16 msgid;
		int payloadlen_in=0;
		u8* payload_in=NULL;
		char* topic=NULL;
		MQTTString receivedTopic;
		ret=MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
				&payload_in, &payloadlen_in, buf, g_publishBufLen);
		if(!ret) return 0;
		topic=receivedTopic.lenstring.data;
		if(strstr((const char *)topic, "iob/s2d/") != NULL)
		{
			MQTT_FLAG_RECEIVE=1;
			if(strstr((const char *)topic, "job/") != NULL)
			{
				MQTT_FLAG_PHOTO=1;
				
			}else if(strstr((const char *)topic, "logfile/")!= NULL)
			{
				MQTT_FLAG_LOG_LIST=1;
				MQTT_FLAG_LOG_INFO=1;
			}else if(strstr((const char *)topic, "update/")!= NULL)
			{
				MQTT_FLAG_RECEIVE=1;
				MQTT_FLAG_UPGRADE=1;
			}else	if(strstr((const char *)topic, "open/")!= NULL)
			{
				MQTT_FLAG_DELY_OPEN=1;
				
			}else if(strstr((const char *)topic, "close/")!= NULL)
			{
				MQTT_FLAG_DELY_CLOSE=1;
			}else if(strstr((const char *)topic, "openLock/")!= NULL)
			{
				MQTT_FLAG_LOCK_OPEN=1;
			}else if(strstr((const char *)topic, "closeLock/")!= NULL)
			{
				MQTT_FLAG_LOCK_CLOSE=1;
			}else if(strstr((const char *)topic, "update_param/")!= NULL)
			{
				MQTT_FLAG_CFG_DOWNLOAD=1;
			}else if(strstr((const char *)topic, "get_params/")!= NULL)
			{
				MQTT_FLAG_CFG_UPLOAD=1;
			}else if(strstr((const char *)topic, "reset/")!= NULL)
			{
				MQTT_FLAG_RESTART=1;
			}
		}
		memcpy(out,payload_in,payloadlen_in);
		printf("receivedTopic:%s,%d\r\n",receivedTopic.lenstring.data,receivedTopic.lenstring.len);
		printf("message arrived: %.*s\n",payloadlen_in, payload_in); //消息到达
		return 1;	
}

static u8 Mqtt_Pingresp_Deserialize(u8* buf)
{
	if(buf[0] == 0xD0&&buf[1] == 0x00){
		return 1;
	}
	return 0;
}


extern vu8 g_usart1RevFinish;
extern vu32 g_usart1Cnt; 
char *g_mqttRes="+IPURC: \"recv\",2,";
char *g_httpRes="HTTP/1.1";
static u32 MQTT_Recvice(u8 *buf, u32 buflen)
{
	u32 len = g_usart1Cnt;
	u32 size=0;
	char *ptr=NULL;
	if(!hm609a_mqtt_conn_flag) return 0;
	if(g_usart1RevFinish)
	{
		if(len > 0)
		{	
			ptr = strstr((const char *)USART1_RX_BUF, g_httpRes);
			if(ptr)
				return 0;
			ptr = strstr((const char *)USART1_RX_BUF, g_mqttRes);
			if(ptr != NULL)
			{	
				ptr += strlen(g_mqttRes);
				size = atoi(ptr);
				ptr = strstr((const char *)ptr,"\n");
				memcpy(buf, ptr+1, size);
				USART1_Clear();
				return size;	
			}
			else
			{
				USART1_Clear();
			}
			return 0;
		}
		else
		{
			USART1_Clear();
			return 0;
		}
	}
	return 0;
}

char* g_mqttSendPublish=NULL;

static u8 MQTT_Publish(u8 sockid, u8 dup, u8 retained, int qos, char* topic, char* publish_buf)
{
	int len=0;
	MQTTString topicString = MQTTString_initializer;
	printf("\r\nMQTT_Publish\r\n");
	printf("Topic:%s,%d\r\n",topic,strlen(topic));
	topicString.cstring=topic;
	//mymemcpy(topicString.cstring, topic, strlen(topic));

	if(publish_buf == NULL)
		return 0;
	payloadlen = strlen(publish_buf);
	memset(g_sPublishBuf,0,g_publishBufLen);
	g_msgType = PUBACK;
	printf("payloadLen:%d\r\n",payloadlen);
	if(payloadlen)
		len = MQTTSerialize_publish(g_sPublishBuf, g_publishBufLen, dup, qos, retained, 0, topicString, (u8*)publish_buf, payloadlen);
	HM609A_Send_Data(sockid,g_sPublishBuf,len,0,MQTT_PROT);
	if(!g_qos) hm609a_mqtt_wait_flag=0;
	g_mqttPublishFlag=0;
	return 1;
}

void MQTT_Package_Publish_Json(u8 sockid, char* topic, u32 payloadlen)
{
	char *pBuf=NULL;
	if(pBuf == NULL)
			pBuf=mymalloc(SRAMIN, 600);
	memset(pBuf,0,600);
	if(MQTT_FLAG_UP_PHOTO_RES)
	{
		sprintf(g_publictTopic,"iob/job_result/%s",g_barCode);
	}
	else if(MQTT_FLAG_UP_DELY_OPEN_RES)
	{
		sprintf(g_publictTopic,"iob/d2s/open/result");
	}
	else if(MQTT_FLAG_UP_DELY_STA)
	{
		
		memset(g_publictTopic,0,sizeof(g_publictTopic));
		sprintf(g_publictTopic,"iob/d2s/upload/finish/%s",g_barCode);
		DelyStatusJsonPackage(&g_sDelySta,pBuf);
		MQTT_Publish(sockid, 0, 0, g_qos,g_publictTopic,pBuf);
		MQTT_FLAG_UP_DELY_STA=0;
	}
	else if(MQTT_FLAG_UP_LOCK_STA)
	{
		
	}
	else if(MQTT_FLAG_UP_ALARM)
	{
		sprintf(g_publictTopic,"iob/d2s/alarm/%s",g_barCode);
	}
	else if(MQTT_FLAG_UP_CAMERA_STA)
	{
		sprintf(g_publictTopic,"iob/status/camera");
	}
	else if(MQTT_FLAG_UP_PARAM_RES)
	{
		sprintf(g_publictTopic,"iob/d2s/update_param_result/%s",g_barCode);
	}
	else if(MQTT_FLAG_UP_PARAM)
	{
		sprintf(g_publictTopic,"iob/d2s/upload_params/%s",g_barCode);
	}
	
	if(MQTT_FLAG_UP_DEVICE_STA)
	{
		u32 len = 0;
		memset(g_publictTopic,0,sizeof(g_publictTopic));
		sprintf(g_publictTopic,"iob/d2s/device/status/%s",g_barCode);
		len = DeviceStatusJsonPackage(g_sDeviceSta, pBuf);
		MQTT_Publish(sockid, 0, 0, g_qos,g_publictTopic,pBuf);
		MQTT_FLAG_UP_DEVICE_STA=0;
	}
	else if(MQTT_FLAG_UP_APPVERSION)
	{
		sprintf(g_publictTopic,"iob/report/version/%s",g_barCode);
	}
	else if(MQTT_FLAG_UP_LOG_LIST)
	{
		sprintf(g_publictTopic,"iob/d2s/loginfo/%s",g_barCode);
	}
	
	if(pBuf != NULL)
	{
		myfree(SRAMIN, pBuf);
		pBuf=NULL;
	}
}



static u8 MQTT_HeartBeat(u8 sockid)
{
	u16 len=0;
	static u16 count=0,state = 0, cnt = 1;
	u8 buf[300];
	if(g_mqttHeartTim==0)
	{
		if(count > 0 && count>= cnt)
		{	
			count= 0;      //次数清零
			cnt = 1;        //最大次数复位
			return 0;     //返回错误码
		}
		else{	
			if(g_mqttHeartbeatNum >= 50){
					count++;
					printf("\r\nMQTT_HeartBeat\r\n");
					
					cnt=5;
					len = MQTTSerialize_pingreq(g_sPublishBuf, g_publishBufLen);
					HM609A_Send_Data(sockid,g_sPublishBuf,len,0,MQTT_PROT);
			}
			else
			{
				count++;
				if(!g_qos) count=0;
				cnt=5;
				
				MQTT_Package_Publish_Json(sockid, g_publictTopic, payloadlen);
			}
			g_mqttHeartTim=200;
		}
	}
	else
	{
		if(MQTT_Recvice(buf, 300))
		{	
				printf("Mqtt ack:%02x%02x\r\n",buf[0],buf[1]);
				if(Mqtt_Publish_Deserialize(buf, g_rPublishbuf))
				{
					MQTT_FLAG_RECEIVE=1;
				}
				if(Mqtt_Pingresp_Deserialize(buf))
				{
					g_mqttHeartbeatNum=0;
					g_mqttHeartTim=0;
					count= 0;      //次数清零
					cnt = 1; 
					return 1;
				}
				if(Mqtt_Puback_Deserialize(buf, g_qos))
				{
					count= 0;      //次数清零
					cnt = 1; 
				
					g_mqttPublishFlag=0;
					g_mqttHeartTim=0;
					return 1;
				}
		}
	}
	return 1;
}


static u8 MQTT_Subscribe(u8 sockid)
{
	u16 len=0;
	static u16 count=0,state = 0, cnt = 1;
	static int req_qos = 0, msgid = 1;//QOS
	u8 buf[20];
	if(g_mqttConnTim==0)
	{
		if(count>0 && count >= cnt)
		{
			count=0;
			cnt=1;
			state=0;
			return 71;
		}
		else
		{
			count++;
			switch(state)
			{
				case 0:
				{
					MQTTString topicString = MQTTString_initializer;
					topicString.cstring = "iob/s2d/#";
					printf("\r\nMQTT_Subscribe\r\n");
					g_mqttConnTim=200;
					len=MQTTSerialize_subscribe(g_sPublishBuf, g_publishBufLen, 0, \
							msgid, 1, &topicString, &req_qos);
					HM609A_Send_Data(sockid,g_sPublishBuf,len,0,MQTT_PROT);
				}
				break;
				default:// 等待连接建立成功
				{
					count=0;      //重试次数清零
          state=0;      //流程清零
          cnt= 1;        //最大次数复位
					return 1;
				}
			}
		}
	}
	else
	{
		if(MQTT_Recvice(buf, 20))
		{	
				printf("Mqtt ack:%02x%02x",buf[0],buf[1]);
				if(Mqtt_Suback_Deserialize(buf))
				{
					g_mqttInitUp=1;
					g_mqttConnTim=0;
					count=0;
					state++;
				}
		}
	}
	return 0;
}

static u8 Mqtt_Connect(u8 sockid)
{
	static u16 count=0,state = 0, cnt = 1;
	u16 len=0;
	u8 buf[20];
	if(g_mqttConnTim==0)
	{
		if(count>0 && count >= cnt)
		{
			count=0;
			cnt=1;
			state=0;
			return 61;
		}
		else
		{
			count++;
			switch(state)
			{
				case 0:
				{
					printf("\r\nMQTT_Connect\r\n");
					cnt=3;
					g_mqttConnTim=200;
					memset(g_sPublishBuf,0,g_publishBufLen);
					g_msgType = CONNACK;
					len = MQTTSerialize_connect(g_sPublishBuf, g_publishBufLen, &data);
					HM609A_Send_Data(sockid,g_sPublishBuf,len,0,MQTT_PROT);
				}
				break;
				default:// 等待连接建立成功
				{
					count=0;      //重试次数清零
          state=0;      //流程清零
          cnt= 1;        //最大次数复位
					return 1;
				}
			}
		}
	}
	else
	{
		if(MQTT_Recvice(buf, 20))
		{	
			printf("Mqtt ack:%02x%02x",buf[0],buf[1]);
				u8 ret=0;
				ret = Mqtt_Connack_Deserialize(buf);
				hm609a_mqtt_reg_flag=ret;
				if(ret)
				{
					g_mqttConnTim=0;
					count=0;
					state++;
				}
		}
	}
	return 0;
}

u8 HM609A_Mqtt_Program(u8 sockid)
{
	u8 ret=0;
	static u8 state = 0, count = 0;
	if(hm609a_mqtt_conn_flag)//TCP连接建立
	{
		switch(state)
		{
			case 0:
			{
				if(!hm609a_mqtt_reg_flag)
				{
					ret=Mqtt_Connect(sockid);
					switch(ret)
					{
						case 0:break; 		//正常流程,直接跳出
						case 1:
						 {
						 //连接成功,跳到下一个流程
						 count=0;
						 hm609a_mqtt_reg_flag=1;
						 state++;
						}
						break;
						default:
						{
							//进入失败
							if(count>=3)
							{
								//超过重试次数，重启模块
								state=0;
								count=0;	
								hm609a_mqtt_conn_flag=0;
							}
							else
							{      
								count++;//重试次数+1
								state++;
							}
						}
					}
				}
				else
				{
					state++;
				}
			}
			break;
			case 1:
			{
				ret = MQTT_Subscribe(sockid);
				switch(ret)
				{
					 case 0:break; 		//正常流程,直接跳出
					 case 1:
					 {
						 //连接成功,跳到下一个流程
						 count=0;
						 state++;
					 }
					  break;
					 default:
					 {
							//进入失败
							if(count>=3)
							{
								//超过重试次数，重启模块
								state=0;
								count=0;	
								hm609a_mqtt_reg_flag=0;
							}
							else
							{      
								count++;//重试次数+1
								state++;
							}
						}
						break;
				}
			}
			break;
			default://TCP连接成功，开始数据收发
			{
				if(MQTT_HeartBeat(sockid) != 1)
				{
					printf("Disconnect E000\r\n");
					u1_printf("\r\nAT+IPSENDEX=%d,\"E000\"\r\n",sockid);
					hm609a_mqtt_reg_flag=0;
				}
				if(!hm609a_mqtt_reg_flag)
					state=0;
			}	
		}
	}	
	else 
	{
		hm609a_mqtt_reg_flag=0;
		g_msgType=None;
	}	
	return ret;
}	

