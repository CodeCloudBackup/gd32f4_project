/***************************************************************************//**
  文件: main.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20220401
	平台:MINI-F407VET6

*******************************************************************************/
#include "program.h"

long GyroData[3] = {0,0,0};//单位mdps
long AccelData[3] = {0,0,0};
u8 IntFlag;//MPU6050中断标志
u8 LedFlag=0;


int main(void)
{
		static u8 led_flag = 0;
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
		Program_Init();
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
				u32 appVersion = 0,flashVersion = 0;
				MPU6050ReadAcc2Real(AccelData);//读取加速度数据	
				MPU6050ReadGyro2Real(GyroData);//读取陀螺仪数据	
				printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				//Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
				//TCP_Send_Data(tcp_data,  strlen(tcp_data));
				printf("curVersion:0x%x\n",appVersion);
				U32ToU8Array(ReadBuff, flashVersion);
				printf("flashVersion:%x\n",flashVersion);
				if (esp8266_conn_flag)
				{
					GetIAPBin();
					if(USART5_Revice(buf)){	
							u16 buf_size = sizeof(buf);
							if(Handle_Http_Download_Header(buf, buf_size,&resp_code, &isstrem, &cont_len))
							{
								printf("resp_code:%d,cont_len:%d\n",resp_code,cont_len);
							}
						
					}
				}
					
			}
				 
		}		
 
}
