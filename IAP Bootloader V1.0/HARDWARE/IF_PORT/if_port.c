/****************************************************
* Description      : ��ͬ��������ʽ
****************************************************/


#include "usart.h" 
#include "if_port.h"



void Delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12;        //�Լ����� 
      while(i--);    
   }
}

void Delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;    //�Լ�����
      while(i--);    
   }
}



// -------------------------------------------------------- FSMC�������� -------------------------------------------------------------

#if GD32_FSMC_16
	  
// ��ʼ��FSMC
void FSMC_Init_16(void)
{ 					
  RCC->AHB1ENR|=0XF<<3;    	//ʹ��PD,PE,PF,PGʱ��  
	RCC->AHB1ENR|=1<<1;     	//ʹ��PBʱ��  
	RCC->AHB3ENR|=1<<0;     	//ʹ��FSMCʱ��  
	GPIO_Set(GPIOB,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);							//PB15 �������,���Ʊ���
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
	 
	//�Ĵ�������
	//bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����
	//��������ʹ��NE4��Ҳ�Ͷ�ӦBTCR[6],[7]��				    
	FSMC_Bank1->BTCR[6]=0X00000000;
	FSMC_Bank1->BTCR[7]=0X00000000;
	FSMC_Bank1E->BWTR[6]=0X00000000;
	//����BCR�Ĵ���	ʹ���첽ģʽ
	FSMC_Bank1->BTCR[6]|=1<<12;		//�洢��дʹ��
	FSMC_Bank1->BTCR[6]|=1<<14;		//��дʹ�ò�ͬ��ʱ��
	FSMC_Bank1->BTCR[6]|=1<<4; 		//�洢�����ݿ��Ϊ16bit 	    
	//����BTR�Ĵ���	
	//��ʱ����ƼĴ��� 							    
	FSMC_Bank1->BTCR[7]|=0<<28;		//ģʽA 	 						  	 
	FSMC_Bank1->BTCR[7]|=15<<0; 	//��ַ����ʱ��(ADDSET)Ϊ15��HCLK 1/168M=6ns*15=90ns	
	FSMC_Bank1->BTCR[7]|=60<<8;  	//���ݱ���ʱ��(DATAST)Ϊ60��HCLK	=6*60=360ns
	//дʱ����ƼĴ���  
	FSMC_Bank1E->BWTR[6]|=0<<28; 	//ģʽA 	 							    
	FSMC_Bank1E->BWTR[6]|=8<<0;	//��ַ����ʱ��(ADDSET)Ϊ9��HCLK=54ns
 	//9��HCLK��HCLK=168M��  	 
	FSMC_Bank1E->BWTR[6]|=10<<8; 	//���ݱ���ʱ��(DATAST)Ϊ6ns*10��HCLK=60ns
	//ʹ��BANK1,����4
	FSMC_Bank1->BTCR[6]|=1<<0;		//ʹ��BANK1������4	    
  
  
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

// -------------------------------------------------------- SPI���� -------------------------------------------------------------

#if GD32_SPI

void SPI2_Init(void)
{
	u16 tempreg=0;
	RCC->AHB1ENR|=1<<1;    	//ʹ��PORTBʱ��	   
	RCC->AHB1ENR|=1<<2;    	//ʹ��PORTCʱ��	
	RCC->APB1ENR|=1<<14;   	//SPI2ʱ��ʹ�� 
	GPIO_Set(GPIOC,PIN1,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PC1,SPI2_CS
	GPIO_Set(GPIOC,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							  //PC2,SPI2_MISO
	GPIO_Set(GPIOC,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							  //PC3,SPI2_MOSI
	GPIO_Set(GPIOB,PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);							//PB10,SPI2_SCK
  GPIO_AF_Set(GPIOB,10,5);	//PB10,AF5
 	GPIO_AF_Set(GPIOC,2,5);	  //PC2,AF5
 	GPIO_AF_Set(GPIOC,3,5);	  //PC3,AF5 
	GPIOC->ODR|=0X7<<1;
  GPIOB->ODR|=0X1<<10;
	//SPI2��ʼ��
	RCC->APB1RSTR|=1<<14;	  //��λSPI2
	RCC->APB1RSTR&=~(1<<14);//ֹͣ��λSPI2
	tempreg|=0<<10;			//ȫ˫��ģʽ	
	tempreg|=1<<9;			//���nss����
	tempreg|=1<<8;			 
	tempreg|=1<<2;			//SPI����  
	tempreg|=0<<11;			//8λ���ݸ�ʽ	
	tempreg|=1<<1;			//����ģʽ��SCKΪ1 CPOL=1 
	tempreg|=1<<0;			//���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1  
	tempreg|=0<<3;			//Fsck=Fpclk1/2
	tempreg|=0<<7;			//MSB First  
	tempreg|=1<<6;			//SPI���� 
	SPI2->CR1=tempreg; 		//����CR1  
	SPI2->I2SCFGR&=~(1<<11);//ѡ��SPIģʽ
	SPI2_ReadWriteByte(0xff);//��������		
		
}

u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;			
	while((SPI2->SR&1<<1)==0)   //�ȴ���������	
	{
		retry++;
		if(retry>0XFFFE)return 0; //��ʱ�˳�
	}			  
	SPI2->DR=TxData;	 	        //����һ��byte 
	retry=0;
	while((SPI2->SR&1<<0)==0)   //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>0XFFFE)return 0; //��ʱ�˳�
	}	  						    
	return SPI2->DR;            //�����յ�������				    
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












