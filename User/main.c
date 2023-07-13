/***************************************************************************//**
  �ļ�: main.c
  ����: Zhengyu https://gzwelink.taobao.com
  �汾: V1.0.0
  ʱ��: 20220401
	ƽ̨:MINI-F407VET6

*******************************************************************************/
#include "program.h"

long GyroData[3] = {0,0,0};//��λmdps
long AccelData[3] = {0,0,0};
u8 IntFlag;//MPU6050�жϱ�־
u8 LedFlag=0;


int main(void)
{
		static u8 led_flag = 0;
		u8 ReadBuff[10];
		// bin file info
		u16 applenth=0;	//���յ���app���볤��
		Program_Init();
		while(1)
		{
			
			TCP_Program();
			
			if(TIMER1_50ms())
			{
				applenth = USART0_TIM_50ms();
				if(applenth)
				{
				
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
				u32 appVersion = 0,flashVersion = 0, flashSize = 0;
				MPU6050ReadAcc2Real(AccelData);//��ȡ���ٶ�����	
				MPU6050ReadGyro2Real(GyroData);//��ȡ����������	
				printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				printf("curVersion:0x%x\n",g_appInfo.App_Version);
				Flash_ReadSomeBytes(ReadBuff,0,8);//��FLASH 0��ַ��ȡ8�ֽ����ݷ���ReadBuff����
				flashVersion = U8ToU32(ReadBuff);
				flashSize = U8ToU32(ReadBuff+4);
				printf("flashAppVersion:0x%x\n",flashVersion);
				printf("flashAppSize:0x%x\n",flashVersion);
				
				if (esp8266_conn_flag)
				{
						Http_Program();
				}
					
			}
				 
		}		
 
}
