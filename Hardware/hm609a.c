#include "hm609a.h"
#include "delay.h"
#include "usart.h"
void HM609A_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOF,GPIO_Pin_11);	// Pwrkey,High, HM609A,Low time >550ms on/off
	delay_ms(1000);
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	GPIO_ResetBits(GPIOF,GPIO_Pin_12); // RESET,High, HM609A,Low time >300ms reset
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // Fly Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //high:mcu sleep state
	usart2_init(115200);
}

//bool ESP8266_Check_AT_Response(const char *ret_ask)
//{
//	char buf[100];
//	if(USART2_Revice(buf)){			
//			if(strstr((const char*)buf, ret_ask) != NULL)
//			{
//					return true;
//			}
//			return false;
//	}
//	return false;
//}
