/****************************************************
* Description      : 不同的驱动方式
****************************************************/


#include "usart.h" 
#include "if_port.h"



void Delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12;        //自己定义 
      while(i--);    
   }
}

void Delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;    //自己定义
      while(i--);    
   }
}



// -------------------------------------------------------- FSMC总线驱动 -------------------------------------------------------------

#if GD32_FSMC_16
	  
// 初始化FSMC
void FSMC_Init_16(void)
{ 					
  RCC->AHB1ENR|=0XF<<3;    	//使能PD,PE,PF,PG时钟  
	RCC->AHB1ENR|=1<<1;     	//使能PB时钟  
	RCC->AHB3ENR|=1<<0;     	//使能FSMC时钟  
	GPIO_Set(GPIOB,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);							//PB15 推挽输出,控制背光
	GPIO_Set(GPIOD,(3<<0)|(3<<4)|(7<<8)|(3<<14),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PD0,1,4,5,8,9,10,14,15 AF OUT
	GPIO_Set(GPIOE,(0X1FF<<7),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);						//PE7~15,AF OUT
	GPIO_Set(GPIOF,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PF12,FSMC_A6
	GPIO_Set(GPIOG,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PG12,FSMC_NE4
 	GPIO_AF_Set(GPIOD,0,12);	//PD0,AF12
 	GPIO_AF_Set(GPIOD,1,12);	//PD1,AF12
 	GPIO_AF_Set(GPIOD,4,12);	//PD4,AF12
 	GPIO_AF_Set(GPIOD,5,12);	//PD5,AF12 
 	GPIO_AF_Set(GPIOD,8,12);	//PD8,AF12
 	GPIO_AF_Set(GPIOD,9,12);	//PD9,AF12
 	GPIO_AF_Set(GPIOD,10,12);	//PD10,AF12 
 	GPIO_AF_Set(GPIOD,14,12);	//PD14,AF12
 	GPIO_AF_Set(GPIOD,15,12);	//PD15,AF12
	
 	GPIO_AF_Set(GPIOE,7,12);	//PE7,AF12
 	GPIO_AF_Set(GPIOE,8,12);	//PE8,AF12
 	GPIO_AF_Set(GPIOE,9,12);	//PE9,AF12
 	GPIO_AF_Set(GPIOE,10,12);	//PE10,AF12
 	GPIO_AF_Set(GPIOE,11,12);	//PE11,AF12
 	GPIO_AF_Set(GPIOE,12,12);	//PE12,AF12
 	GPIO_AF_Set(GPIOE,13,12);	//PE13,AF12
 	GPIO_AF_Set(GPIOE,14,12);	//PE14,AF12
 	GPIO_AF_Set(GPIOE,15,12);	//PE15,AF12
	
 	GPIO_AF_Set(GPIOF,12,12);	//PF12,AF12
 	GPIO_AF_Set(GPIOG,12,12);	//PG12,AF12
	 
	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE4，也就对应BTCR[6],[7]。				    
	FSMC_Bank1->BTCR[6]=0X00000000;
	FSMC_Bank1->BTCR[7]=0X00000000;
	FSMC_Bank1E->BWTR[6]=0X00000000;
	//操作BCR寄存器	使用异步模式
	FSMC_Bank1->BTCR[6]|=1<<12;		//存储器写使能
	FSMC_Bank1->BTCR[6]|=1<<14;		//读写使用不同的时序
	FSMC_Bank1->BTCR[6]|=1<<4; 		//存储器数据宽度为16bit 	    
	//操作BTR寄存器	
	//读时序控制寄存器 							    
	FSMC_Bank1->BTCR[7]|=0<<28;		//模式A 	 						  	 
	FSMC_Bank1->BTCR[7]|=15<<0; 	//地址建立时间(ADDSET)为15个HCLK 1/168M=6ns*15=90ns	
	FSMC_Bank1->BTCR[7]|=60<<8;  	//数据保存时间(DATAST)为60个HCLK	=6*60=360ns
	//写时序控制寄存器  
	FSMC_Bank1E->BWTR[6]|=0<<28; 	//模式A 	 							    
	FSMC_Bank1E->BWTR[6]|=8<<0;	//地址建立时间(ADDSET)为9个HCLK=54ns
 	//9个HCLK（HCLK=168M）  	 
	FSMC_Bank1E->BWTR[6]|=10<<8; 	//数据保存时间(DATAST)为6ns*10个HCLK=60ns
	//使能BANK1,区域4
	FSMC_Bank1->BTCR[6]|=1<<0;		//使能BANK1，区域4	    
  
  
}

void FMSC_16_CmdWrite(u8 cmd)
{
	cmd=cmd;
	*(vu16*) (LCD_BASE0)= (cmd);
}



void FMSC_16_DataWrite(u8 data)
{
	data=data;
	*(vu16*) (LCD_BASE1)= (data);
}



void FMSC_16_DataWrite_Pixel(u16 data)
{
	data=data;
	*(vu16*) (LCD_BASE1)= (data);
}

u8 FMSC_16_StatusRead(void)
{
	u16 temp = 0;
	temp = *(vu16*)(LCD_BASE0);
	return temp;
}

u16 FMSC_16_DataRead(void)
{
	u16 temp = 0;
	temp =  *(vu16*)(LCD_BASE1);
	return temp;
}

#endif

// -------------------------------------------------------- SPI驱动 -------------------------------------------------------------

#if GD32_SPI

void SPI2_Init(void)
{
	u16 tempreg=0;
	RCC->AHB1ENR|=1<<1;    	//使能PORTB时钟	   
	RCC->AHB1ENR|=1<<2;    	//使能PORTC时钟	
	RCC->APB1ENR|=1<<14;   	//SPI2时钟使能 
	GPIO_Set(GPIOC,PIN1,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PC1,SPI2_CS
	GPIO_Set(GPIOC,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							  //PC2,SPI2_MISO
	GPIO_Set(GPIOC,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							  //PC3,SPI2_MOSI
	GPIO_Set(GPIOB,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PB10,SPI2_SCK
  GPIO_AF_Set(GPIOB,10,5);	//PB10,AF5
 	GPIO_AF_Set(GPIOC,2,5);	  //PC2,AF5
 	GPIO_AF_Set(GPIOC,3,5);	  //PC3,AF5 
	GPIOC->ODR|=0X7<<1;
  GPIOB->ODR|=0X1<<10;
	//SPI2初始化
	RCC->APB1RSTR|=1<<14;	  //复位SPI2
	RCC->APB1RSTR&=~(1<<14);//停止复位SPI2
	tempreg|=0<<10;			//全双工模式	
	tempreg|=1<<9;			//软件nss管理
	tempreg|=1<<8;			 
	tempreg|=1<<2;			//SPI主机  
	tempreg|=0<<11;			//8位数据格式	
	tempreg|=1<<1;			//空闲模式下SCK为1 CPOL=1 
	tempreg|=1<<0;			//数据采样从第2个时间边沿开始,CPHA=1  
	tempreg|=0<<3;			//Fsck=Fpclk1/2
	tempreg|=0<<7;			//MSB First  
	tempreg|=1<<6;			//SPI启动 
	SPI2->CR1=tempreg; 		//设置CR1  
	SPI2->I2SCFGR&=~(1<<11);//选择SPI模式
	SPI2_ReadWriteByte(0xff);//启动传输		
		
}

u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;			
	while((SPI2->SR&1<<1)==0)   //等待发送区空	
	{
		retry++;
		if(retry>0XFFFE)return 0; //超时退出
	}			  
	SPI2->DR=TxData;	 	        //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0)   //等待接收完一个byte  
	{
		retry++;
		if(retry>0XFFFE)return 0; //超时退出
	}	  						    
	return SPI2->DR;            //返回收到的数据				    
}


void SPI_CmdWrite(u8 cmd)
{  

	SPI_CS=0;		
	SPI2_ReadWriteByte(0x00);
	SPI2_ReadWriteByte(cmd);
	SPI_CS=1;		
}

void SPI_DataWrite(u8 data)
{ 

  SPI_CS=0;	
	SPI2_ReadWriteByte(0x80);
	SPI2_ReadWriteByte(data);
	SPI_CS=1;
}

void SPI_DataWrite_Pixel(u16 data)
{ 

  SPI_CS=0;	
	SPI2_ReadWriteByte(0x80);
	SPI2_ReadWriteByte(data);	
	SPI_CS=1;	
	
	SPI_CS=0;	
	SPI2_ReadWriteByte(0x80);
	SPI2_ReadWriteByte(data>>8);	
	SPI_CS=1;
}

u8 SPI_StatusRead(void)
{
	u8 temp = 0;	
  SPI_CS=0;	
	SPI2_ReadWriteByte(0x40);
	temp = SPI2_ReadWriteByte(0xff);
	SPI_CS=1;
	return temp;
}

u16 SPI_DataRead(void)
{
	u16 temp = 0;	
  SPI_CS=0;	
	SPI2_ReadWriteByte(0xc0);
	temp = SPI2_ReadWriteByte(0xff);
	SPI_CS=1;
	return temp;
}
#endif




//============================================================
//-----------------------------------------------------------------------------------------------------------------------------------

void LCD_CmdWrite(u8 cmd)
{
	#if GD32_FSMC_16
	FMSC_16_CmdWrite(cmd);
	#endif
	
	#if GD32_SPI
	SPI_CmdWrite(cmd);
	#endif
}

void LCD_DataWrite(u8 data)
{
	#if GD32_FSMC_16
	FMSC_16_DataWrite(data);
	#endif
	
	#if GD32_SPI
	SPI_DataWrite(data);
	#endif
}

void LCD_DataWrite_Pixel(u16 data)
{	
	#if GD32_FSMC_16
	FMSC_16_DataWrite_Pixel(data);
	#endif
	
	#if GD32_SPI
	SPI_DataWrite_Pixel(data);
	#endif
}


u8 LCD_StatusRead(void)
{
	u8 temp = 0;
		
	#if GD32_FSMC_16
	temp = FMSC_16_StatusRead();
	#endif
	
	#if GD32_SPI
	temp = SPI_StatusRead();
	#endif
	
	return temp;
}

u16 LCD_DataRead(void)
{
	u16 temp = 0;
		
	#if GD32_FSMC_16
	temp = FMSC_16_DataRead();
	#endif
	
	#if GD32_SPI
	temp = SPI_DataRead();
	#endif
	
	return temp;
}
	  
	 


void Parallel_Init(void)
{
	
	#if GD32_FSMC_16
		FSMC_Init_16();
	#endif
	
	#if GD32_SPI
		SPI2_Init();
	#endif

}



void test_LCDIO(void)	
{   	
  u8 temp1,temp2;
  temp1=LCD_StatusRead();
  printf("Status:0x%X\r\n",temp1);//0x50


  LCD_CmdWrite(0x01);
  temp2 =LCD_DataRead();
  printf("R01h:0x%X\r\n",temp2);//0xC8	
}












