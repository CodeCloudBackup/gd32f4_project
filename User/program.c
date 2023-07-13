#include "program.h"

static IC_INFO g_icInfo;
void IC_Informatin(void)
{
	g_icInfo.flash_size = *(u16 *)(0x1FFF7A22);
	g_icInfo.IC_ID[0] = *(u32 *)(0x1FFF7A10);
	g_icInfo.IC_ID[1] = *(u32 *)(0x1FFF7A14);
	g_icInfo.IC_ID[2] = *(u32 *)(0x1FFF7A18);
}

#define FLASH_APP_INFO 0x0800C000

APP_INFO g_appInfo;

void App_Info_Init(void)
{
	g_appInfo.App_Version = 0x12345678;
	g_appInfo.App_Size = 0;
	
}

#define CLI() __set_PRIMASK(1)//�ر����ж�  
#define SEI() __set_PRIMASK(0)//�����ж�
void Program_Init(void)
{
		uint32_t FlashJedecid,FlashDeviceid;//FLASH ID����
		SEI();
	  systick_config();//����ϵͳ��Ƶ168M,�ⲿ8M����,������#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
		nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x10000);//�ж�������ַƫ��0x4000
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
		MPU6050_Init();//MPU6050��ʼ�����ã��ж�ʹ������
		while(MPU6050ReadID() == 0)//��ȡMPU6050 ID
		{	
		}
		// 
		ESP8266_Init();
		SPI_Init();
		App_Info_Init();
		FlashDeviceid=SFLASH_ReadID();//��ȡDevice ID
		Flash_WriteSR(0x42);//�������
		// 
		GDFLASH_Write(FLASH_APP_INFO, (u32*)&g_appInfo, sizeof(g_appInfo)/sizeof(uint32_t));
		delay_1ms(100);
}

