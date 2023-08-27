#include "hm609a.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"
#include "MQTTPacket.h"
#include <stdio.h>
#include <string.h>

void HM609A_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOF,GPIO_Pin_11);	// Pwrkey,High, HM609A,Low time >550ms on/off
	delay_ms(1000);
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	GPIO_ResetBits(GPIOF,GPIO_Pin_12); // RESET,High, HM609A,Low time >300ms reset
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp_IN,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // W_DISABLE Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //AP_READY high:mcu sleep state
	 //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化 
	printf("HM609A Init successed.\n");
}

char res_at[20];

u16  g_hm609aState = 0;      // ????,???????
u32  g_hm609aTim = 0;        // ?????
u16  g_hm609aReturnTim = 0;	// ?????
u8   g_hm609aSignal = 0;     // GPRS????
u16  g_hm609aHeartBeat = 0;  // ????
Byte8 g_hm609aFlag;
/*
????1ms????
*/
void HM609A_TIM_1ms(void)
{
    if(g_hm609aTim)g_hm609aTim--;
    if(hm609a_send_return)
    {
        if(g_hm609aReturnTim)g_hm609aReturnTim--;
    }
}

/*
*重启模块处理函数
*
*/
void HM609A_Restart(void)
{
    g_hm609aState = 0;       //状态标志重新开始
    DG_RESET=0;             //关闭模块电源
    g_hm609aTim = 0;         //模块倒计时清零
    hm609a_connect_flag = 0;//网络连接成功标志
    hm609a_reg_flag = 0;    //IP注册成功标志
		delay_ms(300);
}

/*
*模块重启处理程序
*重启完毕后返回1,否则返回0
*
*/
u8 HM609A_Restart_Program(void)//模块重启流程
{
    static uint8_t Signs = 0;  //重启流程
    if(g_hm609aTim <= 0) //倒计时结束后进入流程处理,否则返回0未完成
    {
        switch (Signs)//AIR208状态处理
        {
        case 0: //断电开始
        {
            DG_RESET_OFF;         //关闭模块电源
            g_hm609aTim = 300;  //断电时间ms
            Signs++;            //进入下一步流程
        }
        break;
        case 1: //上电
        {
            DG_RESET_ON;          //打开模块电源
            g_hm609aTim = 5000;  //上电初始化时间ms
            Signs++;            //进入下一步流程
        }
        break;
        default://上电初始化完成
        {
            Signs = 0;          //上电初始化完毕,恢复参数
            return 1;           //重启完成,返回1
        }
        }
    }
    return 0;
}

/*
*模块配置
*成功返回1,执行中返回0,失败返回错误代码>=20
*/
u8 HM609A_config(void)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
	u16 len = 0;
	char buf[200]={0};
	if(g_hm609aTim == 0) //为0时发送测试数据
	{
			if(count > 0 && count >= cnt) //超过最大重复次数
			{
				count = 0;      //次数清零
				cnt = 1;        //最大次数复位
				len = Signs + 20; //计算错误代码
				Signs = 0;      //流程清零
				return len;     //返回错误码
			}
		  else
			{
				count++;
				switch (Signs)//AIR208状态处理
        {
					case 0: //取消核心板回显功能
          {
						printf("A|ATE1\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("ATE1\r\n");  //发送AT指令
					}
					break;
					case 1: // 查询网络注册情况，核心板会自动注册网络，上电到注册大概 10s 左右
          {
							printf("A|ATI\r\n");
							g_hm609aTim = 2000;				//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nATI\r\n"); //发送AT指令
          }
					break;
					case 2: // 查询网络注册情况，核心板会自动注册网络，上电到注册大概 10s 左右
          {
							printf("A|AT+CGMR\r\n");
							g_hm609aTim = 2000;				//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CGMR\r\n"); //发送AT指令
          }
					break;
					case 3: // 查询制造商信息
					{
							printf("A|AT+CFUN?\r\n");
							g_hm609aTim = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CFUN?\r\n"); //发送AT指令
					}
					break;
					case 4: // 查询制造商信息
					{
							printf("A|AT+CPIN?\r\n");
							g_hm609aTim = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CPIN?\r\n"); //发送AT指令
					}
					break;
					case 5:  // 查询系统信息及SIM状态
          {
						 printf("A|AT^SYSINFO\r\n");
             g_hm609aTim = 2000;				//超时时间ms
             cnt = 30;   //重复检查次数,*air208_Tim后时总体时间
             strcpy(res_at, "^SYSINFO");	//设置返回判断关键字
             u1_printf("\r\nAT^SYSINFO\r\n");  //发送AT指令
					}
					break;
					case 6: // 查询SN值
					{
							printf("A|AT+CGSN\r\n");
							g_hm609aTim = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CGSN\r\n"); //发送AT指令
					}
					break;
					default:// 状态配置执行完毕
					{
						count = 0;      //重试次数清零
						Signs = 0;      //流程清零
						cnt = 1;        //最大次数复位
						return 1;       //返回配置完成
					}
				}
			}
	}
	else
	{
		if(USART1_Revice((u8*)buf))         //从串口3读取数据
		{
				printf("Recv:%s",buf);
				if(strstr((const char *)buf, (const char *)res_at) != NULL) //检查是否包含关键字
				{
					g_hm609aTim = 0; //定时清零
					count = 0;      //重试次数清零
					Signs++;        //下一个流程
				}
		}
	}
	return 0;
	
}

char getCommond[100]= "GET /iob/download/test.txt HTTP/1.1\r\nHost:101.37.89.157\r\n\r\n";
u8 HM609A_Tcp(void)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
	u16 len = 0;
	char buf[200];
	if(g_hm609aTim == 0) //为0时发送测试数据
	{
		if(count > 0 && count >= cnt) //超过最大重复次数
			{
				count = 0;      //次数清零
				cnt = 1;        //最大次数复位
				len = Signs + 20; //计算错误代码
				Signs = 0;      //流程清零
				return len;     //返回错误码
			}
		  else
			{
				count++;
				switch (Signs)//HTTP配置查询
        {
					case 0: //
          {
						printf("A|AT+IPOPEN?\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("AT+IPOPEN?\r\n");  //发送AT指令
					}
					break;
					case 1: //
          {
						printf("A|AT+IPOPEN=\"TCP\"\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						
						u1_printf("\r\nAT+IPOPEN=1,\"TCP\",\"101.37.89.157\",80,0,0\r\n");  //发送AT指令
					}
					break;
					case 2: //
          {
						printf("A|AT+IPSWTMD=1,1\r\n");
						g_hm609aTim = 5000;          //超时时间ms
						cnt = 6;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						
						u1_printf("\r\nAT+IPSWTMD=1,1\r\n");  //发送AT指令
					}
					break;
					case 3: //取消核心板回显功能
          {
						u16 len = sizeof(getCommond);
						getCommond[len] = '\0';
						printf("\r\nA|AT+IPSEND=1\r\n");
						g_hm609aTim = 5000;          //超时时间ms
						cnt = 10;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"CONNECT"); 
						u1_printf("\r\nAT+IPSEND=1,%d\r\n",strlen(getCommond)+1);  //发送AT指令
						
					}
					break;
					
					case 4: //
          {
						printf("\r\nA|send data\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1; 
						strcpy(res_at,"Content-Length"); 
						USART1_SendData((u8*)getCommond,strlen(getCommond)+1);  //发送AT指令
					}
					break;
					case 5:
					{
						
						printf("\r\nA|AT+IPRD\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 10;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("AT+IPRD=1,20\r\n");  //发送AT指令
					}
					break;
					case 6: //
          {
						printf("A|AT+IPCLOSE=1\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						
						u1_printf("AT+IPCLOSE=1\r\n");  //发送AT指令
					}
					break;
					default:// 状态配置执行完毕
					{
						count = 0;      //重试次数清零
						Signs = 0;      //流程清零
						cnt = 1;        //最大次数复位
						return 1;       //返回配置完成
					}
				}
			}
	}
	else
	{
		if(USART1_Revice((u8*)buf))         //从串口3读取数据
		{
				printf("Recv:%s",buf);
				if(strstr((const char *)buf, (const char *)res_at) != NULL) //检查是否包含关键字
				{
					g_hm609aTim = 0; //定时清零
					count = 0;      //重试次数清零
					Signs++;        //下一个流程
				}
		}
	}
	return 0;
}	


u8 HM609A_Http(void)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
	u16 len = 0;
	char buf[200];
	if(g_hm609aTim == 0) //为0时发送测试数据
	{
		if(count > 0 && count >= cnt) //超过最大重复次数
			{
				count = 0;      //次数清零
				cnt = 1;        //最大次数复位
				len = Signs + 20; //计算错误代码
				Signs = 0;      //流程清零
				return len;     //返回错误码
			}
		  else
			{
				count++;
				switch (Signs)//HTTP配置查询
        {
					case 0: //
          {
						printf("A|AT+HTTPCFG?\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=?\r\n");  //发送AT指令
					}
					break;
					case 1: //
          {
						char *url = "http://101.37.89.157/iob/download/test.txt";
						u16 len = strlen(url); 
						printf("A|AT+HTTPURL\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						
						u1_printf("AT+HTTPURL=%d,20\r\n",len);  //发送AT指令
						u1_printf("%s",url);
					}
					break;
					case 2: //取消核心板回显功能
          {
						printf("A|AT+HTTPGET=10\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"CONNECT"); 
						u1_printf("AT+HTTPGET=10\r\n");  //发送AT指令
					}
					break;
					default:// 状态配置执行完毕
					{
						count = 0;      //重试次数清零
						Signs = 0;      //流程清零
						cnt = 1;        //最大次数复位
						return 1;       //返回配置完成
					}
				}
			}
	}
	else
	{
		if(USART1_Revice((u8*)buf))         //从串口3读取数据
		{
				printf("Recv:%s",buf);
				if(strstr((const char *)buf, (const char *)res_at) != NULL) //检查是否包含关键字
				{
					g_hm609aTim = 0; //定时清零
					count = 0;      //重试次数清零
					Signs++;        //下一个流程
				}
		}
	}
	return 0;
}	

MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTString topicString = MQTTString_initializer;

void MQTT_Init(void)
{	
	data.clientID.cstring = "124";//客户端ID
	data.keepAliveInterval = 60;//设置心跳包间隔时间
	data.cleansession = 1;//清除会话
	data.username.cstring = "124";//用户名
	data.password.cstring = "124";//密码
}
u8 p [200];
int buflen = 200;
char* payload = "iob/iob/job/x";
u8 revicebuf[200];

int transport_getdata(unsigned char* buf, int count)
{
	memcpy(revicebuf, buf,count);
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


static u8 flag = 0;
u8 HM609A_Mqtt_Program(char* addr, int port)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
	static int len = 0;
	u16 errCode = 0;
	static u8 msg_type = 0;
	int payloadlen = strlen(payload);
	
	if(g_hm609aTim == 0) //为0时发送测试数据
	{
		if(count > 0 && count >= cnt) //超过最大重复次数
			{
				count = 0;      //次数清零
				cnt = 1;        //最大次数复位
				errCode = Signs + 20; //计算错误代码
				Signs = 0;      //流程清零
				return errCode;     //返回错误码
			}
		  else
			{
				count++;
				switch (Signs)//AIR208状态处理
        {
					case 0: //
          {
						flag=0;
						printf("\r\nAT+IPOPEN=2,\"TCP\",\"%s\",1883,0,0\r\n",addr);
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 3;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("\r\nAT+IPOPEN=2,\"TCP\",\"101.37.89.157\",1883,0,0\r\n");  //发送AT指令
					}
					break;
					case 1: //
          {
						flag=0;
						printf("A|AT+IPSWTMD=2,1\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 6;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 				
						u1_printf("\r\nAT+IPSWTMD=2,1\r\n");  //发送AT指令
					}
					break;
					case 2: //
          {
						flag=0;
						memset(p,0,buflen);
						MQTT_Init();
						msg_type = CONNACK;
						len = MQTTSerialize_connect(p, buflen, &data);
						printf("A|AT+IPSENDEX=2,\"101900044d51545404c2003c000331323400033132340003313234\"\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"+IPURC"); 
						
						u1_printf("\r\nAT+IPSENDEX=2,\"101900044d51545404c2003c000331323400033132340003313234\"\r\n");  //发送AT指令
					}
					break;
					case 3: //
					{
						g_hm609aTim = 20000; 
						cnt = 100;
						flag=1;
					}
					break;
//						int req_qos = 0;//QOS
//						int msgid = 1;
//						flag =0;
//						memset(p,0,buflen);
//						msg_type = CONNACK;
//						topicString.cstring = "iob/iob/job/x";
//						len=MQTTSerialize_subscribe(p, buflen, 0, msgid, 1, &topicString, &req_qos);
//						printf("A|AT+IPSEND=2,%d\r\n",len);
//						g_hm609aTim = 5000;          //超时时间ms
//						cnt = 6;   //重复检查次数,*air208_Tim后时总体时间
//						strcpy(res_at,"OK"); 
//						u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);  //发送AT指令
//					}
//					break;
//					case 5: //订阅请求
//          {
//						
//						msg_type = SUBACK;
//						printf("A|send data%s\r\n",p);
//						g_hm609aTim = 2000;          //超时时间ms
//						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
//						p[len]= '\0';
//						u1_printf("%s",p);
//					}
//					break;
//					case 6: //发布请求
//          {
//						flag =0;
//						len = MQTTSerialize_publish(p, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//						printf("A|AT+IPSEND=2,%d\r\n",len);
//						cnt = 1; 
//						g_hm609aTim = 2000;          //超时时间ms
//						strcpy(res_at,"OK"); 
//						u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);  //发送AT指令
//					}
//					break;
//					case 7: //发布请求
//          {
//						flag =1;
//						msg_type = PUBLISH;
//						printf("A|send data%s\r\n",p);
//						g_hm609aTim = 2000;          //超时时间ms
//						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
//						p[len]= '\0';
//						USART1_SendData(p, len+1);
//					}
					break;
					default:// 状态配置执行完毕
					{
						count = 0;      //重试次数清零
						Signs = 0;      //流程清零
						cnt = 1;        //最大次数复位
						return 1;       //返回配置完成
					}
				}
			}
	}
	else
	{
		if(USART1_Revice(revicebuf))         //从串口3读取数据
		{
				printf("Recv:%s",revicebuf);
				if(flag){
					unsigned char buf[200];
					int buflen = sizeof(buf);
					g_hm609aTim = 0; //定时清零
					count = 0;      //重试次数清零
				//	Signs++;        //下一个流程
//					if(MQTTPacket_read(buf, buflen, transport_getdata)==msg_type){
//						Mqtt_Deserialize_Handle(msg_type, buf);
//					}
					
				}else{
					if(strstr((const char *)revicebuf, (const char *)res_at) != NULL) //检查是否包含关键字
					{
						g_hm609aTim = 0; //定时清零
						count = 0;      //重试次数清零
						Signs++;        //下一个流程
					}
				}

		}
	}
	return 0;
}	
/*
主循环程序，每个循环执行一次
*/
void HM609A_Program(char* addr, int port)
{
	u8 err = 0;    //返回得错误代码
	static u8 count = 0;  //重复次数
	switch (g_hm609aState)//AIR208状态处理
  {
		case 0:
		{
				if(HM609A_Restart_Program())
        {
            g_hm609aState++;
            count=0;
        }
		}
		break;
		case 1:     //模块进入AT
    {
        err = HM609A_config();
        switch(err)
        {
        case 1:
        {
            //进入AT成功,跳到下一个流程
            g_hm609aState++;
            count=0;
        }
        break;
        case 2:
        {
            HM609A_Restart();
        }
        break;
        default:
            break;
        }
    }
    break;
		case 2:
		{
			err = HM609A_Tcp();
			switch(err)
      {
         case 0:break; 		//正常流程,直接跳出
         case 1:
         {
						//初始化,跳到下一个流程
						count=0;
						g_hm609aState++;
          }
          break;
          default:
          {
                //初始化失败，重启模块
             HM609A_Restart();
          }
          break;
      }
		}
		break;
		case 3:
		{
			char heart[2]= {0xC0, 0x00};
			err = HM609A_Mqtt_Program(addr, port);
			switch(err)
      {
         case 0:
							if(g_hm609aHeartBeat == 55 &&flag)
							{
								printf("\r\nAT+IPSENDEX=2,\"C000\"\r\n");
								u1_printf("\r\nAT+IPSENDEX=2,\"C000\"\r\n");
								g_hm609aHeartBeat=0;
							}
							hm609a_connect_flag = 1;
				 case 1:break; 		//正常流程,直接跳出
         case 2:
         {
						//初始化,跳到下一个流程
						count=0;
						g_hm609aState++;
          }
          break;
          default:
          {
                //初始化失败，重启模块
             HM609A_Restart();
          }
          break;
      }
		}
		break;
		default://TCP连接成功，开始数据收发
    {
        if(hm609a_send_return&&g_hm609aReturnTim<=0)
        {
             //发送超时，重新建立连接
            hm609a_send_return=0;
            hm609a_connect_flag=0;
            hm609a_reg_flag=0;
        }
        //连接成功，如果连接断开将重新连接
        if(!hm609a_connect_flag)g_hm609aState=3;//如果连接断开，执行断开TCP，重新创建TCP连接
    }break;
	}
}

void HM609A_test(void)
{
	
}
