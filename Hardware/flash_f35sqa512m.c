#include "flash_f35sqa512m.h"

void FLASH_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	F35SQA512M_CS=1;
	SPI_Init();
	
}
