#include "mqtt_app.h"
#include "usart.h"
#include "hm609a.h"

MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTString topicString = MQTTString_initializer;

u16 g_mqttHeartbeatNum=0;  //心跳计数

int buflen = 200;
char* payload = "iob/iob/job/x";
int payloadlen = 0;

u8 bufflen=200;
u8 p [200];
u8 revicebuf[200];
u8 msg_type;

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
	int len;
	if(hm609a_mqtt_conn_flag){
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
	}
	else
	{
		hm609a_mqtt_conn_flag=1;
		memset(p,0,bufflen);
		msg_type = CONNACK;
		len = MQTTSerialize_connect(p, bufflen, &data);
		HM609A_Send_Data(2,p,len);
	}
	
	return ret;
}

u8 HM609A_Mqtt_Program(char* addr, int port)
{
	u16 i;
	u8 buf[200];
	int len;
	static u16 count=0;
	MQTT_Init();
	if(hm609a_connect_flag)//TCP连接建立
	{  
			i = USART1_Revice(buf);
			if(i)
      {
				Mqtt_Deserialize_Handle(1, buf);
			}
			if(!hm609a_mqtt_conn_flag)
			{
				if(count>3)//是注册失败超过次数后断开TCP重新连接
        {
					hm609a_connect_flag=0;
					hm609a_mqtt_conn_flag=0;
					count=0;
				}
				else
				{
					count++;
					
				}
			}
			else
			{
				if(count)count=0;
				if(g_mqttHeartbeatNum == 55){
					g_mqttHeartbeatNum = 0;
					len = MQTTSerialize_pingreq(p, buflen);//发送心跳
					HM609A_Send_Data(2,p,len);
				}
			}
	}		
//	static uint8_t count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
//	u16 errCode = 0;
//	static u8 msg_type = 0;
//	static int len = 0;
//	if(hm609a_connect_flag == 0) //为0时发送测试数据
//	{
//		
//	}
//		if(count > 0 && count >= cnt) //超过最大重复次数
//			{
//				count = 0;      //次数清零
//				cnt = 1;        //最大次数复位
//				errCode = Signs + 20; //计算错误代码
//				Signs = 0;      //流程清零
//				return errCode;     //返回错误码
//			}
//		  else
//			{
//				count++;
//				switch (Signs)//AIR208状态处理
//        {

//					case 1: //
//          {
//						flag=0;
//						printf("A|AT+IPSWTMD=2,1\r\n");
//						g_hm609aTim = 2000;          //超时时间ms
//						cnt = 6;   //重复检查次数,*air208_Tim后时总体时间
//						strcpy(res_at,"OK"); 				
//						u1_printf("\r\nAT+IPSWTMD=2,1\r\n");  //发送AT指令
//					}
//					break;
//					case 2: //
//          {
//						
//						flag=0;
//						memset(p,0,bufflen);
//						MQTT_Init();
//						msg_type = CONNACK;
//						len = MQTTSerialize_connect(p, bufflen, &data);
//						g_hm609aTim = 2000;          //超时时间ms
//						cnt = 3;   //重复检查次数,*air208_Tim后时总体时间
//						printf("CONNECT|AT+IPSEND=2,%d",len);
//						strcpy(res_at,"OK"); 
//						u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);
//					//	printf("A|AT+IPSENDEX=2,\"101900044d51545404c2003c000331323400033132340003313234\"\r\n");
//					//	strcpy(res_at,"+IPURC"); 
//					//	u1_printf("\r\nAT+IPSENDEX=2,\"101900044d51545404c2003c000331323400033132340003313234\"\r\n");  //发送AT指令
//					}
//					break;
//					case 3: //
//					{
//						printf("A|Send Data %d",len);
//						flag=1;
//						g_hm609aTim = 2000; 
//						cnt = 3;
//						USART1_SendData(p,len);
//					}
//					break;
//					case 4:
//					{
//						int req_qos = 0;//QOS
//						int msgid = 1;
//						flag =0;
//						cnt = 3;  
//						memset(p,0,bufflen);
//						topicString.cstring = "iob/iob/job/x";
//						msg_type = SUBACK;
//						len=MQTTSerialize_subscribe(p, bufflen, 0, msgid, 1, &topicString, &req_qos);
//						printf("SUBSCRIBE|AT+IPSEND=2,%d\r\n",len);
//						strcpy(res_at,"OK"); 
//						u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);
//					}
//					break;
//					case 5: //
//					{
//						flag=1;
//						printf("A|Send Data %d",len);
//						g_hm609aTim = 2000; 
//						cnt = 3;
//						USART1_SendData(p,len);
//					}
//					break;
//					case 6: //发布请求
//					{
//							flag =0;
//							len = MQTTSerialize_publish(p, bufflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//							printf("A|AT+IPSEND=2,%d\r\n",len);
//							cnt = 1; 
//							g_hm609aTim = 2000;          //超时时间ms
//							strcpy(res_at,"OK"); 
//							u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);  //发送AT指令
//						}
//						break;
//					case 7: //发布请求
//          {
//						flag=1;
//						printf("A|Send Data %d",len);
//						g_hm609aTim = 2000; 
//						cnt = 3;
//						USART1_SendData(p,len);
//					}
//					break;
//					default:// 状态配置执行完毕
//					{
//						count = 0;      //重试次数清零
//						Signs = 0;      //流程清零
//						cnt = 1;        //最大次数复位
//						return 1;       //返回配置完成
//					}
//				}
//			}
//	}
//	else
//	{
//		if(USART1_Revice(revicebuf))         //从串口3读取数据
//		{
//				printf("Recv:%s",revicebuf);
//				if(flag){
//					unsigned char buf[200];
//					int buflen = sizeof(buf);
//				
//					if(MQTTPacket_read(buf, buflen, transport_getdata) == msg_type){
//						if(Mqtt_Deserialize_Handle(msg_type, buf) == 0)
//						{
//								g_hm609aTim = 0; //定时清零
//								count = 0;      //重试次数清零
//								Signs++;        //下一个流程
//						}
//					}
//					
//				}else{
//					if(strstr((const char *)revicebuf, (const char *)res_at) != NULL) //检查是否包含关键字
//					{
//						g_hm609aTim = 0; //定时清零
//						count = 0;      //重试次数清零
//						Signs++;        //下一个流程
//					}
//				}

//		}
//	}
	return 0;
}	

