#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "key.h"   
#include "gdflash.h" 
#include "iap.h"   
//WKS GD32F427ZGT6核心板 实验37
//串口IAP 实验-IAP Bootloader V1.0 

typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;

#define MAX_BIN_BUFF 4*4096
u8 gBinBuff[MAX_BIN_BUFF] __attribute__ ((at(0X20001000)));

int main(void)
{      
	u16 oldcount=0;	//老的串口接收数据值
	u32 applenth=0;	//接收到的app代码长度
	BOOL softwartUpdateFlag = FALSE;
	GD32_Clock_Init(400,25,2,9);//设置时钟,168Mhz 
  delay_init(200);			//延时初始化  
	uart_init(100,115200);		//初始化串口波特率为460800 
	LED_Init();					//初始化LED 
	LEDR=0;
	delay_ms(1000);
	printf("IAP Program begin ...\n");
	while(1)
	{
			if(softwartUpdateFlag){
				printf("开始更新固件...\r\n");	
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP2_ADDR,gBinBuff,applenth);//更新FLASH代码   
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
					printf("固件更新完成!\r\n");	
				}else 
				{ 
					printf("非FLASH应用程序!\r\n");
				}
			}
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
	
	}  
}
















