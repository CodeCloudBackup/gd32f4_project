#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"   
#include "gdflash.h" 
#include "iap.h"   
//WKS GD32F427ZGT6���İ� ʵ��37
//����IAP ʵ��-IAP Bootloader V1.0 

 
int main(void)
{      
	u8 t;
	u8 key;
	u16 oldcount=0;	//�ϵĴ��ڽ�������ֵ
	u32 applenth=0;	//���յ���app���볤��
	
//	GD32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz 
//	delay_init(168);			//��ʱ��ʼ��  
//	uart_init(84,460800);		//��ʼ�����ڲ�����Ϊ460800 
	GD32_Clock_Init(400,25,2,9);//����ʱ��,168Mhz 
  delay_init(200);			//��ʱ��ʼ��  
	uart_init(100,115200);		//��ʼ�����ڲ�����Ϊ460800 
	LED_Init();					//��ʼ��LED 

	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
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
		if(key==WKUP_PRES)	//WK_UP��������
		{
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");	
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
					
					printf("�̼��������!\r\n");	
				}else 
				{ 
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
			}						 
		}
		if(key==KEY0_PRES)	//KEY0����
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
			}									   
		}  		
	}  
}
















