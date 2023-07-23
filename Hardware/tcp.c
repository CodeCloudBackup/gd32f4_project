#include "tcp.h"
#include "esp8266.h"
#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "usart.h"
#include "delay.h"

char res_at[20];

vu8 TcpClosedFlag = 0;
u16 g_state = 0;
u16 g_send_return_tim = 0;
u8 g_signal = 0;
u16 g_heartbeat = 0;
u32 g_tcpTime = 0;
Byte8 _esp8266_flag;

void TCP_TIM_10ms(void)
{
		if(g_tcpTime) g_tcpTime--;
		if(esp8266_send_return) {
				if(g_send_return_tim)g_send_return_tim--;
		}
}

void TCP_Restart(void)
{
		g_state = 0;
		ESP8266_RST=0;
		g_tcpTime = 0;
		esp8266_conn_flag = 0;
		esp8266_reg_flag = 0;
}
	
u8 TCP_Restart_Program(void)
{
	static u8 signs = 0;
	if(g_tcpTime <= 0){
		switch(signs)
		{
			case 0:
			{
				// 关闭模块电源
				ESP8266_RST=0;
				g_tcpTime = 50;
				signs++;
			}
			break;
			case 1:
			{
				// 打开模块电源
				ESP8266_RST=1;
				g_tcpTime = 1500;
				signs++;
			}
			break;
			default:
			{
				signs = 0;
				return 1;
			}
		}	
	}
	return 0;
}

u8 TCP_InAT(void)
{
		static u8 cnt = 0, sign = 0;
		switch(sign)
		{
			case 0:
			{
				cnt = 0;
				sign++;
				g_tcpTime = 150;
				USART2_Clear(); //清空接收缓存
			}
			break;
			case 1:
			{
				if(g_tcpTime == 0)
				{
					if(cnt >= 2)	// 发送测试数据++
					{
						cnt = 0;
						sign++;
					}else{
						printf("+++\r\nAT命令测试%d\r\n",cnt+1);
						g_tcpTime = 150;
						cnt++;
						u2_printf("+++");
					}
				}
			}
			break;
			case 2:
			{
				if(g_tcpTime == 0)
				{
					if(cnt >= 5)
					{
						cnt = 0;
						sign = 0;
						return 10;
					}else {
					 	printf("AT\r\n进入AT模式%d\r\n",cnt+1);
						g_tcpTime = 150;
						cnt++;
						u2_printf("AT\r\n");
					}
				}else{
					if(ESP8266_Check_AT_Response("OK")){
							g_tcpTime = 0;	//定时清零
							cnt = 0;		//重试次数清零
							sign = 0;	  //流程清零
							return 1;   //返回进入AT成功
					}
				}
			}
			break;
			default:
			{
					cnt = 0;
					sign = 0;
					return 10;
			}
		}
		return 0;
}

u8 TCP_gprs(void)//获取GPRS定位信息
{
	return 1;
}

/*
*关闭TCP连接
*
*/
u8 ESP8266_tcp_off(void)// 关闭TCP连接
{
    static u8 count = 0; //重复次数,重启流程
		if(g_tcpTime == 0)
    {
        if(count >= 2) //超过最大重复次数
        {
            count = 0;      //次数清零
            return 41;     //返回错误码
        }
        else{
//            printf("A|AT+CIPCLOSE");    // 关闭TCP连接
            g_tcpTime = 100;      //超时时间ms
            count++;
            u2_printf("AT+CIPCLOSE\r\n");     //发送AT指令
        }
    }
    else
    {
				if(ESP8266_Check_AT_Response("OK"))
				{
						count = 0;      //重试次数清零
						return 1;     //返回执行完毕
				}
				
    }
    return 0;
}

/*
*模块配置
*成功返回1,执行中返回0,失败返回错误代码>=20
*/

static char res_at[20];
u8 TCP_config(void)
{
    static u8 count = 0, Signs = 0, cnt = 1; //重复次数,重启流程
    u16 len = 0;
    if(g_tcpTime == 0) //为0时发送测试数据
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
                    printf("A|ATE0\r\n");
										ESP8266_AT_ATEO(res_at);
                    g_tcpTime = 200;          //超时时间ms
                    cnt = 3;   //重复检查次数,*air208_Tim后时总体时间

                }
                break;
                case 1: //  设置波特率
                {
										ESP8266_AT_BaudRate(res_at);
                   printf("A|“AT+IPR=115200;&W\r\n");
                    g_tcpTime = 200;          //超时时间ms
                    cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
                }
                break;
                case 2: // 设置为STA模式
                {
                   printf("A|AT+CIPMODE=0\r\n");
                    g_tcpTime = 200;          	//超时时间ms
                    cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
                    ESP8266_Net_Mode_Choose(STA,res_at);
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
				if(ESP8266_Check_AT_Response(res_at))
				{
					 g_tcpTime = 0; //定时清零
           count = 0;      //重试次数清零
           Signs++;        //下一个流程
				}
    }
    return 0;
}

u8 TCP_Connect(void)// 建立TCP连接
{
	static u8 count = 0, sign = 0,cnt = 1;
	u16 len = 0;
	char response[100];
	if(g_tcpTime == 0)
	{
		 if(count > 0 && count >= cnt) //超过最大重复次数
     {
            count = 0;      //次数清零
            cnt = 1;        //最大次数复位
            len = sign + 20; //计算错误代码
            sign = 0;      //流程清零
            return len;     //返回错误码
     }
		 else
		 {	
					count++;
					switch(sign)
					{
						case 0:
						{
							  g_tcpTime = 200;          //超时时间ms
                cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
								printf("Connect to WIFI %s\r\n",User_ESP8266_PWD);
								ESP8266_AT_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD, res_at);
						}
						break;
						case 1:
						{
								g_tcpTime = 200;          	//超时时间ms
								cnt = 5;   //重复检查次数,*air208_Tim后时总体时间
                ESP8266_Enable_MultipleId(DISABLE,res_at);
						}
						break;
						case 2:
						{
								cnt = 3;
								g_tcpTime  = 100*5;
								printf("Connect to Server %s\r\n",TCPServer_IP);		
								ESP8266_Link_Server(enumTCP,TCPServer_IP,TCPServer_PORT,Single_ID_0,res_at);
						}
						break;
						default:// 状态配置执行完毕
            {
                    count = 0;      //重试次数清零
                    sign = 0;      //流程清零
                    cnt = 1;        //最大次数复位
                    return 1;       //返回配置完成
            }
					}
			}

		}
		else
		{
				if(USART2_Revice(COMMAND,response))         //从串口3读取数据
        {
            if(strstr((const char *)response, (const char *)res_at) != NULL) //检查是否包含关键字
            {
                g_tcpTime = 0; //定时清零
                count = 0;      //重试次数清零
                sign++;        //下一个流程
            }else if (strstr((const char *)response, "ALREADY CONNECT") != NULL){
								g_tcpTime = 0; //定时清零
                count = 0;      //重试次数清零
                sign++;        //下一个流程
						}else if(strstr((const char *)response, "ERROR") != NULL)
						{
								g_tcpTime = 0; //定时清零
                count = 0;      //重试次数清零
                sign=0;        //流程复位
                return 32;     //返回错误码
						}
        } 
		}		
	return 0;
}

void TCP_Program(void)
{
		u8 errCode = 0;
		static u8 cnt = 0;
		switch(g_state)
		{
			case 0:
			{
				if(TCP_Restart_Program()){
						g_state++;
						cnt=0;
				}
			}
			break;
			case 1:
			{
				errCode = TCP_InAT();
				switch(errCode)
				{
					case 1:
					{
						// 进入AT成功,跳到下一个
						g_state++;
						cnt=0;
					}
					break;
					case 2:
					{
						// 进入失败,重启模块
						cnt = 0;
						TCP_Restart();
					}
					break;
					default:
						break;
				}
			}
			break;
			case 2: // 模块初始化
			{
					errCode = TCP_config();
					switch(errCode)
					{
						case 0:break;
						case 1:
						{
							cnt = 0;
							g_state++;
						}
						break;
						default:
						{
							TCP_Restart();
						}
						break;
					}
			}
			break;
			case 3: // 获取GPS定位信息
			{
				errCode = TCP_gprs();
				switch(errCode)
				{
						case 0:break;
						default:
            {
				//无论是否成功获取都跳到下一个流程
                cnt=0;
                g_state++;
            }
            break;
				}
			}
			break;
			case 4: // 创建连接
			{
					errCode = TCP_Connect();
					switch(errCode)
					{
						case 0:break;
						case 1:
						{
								cnt= 0;
								esp8266_conn_flag = 1;// 连接成功
								esp8266_reg_flag = 0;
								g_state+=10; 
						}
						break;
						default:
						{
								if(cnt >= 3){
									cnt = 0;
									TCP_Restart();
								} else {
									cnt++;
									g_state++;
								}
						}
						break;
					}
			}
			break;
			case 5: // 关闭连接
			{
				if(ESP8266_tcp_off())g_state--;
			}
			break;
			default:  //TCP连接成功，开始数据收发
			{
				if(esp8266_send_return&&g_send_return_tim<=0)
				{
					esp8266_send_return=0;
					esp8266_conn_flag=0;
					esp8266_reg_flag=0;
				}
				if(!esp8266_conn_flag)g_state=5;
			}
			break;
		}
}

BOOL TCP_Send_Data(char *data,  uint16_t len)
{
		if(!esp8266_conn_flag) return FALSE;
		u2_printf("AT+CIPSEND=%d\r\n",len);
		delay_ms(10);
		USART2_Send(data,len);
		esp8266_send_return=1;
		g_send_return_tim = 30000;
		return TRUE;
}
