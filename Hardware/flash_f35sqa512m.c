#include "flash_f35sqa512m.h"

void FLASH_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	F35SQA512M_CS=1;
	SPI_Init();
	
}
