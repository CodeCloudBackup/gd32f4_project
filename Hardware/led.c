#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU->AHB1EN|=1<<1;//ʹ��GPIOBʱ��	
	
	 //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4/6   ���ù������
  GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT; //���ù������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
}



