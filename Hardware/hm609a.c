#include "hm609a.h"
#include "delay.h"
#include "usart.h"

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
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // Fly Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //high:mcu sleep state
	 //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化 

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
*模块进入AT模式
*进入成功返回1,执行中返回0,进入失败返回10
*/
u8 air208_InAt(void)//模块进入AT模式
{
    static uint8_t count = 0, Signs = 0; //重复次数,重启流程
    char buf[200];
    switch (Signs)//AIR208状态处理
    {
        case 0: //延时1.5s
        {
            count = 0;      //重试次数清零
            Signs++;        //进入下一个流程
            g_hm609aTim =1500;           //延时时间ms
            USART1_Clear();             //清除USART3接收缓存
        }
        break;
        case 1: //检查是否在AT模式
        {
            if(g_hm609aTim == 0) //发送测试数据+++
            {
                if(count>=2)
                {
                    count = 0;      //重试次数清零
                    Signs=0;        //进入下一个流程
                }
                else
                {
                    printf("+++\r\n测试AT命令通信是否正常%d\r\n",count+1);  //调试回显 ,正常是可回避
                    g_hm609aTim =1500;           //检测超时时间ms
                    count++;                    //重试次数+1
                    u1_printf("+++");    //发送AT测试指令
                }
            }
        }
        break;
        case 2: //发送AT
        {
            if(g_hm609aTim == 0) //测试AT是否正常
            {
                if(count >= 5)
                {
                    //超时未检测到AT指令
                    count = 0;      //重试次数清零
                    Signs = 0 ;     //进入下一个流程
                    return 10;  
                }
                else
                {
                   printf("AT\r\n进入AT模式%d次\r\n", count + 1); //测试AT是否正常
                    g_hm609aTim = 1500;      //检测超时时间ms
                    count++;                //重试次数+1
                    u1_printf("+++");//发送AT测试指令
                }
            }
            else
            {
                if(USART1_Revice(buf))         //从串口3读取数据
                {
										printf("Rev:%s\n",buf);
                    if(strstr((const char *)buf, "OK") != NULL) //检查是否包含关键字
                    {
                        g_hm609aTim = 0; //定时清零
                        count = 0;      //重试次数清零
                        Signs = 0;      //流程清零
                        return 1;       //返回进入AT成功
                    }
                }
            }
        }
        break;
        default://进入AT成功
        {
            count = 0;      //重试次数清零
            Signs = 0;      //流程清零
            return 10;      //返回进入流程失败
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
				switch (Signs)//AIR208状态处理
        {
					case 0: //取消核心板回显功能
          {
						printf("A|ATE0\r\n");
						g_hm609aTim = 2000;          //超时时间ms
						cnt = 1;   //重复检查次数,*air208_Tim后时总体时间
						strcpy(res_at,"OK"); 
						u1_printf("ATS0?");  //发送AT指令
					}
					break;
					case 1:  // 查询核心板是否读到卡
          {
						 printf("A|AT+CPIN?\r\n");
             g_hm609aTim = 2000;				//超时时间ms
             cnt = 30;   //重复检查次数,*air208_Tim后时总体时间
             strcpy(res_at, "+CPIN: READY");	//设置返回判断关键字
             u1_printf("\r\nAT+CPIN?\r\n");  //发送AT指令
					}
					break;
					case 3: // 查询网络注册情况，核心板会自动注册网络，上电到注册大概 10s 左右
          {
							printf("A|AT+CGREG?\r\n");
							g_hm609aTim = 2000;				//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "+CGREG:0,1");		//设置返回判断关键字
							u1_printf("\r\nAT+CGREG?\r\n"); //发送AT指令
          }
					case 4: // 查询是否附着网络
					{
							printf("A|AT+CGATT?\r\n");
							g_hm609aTim = 2000;          	//超时时间ms
							cnt = 60;   //重复检查次数,*air208_Tim后时总体时间
							strcpy(res_at, "+CGATT:1");		//设置返回判断关键字
							u1_printf("\r\nAT+CGATT?\r\n"); //发送AT指令
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
		if(USART1_Revice(buf))         //从串口3读取数据
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
void HM609A_Program(void)
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
        err = air208_InAt();
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
            //进入失败,重启模块
            count=0;
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
			err = HM609A_config();
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
               // air208_restart();
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
        if(!hm609a_connect_flag)g_hm609aState=5;//如果连接断开，执行断开TCP，重新创建TCP连接
    }break;
	}
}

void HM609A_test(void)
{
	
}
