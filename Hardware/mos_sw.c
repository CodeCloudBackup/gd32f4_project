#include "mos_sw.h"

void OutDirveInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOFʱ��
  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
}
