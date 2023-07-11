/***************************************************************************//**
  �ļ�: main.c
  ����: Zhengyu https://gzwelink.taobao.com
  �汾: V1.0.0
  ʱ��: 20220401
	ƽ̨:MINI-F407VET6

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

long GyroData[3] = {0,0,0};//��λmdps
long AccelData[3] = {0,0,0};
u8 IntFlag;//MPU6050�жϱ�־
u8 LedFlag=0;
#define Flash_App_Info 0x0800C000

#define CLI() __set_PRIMASK(1)//�ر����ж�  
#define SEI() __set_PRIMASK(0)//�����ж�
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
		char buf[2048];
		// http header info
		u8 resp_code, isstrem;
		u32 cont_len;
		// bin file info
		u16 applenth=0;	//���յ���app���볤��
		u8 versionBuff[4];
		u8 sizeBuff[4];
		uint32_t FlashJedecid,FlashDeviceid;//FLASH ID����
		u32 appVersion = 0x12345678;
		SEI();
	  systick_config();//����ϵͳ��Ƶ168M,�ⲿ8M����,������#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x10008);//�ж�������ַƫ��0x4000
		LED_Init();
		IIC0_Init();
		TIMER1_Init();
		TIMER2_Init();
		timer_disable(TIMER2);
		USART5_RX_STA = 0;
		USART_Init(USART0,115200);
		USART_Init(USART5,115200);
		MPU6050_Init();//MPU6050��ʼ�����ã��ж�ʹ������
		while(MPU6050ReadID() == 0)//��ȡMPU6050 ID
		{	
		}
		ESP8266_Init();
		SPI_Init();
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
		FlashDeviceid=SFLASH_ReadID();//��ȡDevice ID
		Flash_WriteSR(0x42);//�������
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
					Flash_ReadSR();//��״̬�Ĵ���
					memcpy(USART0_RX_BUF,versionBuff, 4);
					memcpy(USART0_RX_BUF+4,sizeBuff, 4);
					Flash_WriteSomeBytes(USART0_RX_BUF,0,applenth+8);//��WriteBuff�����е�����д��FLASH 0��ַ
					Flash_ReadSomeBytes(ReadBuff,0,8);//��FLASH 0��ַ��ȡ8�ֽ����ݷ���ReadBuff����
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
				MPU6050ReadAcc2Real(AccelData);//��ȡ���ٶ�����	
				MPU6050ReadGyro2Real(GyroData);//��ȡ����������	
				printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				//Flash_ReadSomeBytes(ReadBuff,0,8);//��FLASH 0��ַ��ȡ8�ֽ����ݷ���ReadBuff����
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
