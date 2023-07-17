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
#include "iap.h"
#include "usart.h"
#include "spi.h"

u32 U8toU32(const u8* buf)
{
	  u32 u32Val = 0;
		u32Val|=buf[0];
		u32Val=u32Val<<8;
		u32Val|=buf[1];
		u32Val=u32Val<<8;
		u32Val|=buf[2];
		u32Val=u32Val<<8;
		u32Val|=buf[3];
	return  u32Val;
}

typedef union 
{
	u8 u8_data[4];
	u32 u32_data;
} U8_U32;

void LED_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);//??GPIOB??
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);//PB4?????
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//PB4???????,50M??
}

void LED_ON(void)
{
	gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4?????
}
void LED_OFF(void)
{
	gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4?????
}
#define MAX_BIN_BUFF 8*4096
u8 binBuff[MAX_BIN_BUFF] __attribute__ ((at(0X20001000)));

int main(void)
{
	U8_U32 flash_version;
	U8_U32 flash_size;
	BOOL softwartUpdateFlag = FALSE;
	u32 oldVersion = 0;
  systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
	USART_Init(USART0, 115200);
	SPI_Init();
	LED_Init();
	printf("IAP Program begin ...\n");
	Flash_ReadSomeBytes(flash_version.u8_data,0,4);//从FLASH 0地址读取8字节内容放入ReadBuff数组
	Flash_ReadSomeBytes(flash_size.u8_data,4,4);//从FLASH 0地址读取8字节内容放入ReadBuff数组
	printf("0x%x\n0x%x\n",flash_version.u32_data, flash_size.u32_data);
	oldVersion = GDFLASH_ReadWord(Flash_App_Info);
	if(flash_version.u32_data > oldVersion|| oldVersion == 0xFFFFFFFF)
	{
		printf("new: %x\n old: %x\n",flash_version.u32_data, oldVersion);
		softwartUpdateFlag = TRUE;
	}
	while(1)
	{
			if(softwartUpdateFlag){
				Flash_ReadSomeBytes(binBuff,8,flash_size.u32_data);//从FLASH 0地址读取8字节内容放入ReadBuff数组
				IAP_Write_Appbin(FLASH_APP2_ADDR, binBuff, flash_size.u32_data);
				LED_ON();
				delay_1ms(500);
				LED_OFF();
				delay_1ms(500);
				LED_ON();
				delay_1ms(500);
				LED_OFF();
				delay_1ms(500);
				LED_ON();
				delay_1ms(500);
				LED_OFF();
				GDFLASH_Write(Flash_App_Info, &flash_version.u32_data, 1);
				softwartUpdateFlag=FALSE;
			}
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				IAP_Load_App(FLASH_APP1_ADDR);//执行FLASH APP代码
			}
	
	}
}
