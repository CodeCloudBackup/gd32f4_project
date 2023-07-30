#include "spi.h"

void SPI_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		spi_parameter_struct spi_init_struct;
		RCU->AHB1EN|=1<<2;
		RCU->AHB1EN|=1<<3;
		RCU->APB1EN|=1<<15;//SPI2
		 //GPIOFB3,4,5?????
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;//PB3~5??????	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
		GPIO_Init(GPIOC, &GPIO_InitStructure);//???
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF6_SPI2); //PB3??? SPI1
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF6_SPI2); //PB4??? SPI1
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF6_SPI2); //PB5??? SPI1
	
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI2_BASE);
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_128;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI2_BASE, &spi_init_struct);
    spi_enable(SPI2_BASE);	
}
