
#include "f35sqa512m.h"
#include "spi.h"

u16 F35SQA_TYPE=F35SQA512M;	//默认是W25Q128

void F35SQA_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;// CS PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	F35SQA_CS=1;			//SPI FLASH不选中
	SPI1_Init();		   			//初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);		//设置为25M时钟,高速模式 
	F35SQA_TYPE=F35SQA_ReadID();	//读取FLASH ID.
}

//读取芯片ID
//返回值如下:	
u16  F35SQA_ReadID(void)
{
	u16 Temp = 0;	  
	F35SQA_CS=0;				    
	SPI1_ReadWriteByte(0x90);//发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	F35SQA_CS=1;				    
	return Temp;
}