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
#include "http.h"
#include "gdflash.h"
#include "nand_flash.h"
#include "spi.h"
#include "program.h"

long GyroData[3] = {0,0,0};//单位mdps
long AccelData[3] = {0,0,0};
u8 IntFlag;//MPU6050中断标志
u8 LedFlag=0;
#define Flash_App_Info 0x0800C000


int main(void)
{
		static u8 led_flag = 0;
		char *tcp_data = "tcp test";
		u8 ReadBuff[10];
		char buf[2048];
		// http header info
		u8 resp_code, isstrem;
		u32 cont_len;
		// bin file info
		u16 applenth=0;	//接收到的app代码长度
		u8 versionBuff[4];
		u8 sizeBuff[4];
		uint32_t FlashJedecid,FlashDeviceid;//FLASH ID变量
		u32 appVersion = 0x12345678;
		Program_Init();
		appVersion=GDFLASH_ReadWord(Flash_App_Info); 
		if(appVersion == 0xFFFFFFFF)
		{
			appVersion=0x1234567a;	
		}	
		else
		{
			appVersion=0x1234567a;
		}
		GDFLASH_Write(Flash_App_Info, &appVersion, 1);
		FlashDeviceid=SFLASH_ReadID();//读取Device ID
		Flash_WriteSR(0x42);//解除保护
		delay_1ms(100);
		while(1)
		{
			
			TCP_Program();
			
			if(TIMER1_50ms())
			{
				applenth = USART0_TIM_50ms();
				if(applenth)
				{
					u32 version = 0x1234567a;
					U32ToU8Array(versionBuff,version);
					U32ToU8Array(sizeBuff, applenth);
					Flash_ReadSR();//读状态寄存器
					memcpy(USART0_RX_BUF,versionBuff, 4);
					memcpy(USART0_RX_BUF+4,sizeBuff, 4);
					Flash_WriteSomeBytes(USART0_RX_BUF,0,applenth+8);//把WriteBuff数组中的内容写入FLASH 0地址
					Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
					ReadBuff[8]= '\0';
					applenth=0;
				}
			}
			
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
				MPU6050ReadAcc2Real(AccelData);//读取加速度数据	
				MPU6050ReadGyro2Real(GyroData);//读取陀螺仪数据	
				printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				//Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
				//TCP_Send_Data(tcp_data,  strlen(tcp_data));
				printf("curVersion:0x%x\n",appVersion);
				printf("flashVersion:%s\n",ReadBuff);
				if (esp8266_conn_flag)
				{
					GetIAPBin();
					if(USART5_Revice(buf)){	
							u16 buf_size = sizeof(buf);
							if(Handle_Http_Download_Header(buf, buf_size,&resp_code, &isstrem, &cont_len))
							{
								memset(buf, 0, sizeof(buf));
								printf("resp_code:%d,cont_len:%d\n",resp_code,cont_len);
							}
						
					}
				}
					
			}
				 
		}		
 
}
