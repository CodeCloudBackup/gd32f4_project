#include "spi.h"
void SPI_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		spi_parameter_struct spi_init_struct;
		RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
		RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//使能GPIOB时钟
		RCU_APB1PeriphClockCmd(RCU_APB1Periph_SPI1, ENABLE);//使能SPI1时钟
		 //GPIOFB3,4,5?????
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB3~5??????	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
		GPIO_Init(GPIOB, &GPIO_InitStructure);//???
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI1); //PB3??? SPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI1); //PB4??? SPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI1); //PB5??? SPI1
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
		GPIO_Init(GPIOF, &GPIO_InitStructure);//???
		GPIO_SetBits(GPIOF,GPIO_Pin_5 | GPIO_Pin_6);
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI1_BASE);
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_128;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1_BASE, &spi_init_struct);
    spi_enable(SPI1_BASE);	
} 			 
 
  //SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		 			 
	while(RESET == spi_i2s_flag_get(SPI1_BASE, SPI_FLAG_TBE));
	spi_i2s_data_transmit(SPI1_BASE,TxData);
	while(RESET == spi_i2s_flag_get(SPI1_BASE, SPI_FLAG_RBNE));
  return spi_i2s_data_receive(SPI1_BASE);		    
}	
 		    

