#include "program.h"

static IC_INFO ic_info;
void IC_Informatin(void)
{
	ic_info.flash_size = *(u16 *)(0x1FFF7A22);
	ic_info.IC_ID[0] = *(u32 *)(0x1FFF7A10);
	ic_info.IC_ID[1] = *(u32 *)(0x1FFF7A14);
	ic_info.IC_ID[2] = *(u32 *)(0x1FFF7A18);
}


void U32ToU8Array(u8* buf, u32 u32Val)
{
	buf[0]=((u32Val >> 24)&0xFF);
	buf[1]=((u32Val >> 16)&0xFF);
	buf[2]=((u32Val >> 8)&0xFF);
	buf[3]=(u32Val&0xFF);
}

#define Flash_App_Info 0x0800C000
static APP_INFO gAppInfo;
void Set_App_Info(void)
{
	GDFLASH_Write(Flash_App_Info, &gAppInfo.App_Version, 1);
}

#define CLI() __set_PRIMASK(1)//关闭总中断  
#define SEI() __set_PRIMASK(0)//打开总中断
void Program_Init(void)
{
		SEI();
	  systick_config();//配置系统主频168M,外部8M晶振,配置在#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x10000);//中断向量地址偏移0x4000
		IC_Informatin();
		LED_Init();
		IIC0_Init();
		TIMER1_Init();
		TIMER2_Init();
		timer_disable(TIMER2);
		// 
		my_mem_init(SRAMIN);
		USART5_RX_STA = 0;
		USART_Init(USART0,115200);
		USART_Init(USART5,115200);
		USART5_RX_BUF=mymalloc(SRAMIN, USART5_MAX_RECV_LEN);
		//
		MPU6050_Init();//MPU6050初始化配置，中断使能配置
		while(MPU6050ReadID() == 0)//读取MPU6050 ID
		{	
		}
		// 
		ESP8266_Init();
		SPI_Init();
		Set_App_Info();
}