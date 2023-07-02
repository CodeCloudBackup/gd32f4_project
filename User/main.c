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
#include "gdflash.h"
#include "nand_flash.h"
#include "spi.h"

long GyroData[3] = {0,0,0};//单位mdps
long AccelData[3] = {0,0,0};
u8 IntFlag;//MPU6050中断标志
u8 LedFlag=0;
u32 appVersion = 0x12345678;
#define Flash_App_Info 0x0800C000

#define CLI() __set_PRIMASK(1)//关闭总中断  
#define SEI() __set_PRIMASK(0)//打开总中断
void U32ToU8Array(u8* buf, u32 u32Val)
{
	buf[0]=((u32Val >> 24)&0xFF);
	buf[1]=((u32Val >> 16)&0xFF);
	buf[2]=((u32Val >> 8)&0xFF);
	buf[3]=(u32Val&0xFF);
}
int main(void)
{
		static u8 led_flag = 0;
		char *tcp_data = "tcp test";
		u8 ReadBuff[10];
		uint16_t applenth=0;	//接收到的app代码长度
		u8 versionBuff[4];
		u8 sizeBuff[4];
		uint32_t FlashJedecid,FlashDeviceid;//FLASH ID变量
		SEI();
	  systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x10008);//中断向量地址偏移0x4000
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
		GDFLASH_Write(Flash_App_Info, &appVersion, 2);
		FlashDeviceid=SFLASH_ReadID();//读取Device ID
		Flash_WriteSR(0x42);//解除保护
		delay_1ms(100);
		while(1)
		{
			
			//TCP_Program();
			MPU6050ReadAcc2Real(AccelData);//读取加速度数据	
			MPU6050ReadGyro2Real(GyroData);//读取陀螺仪数据	
			if(TIMER1_50ms())
			{
				applenth = USART0_TIM_50ms();

			}
			if(applenth)
			{
				appVersion = 0x12345679;
				U32ToU8Array(versionBuff,appVersion);
				U32ToU8Array(sizeBuff, applenth);
				Flash_ReadSR();//读状态寄存器
				memcpy(USART0_RX_BUF,versionBuff, 4);
				memcpy(USART0_RX_BUF+4,sizeBuff, 4);
				Flash_WriteSomeBytes(USART0_RX_BUF,0,applenth+8);//把WriteBuff数组中的内容写入FLASH 0地址
				delay_1ms(100);
				Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
				ReadBuff[8]= '\0';
				applenth=0;
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
				
			}
			if(TIMER1_1000ms())
			{
				//printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				//Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
				//TCP_Send_Data(tcp_data,  strlen(tcp_data));
				printf("curVersion:0x%x\n",appVersion);
				printf("flashVersion:%s\n",ReadBuff);
				
			}
				 
		}		
 
}
