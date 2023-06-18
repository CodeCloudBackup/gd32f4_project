/***************************************************************************//**
  文件: main.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20220401
	平台:MINI-F407VET6

*******************************************************************************/
#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
#include "systick.h"
#include "oled.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "esp8266.h"
#include "tcp.h"
#include "nand_flash.h"
#include "spi.h"

long GyroData[3] = {0,0,0};//单位mdps
long AccelData[3] = {0,0,0};
uint8_t IntFlag;//MPU6050中断标志
uint8_t LedFlag=0;
int main(void)
{
		static uint8_t led_flag = 0;
		char *tcp_data = "tcp test";
		uint8_t ReadBuff[10];
    systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		LED_Init();
		IIC0_Init();
		TIMER1_Init();
		TIMER2_Init();
		timer_disable(TIMER2);
		USART5_RX_STA = 0;
		USART_Init(USART0,115200);
		USART_Init(USART5,115200);
		MPU6050_Init();//MPU6050初始化配置，中断使能配置
		while(MPU6050ReadID() == 0)//读取MPU6050 ID
		{	
		}
		ESP8266_Init();
		SPI_Init();
		while(1)
		{
			
			TCP_Program();
			MPU6050ReadAcc2Real(AccelData);//读取加速度数据	
			MPU6050ReadGyro2Real(GyroData);//读取陀螺仪数据	
			if(TIMER1_50ms())
			{
			}
			// ESP8266_STA_TCPClient_Test();//测试TCP通讯
			if(TIMER1_100ms()) 
			{
				led_flag ^= 1;
				if(led_flag)
					LED_OFF();
				else
					LED_ON();
			}
			if(TIMER1_200ms()) 
			{
				Flash_RD_Test(ReadBuff);
			}
			if(TIMER1_1000ms())
			{
				printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				TCP_Send_Data(tcp_data,  strlen(tcp_data));
				printf("flash read buff: %s",ReadBuff);
			}
				 
		}		
 
}
