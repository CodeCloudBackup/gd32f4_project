#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"   
#include "gdflash.h" 
#include "iap.h"   
//WKS GD32F427ZGT6���İ� ʵ��37
//����IAP ʵ��-IAP Bootloader V1.0 

typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;

#define MAX_BIN_BUFF 4*4096
u8 gBinBuff[MAX_BIN_BUFF] __attribute__ ((at(0X20001000)));

int main(void)
{      
	u16 oldcount=0;	//�ϵĴ��ڽ�������ֵ
	u32 applenth=0;	//���յ���app���볤��
	BOOL softwartUpdateFlag = FALSE;
	GD32_Clock_Init(400,25,2,9);//����ʱ��,168Mhz 
  delay_init(200);			//��ʱ��ʼ��  
	uart_init(100,115200);		//��ʼ�����ڲ�����Ϊ460800 
	LED_Init();					//��ʼ��LED 
	LEDR=0;
	delay_ms(1000);
	printf("IAP Program begin ...\n");
	while(1)
	{
			if(softwartUpdateFlag){
				printf("��ʼ���¹̼�...\r\n");	
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP2_ADDR,gBinBuff,applenth);//����FLASH����   
					LEDB=!LEDB;
					delay_ms(500);
					LEDB=!LEDB;
					delay_ms(500);
					LEDB=!LEDB;
					delay_ms(500);
					LEDR=!LEDR;
					delay_ms(500);
					LEDG=!LEDG;
					delay_ms(500);
					LEDB=0;
					printf("�̼��������!\r\n");	
				}else 
				{ 
					printf("��FLASHӦ�ó���!\r\n");
				}
			}
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}
	
	}  
}
















