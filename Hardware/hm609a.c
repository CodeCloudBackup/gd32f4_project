#include "hm609a.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"
#include "http_app.h"
#include <stdio.h>
#include <string.h>

void HM609A_Init(void)
{
	u8 status=0;
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOF,GPIO_Pin_12); // RESET,High, HM609A,Low time >300ms reset
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp_IN,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // W_DISABLE Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //AP_READY high:mcu sleep state
	 //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化 
	
	status = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3);
	if(!status){
		DG_PWRKEY_ON;	// Pwrkey,High, HM609A,Low time >550ms on/off
		delay_ms(1000);
		DG_PWRKEY_OFF;
	}
	printf("\r\nHM609A Init successed.status:%d\n",status);
}


u32  g_hm609aTim[7] = {0};        // ?????
u16  g_hm609aMqttWaitTim=0;	// ?????
u16  g_hm609aHttpWaitTim=0;
u16 g_hm609aHttp=0;
Byte8 g_hm609aFlag;
/*
????1ms????
*/
void HM609A_TIM_1ms(void)
{
		if(g_hm609aTim[0])g_hm609aTim[0]--;
    if(g_hm609aTim[1])g_hm609aTim[1]--;
		if(g_hm609aTim[2])g_hm609aTim[2]--;
		if(g_hm609aTim[3])g_hm609aTim[3]--;
    if(hm609a_mqtt_wait_flag)
    {
      if(g_hm609aMqttWaitTim)g_hm609aMqttWaitTim--;
    }

    if(g_hm609aHttpWaitTim)g_hm609aHttpWaitTim--;
		
}

/*
*重启模块处理函数
*
*/
static void HM609A_Restart(void)
{
    DG_RESET=0;             		// 模块复位
		memset(g_hm609aTim,0,sizeof(g_hm609aTim));   // 模块倒计时清零
    hm609a_config_flag=0;				// 模块配置标志清零
		hm609a_mqtt_conn_flag=0;		// 网络连接成功标志清零
    hm609a_mqtt_reg_flag=0;   	// mqtt注册成功标志
		hm609a_mqtt_wait_flag=0;
		hm609a_http_conn_flag=0;		//http连接成功标志
		hm609a_http_wait_flag=0;
		delay_ms(300);
}

/*
*模块重启处理程序
*重启完毕后返回1,否则返回0
*
*/
static u8 HM609A_Restart_Program(void)//模块重启流程
{
    static u8 sign = 0;  //重启流程
    if(g_hm609aTim[0] <= 0) //倒计时结束后进入流程处理,否则返回0未完成
    {
        switch (sign)//AIR208状态处理
        {
        case 0: //断电开始
        {
            DG_RESET_OFF;         //关闭模块电源
            g_hm609aTim[0] = 300;  //断电时间ms
            sign++;            //进入下一步流程
        }
        break;
        case 1: //上电
        {
            DG_RESET_ON;          //打开模块电源
            g_hm609aTim[0] = 5000;  //上电初始化时间ms
            sign++;            //进入下一步流程
        }
        break;
        default://上电初始化完成
        {
            sign = 0;          //上电初始化完毕,恢复参数
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
static u8 HM609A_Config(void)
{
	static char res_at[20];
	static u8 count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
	char buf[100];
	u16 len = 0;
	if(g_hm609aTim[0] == 0) //为0时发送测试数据
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
						g_hm609aTim[0] = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("ATE1\r\n");  //发送AT指令
					}
					break;
					case 1: // 查询网络注册情况，核心板会自动注册网络，上电到注册大概 10s 左右
          {
							printf("A|ATI\r\n");
							g_hm609aTim[0] = 2000;				//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nATI\r\n"); //发送AT指令
          }
					break;
					case 2: // 查询网络注册情况，核心板会自动注册网络，上电到注册大概 10s 左右
          {
							printf("A|AT+CGMR\r\n");
							g_hm609aTim[0] = 2000;				//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CGMR\r\n"); //发送AT指令
          }
					break;
					case 3: // 查询制造商信息
					{
							printf("A|AT+CFUN?\r\n");
							g_hm609aTim[0] = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CFUN?\r\n"); //发送AT指令
					}
					break;
					case 4: // 查询制造商信息
					{
							printf("A|AT+CPIN?\r\n");
							g_hm609aTim[0] = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CPIN?\r\n"); //发送AT指令
					}
					break;
					case 5:  // 查询系统信息及SIM状态
          {
						 printf("A|AT^SYSINFO\r\n");
             g_hm609aTim[0] = 2000;				//超时时间ms
             cnt = 30;   //重复检查次数,*air208_Tim后时总体时间
             strcpy(res_at, "^SYSINFO");	//设置返回判断关键字
             u1_printf("\r\nAT^SYSINFO\r\n");  //发送AT指令
					}
					break;
					case 6: // 查询SN值
					{
							printf("A|AT+CGSN\r\n");
							g_hm609aTim[0] = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+CGSN\r\n"); //发送AT指令
					}
					break;
					case 7: // 查询IP值
					{
							printf("A|AT+IPDNSR=\"debug.armlogic.tech\"\r\n");
							g_hm609aTim[0] = 5000;          	//超时时间ms
							cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+IPDNSR=\"debug.armlogic.tech\"\r\n"); //发送AT指令
					}
					break;
					case 8:
					{
						printf("\r\nA|ATE0\r\n");
						cnt = 3;
						g_hm609aTim[0] = 2000;
						strcpy(res_at, "OK");		//设置返回判断关键字
						u1_printf("\r\nATE0\r\n");  //发送AT指令
					}
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
		if(USART1_Revice((u8*)buf, 100))         //从串口3读取数据
		{
			if(strstr(buf, (const char *)res_at) != NULL) //检查是否包含关键字
			{
					g_hm609aTim[0] = 0; //定时清零
					count = 0;      //重试次数清零
					Signs++;        //下一个流程
			}
		}
	}
	return 0;
	
}

/*
*模块TCP连接
*成功返回1,执行中返回0,失败返回错误代码>=20
*/
static u8 HM609A_Connect(u8 sockid, char* addr, int port, NET_PROT protocol)
{
	static char res_at[10];
	static u8 count[7] = {0}, Signs[7] = {0}, cnt[7] = {0};
	char buf[100];
	if(g_hm609aTim[sockid] == 0)
	{
		if(count[sockid] > 0 && count[sockid] >= cnt[sockid]) //超过最大重复次数
		{
				count[sockid] = 0;      //次数清零
				cnt[sockid] = 1;        //最大次数复位
				Signs[sockid] = 0;      //流程清零
				return Signs[sockid] + 30;     //返回错误码
		} else {
			count[sockid]++;
			switch (Signs[sockid])//AIR208状态处理
      {
				case 0: //
        {
					printf("\r\nA|AT+IPOPEN=%d,\"TCP\",%s,%d,0,0\r\n",sockid,addr,port);
					cnt[sockid] = 3;
					g_hm609aTim[sockid] = 5000;
					strcpy(res_at,"OK"); 
					u1_printf("\r\nAT+IPOPEN=%d,\"TCP\",\"%s\",%d,0,0\r\n",sockid,addr,port);  //发送AT指令
				}
				break;
				case 1: //
        {
					cnt[sockid] = 3;
					g_hm609aTim[sockid] = 5000;
					
					if(protocol == MQTT_PROT)
					{
						printf("\r\nA|AT+IPSWTMD=%d,1\r\n",sockid);
						strcpy(res_at,"OK"); 
						u1_printf("\r\nAT+IPSWTMD=%d,1\r\n",sockid);  //发送AT指令
					} 
					else if (protocol == HTTP_PROT)
					{
						printf("\r\nA|AT+IPSWTMD=%d,2\r\n",sockid);
						strcpy(res_at,"CONNECT"); 
						u1_printf("\r\nAT+IPSWTMD=%d,2\r\n",sockid);  //发送AT指令
					}
					
				}
				break;
				default:// 等待连接建立成功
				{
						count[sockid] = 0;      //重试次数清零
            Signs[sockid] = 0;      //流程清零
            cnt[sockid] = 1;        //最大次数复位
            return 1;       //返回配置完成
				}
			}
		}
	}
	else
	{
		if(USART1_Revice( (u8*)buf, 100))        //从串口3读取数据
		{
			if(strstr(buf, res_at) != NULL) //检查是否包含关键字
			{
				g_hm609aTim[sockid] = 0; //定时清零
				count[sockid] = 0;      //重试次数清零
				Signs[sockid]++;        //下一个流程
			}
		}
	}
	return 0;
}

/*
*关闭TCP连接
*
*/
static u8 HM609A_Tcp_Off(u8 sockid)// 关闭TCP连接
{
    static u8 count[7] = {0}, state[7] = {0}, cnt[7] =  {1,1,1,1,1,1,1}; //重复次数,重启流程
		char buf[50];
    if(g_hm609aTim[sockid] == 0)
    {
       if(count[sockid] > 0 && count[sockid] >= cnt[sockid] ) //超过最大重复次数
       {
           count[sockid] = 0;      //次数清零
					 cnt[sockid] = 1;
					 state[sockid]=0;
           return 41;     //返回错误码
       }
       else
			{
					count[sockid]++;
					switch(state[sockid]){
					case 0:
					{
							u1_printf("\r\n+++\r\n"); 
							printf("A|AT+IPCLOSE=%d",sockid);    // 关闭TCP连接
							g_hm609aTim[sockid] = 1000;      //超时时间ms
							cnt[sockid]=3;
							u1_printf("\r\nAT+IPCLOSE=%d\r\n",sockid);     //发送AT指令
					}
					break;
					default:// 等待连接建立成功
					{
							count[sockid] = 0;      //重试次数清零
							state[sockid] = 0;      //流程清零
							cnt[sockid] = 1;        //最大次数复位
							return 1;       //返回配置完成
					}
				}
           
       }
    }
    else
    {
			if(USART1_Revice((u8*)buf, 50))         //从串口3读取数据
			{
				if(strstr((const char *)buf, "OK") != NULL) //连接设置成功
				{
					count[sockid] = 0;      //重试次数清零
					return 1;     //返回执行完毕
				}
				else if(strstr((const char *)buf, "913") != NULL) //连接设置成功
				{
					count[sockid] = 0;      //重试次数清零
					return 1;     //返回执行完毕
				}
				
			}
    }
    return 0;
}

static FunctionalState GetConnStatus( NET_PROT protocol)
{
		if(protocol == MQTT_PROT)
		{
			if(hm609a_mqtt_wait_flag && g_hm609aMqttWaitTim <= 0)
			{
				hm609a_mqtt_wait_flag=0;
				hm609a_mqtt_conn_flag=0;
				hm609a_mqtt_reg_flag=0;
			}
			if(!hm609a_mqtt_conn_flag) 
				return DISABLE;
		} else if (protocol == HTTP_PROT)
		{
			if(hm609a_http_wait_flag && g_hm609aHttpWaitTim <= 0)
			{
				hm609a_http_wait_flag=0;
				hm609a_http_conn_flag=0;
			}
			if(!hm609a_http_conn_flag) 
				return DISABLE;
		}
		return ENABLE;
}
/*
主循环程序，每个循环执行一次
*/
void HM609A_Tcp_Program(u8 sockid, char* addr, int port, NET_PROT protocol)
{
	u8 err = 0;    //返回得错误代码
	static u8 count=0,state=0;      // ????,???????
	switch (state)//AIR208状态处理
  {
		case 0:
		{
			if(!hm609a_config_flag)
			{
				if(HM609A_Restart_Program())
        {
            state++;
            count=0;
        }
			}else{
					state++;
					count=0;
			}
		}
		break;
		case 1:     //配置模块
    {
				if(hm609a_config_flag)
					err=1;
				else
					err = HM609A_Config();
        switch(err)
        {
        case 1:
        {
            //进入AT成功,跳到下一个流程
            state++;
						hm609a_config_flag=1;
            count=0;
					
        }
        break;
        case 2:
        {
						state=0;
						count=0;
            HM609A_Restart();
        }
        break;
        default:
            break;
        }
    }
    break;
		case 2:     //模块进入AT
    {
			err = HM609A_Connect(sockid, addr, port,protocol);
			switch(err)
      {
				 case 0:break; 		//正常流程,直接跳出
         case 1:
				 {
					 //连接成功,跳到下一个流程
           count=0;
					 hm609a_mqtt_conn_flag=1;
					 hm609a_mqtt_reg_flag=0;
           state+=10;
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
              HM609A_Restart();
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
		case 3:
		{
			if(HM609A_Tcp_Off(sockid))
				state--;//关闭连接后重新连接
		}
		break;
		default://TCP连接成功，开始数据收发
    {
        if(!GetConnStatus(protocol))
					state=3;//如果连接断开，执行断开TCP，重新创建TCP连接
    }break;
	}
}
u8* g_netData=NULL;

u8 HM609A_Http_Request(const u8 sockid, const char *host,const u32 port)
{
	static u8 count = 0, sign = 0, cnt = 1; //重复次数,重启流程
	u16 len = 0;
	char buf[10];

	if(g_hm609aHttpWaitTim == 0) //为0时发送测试数据
	{
		if(count > 0 && count >= cnt) //超过最大重复次数
		{
				count = 0;      //次数清零
				cnt = 1;        //最大次数复位
				len = sign + 20; //计算错误代码
				sign = 0;      //流程清零
				return len;     //返回错误码
		}
		else {
			count++;
			switch (sign)//AIR208状态处理
      {
				case 0:
				{
					printf("\r\nsend http request");
					cnt = 3;
					g_hm609aHttpWaitTim = 5000;
					Http_Send_Resquest(sockid,host,port); 
				}
				break;
				default:
				{
						count= 0;      //重试次数清零
            sign = 0;      //流程清零
            cnt = 1;        //最大次数复位
            return 1;       //返回配置完成
				}
				break;
			}
		}
	}
	else 
	{
		if(HTTP_Recvice(g_netData, 800))        //从串口3读取数据
		{
			
			if (g_sHttpCmdSta.sta_equip_ident == 1) {
				if (strstr((const char *)g_netData, "Set-Cookie: session="))
				{
					g_sHttpCmdSta.sta_equip_ident=2;
				}
			}
			if (g_sHttpCmdSta.sta_download_bin == 1) {
				if (strstr((const char *)g_netData, "application/octet-stream"))
				{
					g_sHttpCmdSta.sta_download_bin = 2;
				}
			}
			
			g_hm609aHttpWaitTim = 0; //定时清零
			count = 0;      //重试次数清零
			sign++;        //下一个流程
		}
	}
	return 0;
}

/*
主循环程序，每个循环执行一次
*/
void HM609A_TcpHttp_Program(u8 sockid, char* addr, int port, NET_PROT protocol)
{
	u8 err = 0;    //返回得错误代码
	static u8 state = 0,count = 0;      // ????,???????
	switch (state)//AIR208状态处理
  {
		case 0:
    {
			err = HM609A_Connect(sockid, addr, port, protocol);
			switch(err)
      {
				 case 0:break; 		//正常流程,直接跳出
         case 1:
				 {
					 //连接成功,跳到下一个流程
           count=0;
					 hm609a_http_conn_flag=1;
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
              HM609A_Restart();
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
		case 1:     //模块进入AT
    {
			err = HM609A_Http_Request(sockid, addr, port);
			switch(err)
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
              HM609A_Restart();
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
		case 2:
		{
			if(HM609A_Tcp_Off(sockid))
			{
				g_sHttpCmdSta.sta_cmd=0;
				state++;//关闭连接后重新连接
			}

		}
		break;
		default://TCP连接成功，开始数据收发
    {
      if(hm609a_http_wait_flag && g_hm609aHttpWaitTim <= 0)
			{
				hm609a_http_wait_flag=0;
				hm609a_http_conn_flag=0;
			}
			if(!hm609a_http_conn_flag && g_sHttpCmdSta.sta_cmd) 
					state=0;//如果连接断开，执行断开TCP，重新创建TCP连接
    }break;
	}
}

void HM609A_Send_Data(u8 sockid, const u8* data, u16 len, u8 flag, NET_PROT protocol)
{
	char *hexStr=NULL;
	if(protocol == MQTT_PROT)
	{	
		if(!hm609a_mqtt_conn_flag)
			return;//未连接，禁止发送
	}else if (protocol == HTTP_PROT)
	{
		if(!hm609a_http_conn_flag)
			return;//未连接，禁止发送
	}
	
	if(flag)
	{
		printf("SEND %s\r\n",data);
		USART1_SendData(data,len);	
	}
	else{
		if(hexStr == NULL)
			hexStr = mymalloc(SRAMIN,600);
		to_hex(data, len, hexStr);
		hexStr[len*2] = '\0';
		printf("AT+IPSENDEX=%d,%s, len:%d\r\n",sockid,hexStr,strlen(hexStr));
		u1_printf("\r\nAT+IPSENDEX=%d,\"%s\"\r\n",sockid, hexStr);
	}
	if (protocol == HTTP_PROT)
	{
		g_hm609aHttpWaitTim=10000;
		hm609a_http_wait_flag=1;
	}
	if(hexStr != NULL)
	{
		myfree(SRAMIN,hexStr);
		hexStr=NULL;
	}
		
}

void HM609A_test(void)
{
	
}
