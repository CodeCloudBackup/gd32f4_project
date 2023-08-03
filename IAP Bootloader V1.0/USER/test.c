#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"   
#include "gdflash.h" 
#include "iap.h"   
//WKS GD32F427ZGT6核心板 实验37
//串口IAP 实验-IAP Bootloader V1.0 

 
int main(void)
{      
	u8 t;
	u8 key;
	u16 oldcount=0;	//老的串口接收数据值
	u32 applenth=0;	//接收到的app代码长度
	
//	GD32_Clock_Init(336,8,2,7);//设置时钟,168Mhz 
//	delay_init(168);			//延时初始化  
//	uart_init(84,460800);		//初始化串口波特率为460800 
	GD32_Clock_Init(400,25,2,9);//设置时钟,168Mhz 
  delay_init(200);			//延时初始化  
	uart_init(100,115200);		//初始化串口波特率为460800 
	LED_Init();					//初始化LED 

	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0;
			t=0;
		}	  	 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//WK_UP按键按下
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					
					printf("固件更新完成!\r\n");	
				}else 
				{ 
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
			}						 
		}
		if(key==KEY0_PRES)	//KEY0按下
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");
			}									   
		}  		
	}  
}
















