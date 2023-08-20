
#include "f35sqa512m.h"
#include "spi.h"

u16 F35SQA_TYPE=F35SQA512M;	//Ĭ����W25Q128

void F35SQA_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;// CS PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	F35SQA_CS=1;			//SPI FLASH��ѡ��
	SPI1_Init();		   			//��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);		//����Ϊ25Mʱ��,����ģʽ 
	F35SQA_TYPE=F35SQA_ReadID();	//��ȡFLASH ID.
}

//��ȡоƬID
//����ֵ����:	
u16  F35SQA_ReadID(void)
{
	u16 Temp = 0;	  
	F35SQA_CS=0;				    
	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	F35SQA_CS=1;				    
	return Temp;
}