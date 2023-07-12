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
		char buf[2048];
		// http header info
		u8 resp_code, isstrem;
		u32 cont_len;
		// bin file info
		u16 applenth=0;	//���յ���app���볤��
		u8 versionBuff[4];
		u8 sizeBuff[4];
		uint32_t FlashJedecid,FlashDeviceid;//FLASH ID����
		Program_Init();
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
				u32 appVersion = 0,flashVersion = 0;
				MPU6050ReadAcc2Real(AccelData);//��ȡ���ٶ�����	
				MPU6050ReadGyro2Real(GyroData);//��ȡ����������	
				printf("x:%ld, y:%ld, z:%ld\n",GyroData[0],GyroData[1],GyroData[2]);
				//Flash_ReadSomeBytes(ReadBuff,0,8);//��FLASH 0��ַ��ȡ8�ֽ����ݷ���ReadBuff����
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
