#include "hm609a.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"
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
    hm609a_mqtt_conn_flag = 0;    //IP注册成功标志
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
u8 HM609A_Config(void)
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
					case 7: // 查询IP值
					{
							printf("A|AT+IPDNSR=\"debug.armlogic.tech\"\r\n");
							g_hm609aTim = 3000;          	//超时时间ms
							cnt = 10;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "OK");		//设置返回判断关键字
							u1_printf("\r\nAT+IPDNSR=\"debug.armlogic.tech\"\r\n"); //发送AT指令
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

/*
*模块TCP连接
*成功返回1,执行中返回0,失败返回错误代码>=20
*/
u8 HM609A_Connect(u8 sockid, char* addr, int port)
{
	static u8 count = 0, Signs = 0, cnt = 1;
	char buf[200]={0};
	if(g_hm609aTim == 0)
	{
		if(count > 0 && count >= cnt) //超过最大重复次数
		{
				count = 0;      //次数清零
				cnt = 1;        //最大次数复位
				Signs = 0;      //流程清零
				return Signs + 30;     //返回错误码
		} else {
			count++;
			switch (Signs)//AIR208状态处理
      {
				case 0: //
        {
					printf("\r\nA|AT+IPOPEN=%d,\"TCP\",%s,%d,0,0\r\n",sockid,addr,port);
					cnt = 3;
					g_hm609aTim = 5000;
					strcpy(res_at,"OK");
					u1_printf("\r\nAT+IPOPEN=%d,\"TCP\",\"%s\",%d,0,0\r\n",sockid,addr,port);  //发送AT指令
				}
				break;
				case 1:
				{
					printf("\r\nA|AT+IPSWTMD=%d,1\r\n",sockid);
					cnt = 3;
					g_hm609aTim = 5000;
					strcpy(res_at,"OK");
					u1_printf("\r\nAT+IPSWTMD=%d,1\r\n",sockid);  //发送AT指令
				}
				break;
				case 2:
				{
					printf("\r\nA|ATE0\r\n");
					cnt = 3;
					g_hm609aTim = 2000;
					strcpy(res_at,"OK");
					u1_printf("\r\nATE0\r\n");  //发送AT指令
				}
				break;
				default:// 等待连接建立成功
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

/*
*关闭TCP连接
*
*/
u8 HM609A_Tcp_Off(u8 sockid)// 关闭TCP连接
{
    static uint8_t count = 0; //重复次数,重启流程
    char buf[50];
    if(g_hm609aTim == 0)
    {
        if(count >= 2) //超过最大重复次数
        {
            count = 0;      //次数清零
            return 41;     //返回错误码
        }
        else{
            printf("A|AT+IPCLOSE=%d",sockid);    // 关闭TCP连接
            g_hm609aTim = 1000;      //超时时间ms
            count++;
            u1_printf("\r\nAT+IPCLOSE=%d\r\n",sockid);     //发送AT指令
        }
    }
    else
    {
        if(USART1_Revice((u8*)buf))         //从串口3读取数据
        {
            if(strstr((const char *)buf, "OK") != NULL) //连接设置成功
            {
                count = 0;      //重试次数清零
                return 1;     //返回执行完毕
            }
        }
    }
    return 0;
}

char getCommond[100]= "GET /iob/download/test.txt HTTP/1.1\r\nHost:101.37.89.157\r\n\r\n";
u8 HM609A_Http_Program(void)
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
				
					case 0:
					{
						printf("A|AT+IPSWTMD=%d,1\r\n",1);
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 6;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 				
						u1_printf("\r\nAT+IPSWTMD=%d,1\r\n",1);  //发送AT指令
					}
					break;
					case 1: //取消核心板回显功能
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
					case 2: //
          {
						printf("\r\nA|send data\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1; 
						strcpy(res_at,"Content-Length"); 
						USART1_SendData((u8*)getCommond,strlen(getCommond)+1);  //发送AT指令
					}
					break;
					case 3:
					{
						
						printf("\r\nA|AT+IPRD\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 10;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("AT+IPRD=1,20\r\n");  //发送AT指令
					}
					break;
					case 4: //
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
		case 1:     //配置模块
    {
        err = HM609A_Config();
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
		case 2:     //模块进入AT
    {
			err = HM609A_Connect(2, "101.37.89.157", 1883);
			switch(err)
      {
				 case 0:break; 		//正常流程,直接跳出
         case 1:
				 {
					 //连接成功,跳到下一个流程
           count=0;
					 hm609a_connect_flag=1;//标记连接成功
					 hm609a_mqtt_conn_flag=0;
           g_hm609aState+=10;
				 }
				 break;
				 default:
         {
            //进入失败
            if(count>=3)
            {
              //超过重试次数，重启模块
              count=0;
              HM609A_Restart();
            }
            else
            {      
              count++;//重试次数+1
              g_hm609aState++;
            }
          }
          break;
			}
		}
		break;
		case 3:
		{
			if(HM609A_Tcp_Off(2))g_hm609aState--;//关闭连接后重新连接
		}
		break;
		default://TCP连接成功，开始数据收发
    {
        if(hm609a_send_return&&g_hm609aReturnTim<=0)
        {
             //发送超时，重新建立连接
            hm609a_send_return=0;
            hm609a_connect_flag=0;
            hm609a_mqtt_conn_flag=0;
        }
        //连接成功，如果连接断开将重新连接
        if(!hm609a_connect_flag)g_hm609aState=3;//如果连接断开，执行断开TCP，重新创建TCP连接
    }break;
	}
}

const char hex_table[] = {
'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
};
void to_hex(char *src, int l, char *dst)
{
	while(l--)
	{	
		*(dst+2*l+1) = hex_table[(*(src+l))&0x0f];
		*(dst+2*l) = hex_table[(*(src+l))>>4];
	}
}

void HM609A_Send_Data(u8 sockid, u8* data, u16 len)
{
	char *hexStr;
	if(!hm609a_connect_flag)return;//未连接，禁止发送
	hexStr = mymalloc(SRAMIN,200);
	to_hex((char*)data, len, hexStr);
	hexStr[len*2] = '\0';
	printf("\r\nAT+IPSENDEX=%d,%d",sockid,len*2);
	u1_printf("\r\nAT+IPSENDEX=%d,\"%s\"\r\n",sockid, hexStr);
	//USART1_SendData(data, len);
  g_hm609aReturnTim=30000;
  hm609a_send_return=1;
	myfree(SRAMIN,hexStr);
}

void HM609A_test(void)
{
	
}
