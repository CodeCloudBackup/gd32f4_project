#include "spi.h"

void SPI_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCU->AHB1EN|=1<<2;//使能GPIOC时钟	
		RCU->AHB1EN|=1<<3;//使能GPIOD时钟	
		RCU->APB1EN|=1<<15;//使能SPI2时钟
	
		gpio_af_set(GPIOC_BASE, GPIO_AF_6, GPIO_PIN_10);//复用功能6
		gpio_af_set(GPIOC_BASE, GPIO_AF_6, GPIO_PIN_11);//复用功能6
		gpio_af_set(GPIOC_BASE, GPIO_AF_6, GPIO_PIN_12);//复用功能6
	  gpio_mode_set(GPIOC_BASE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12); /* SPI2 GPIO config:SCK/PC10, MISO/PC11, MOSI/PC12 */
    gpio_output_options_set(GPIOC_BASE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);
		 //GPIOFB3,4,5初始化设置
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;//PB3~5复用功能输出	
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
//		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
//	
//		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_6); //PC10复用为 SPI1
//		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_6); //PC11复用为 SPI1
//		GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_6); //PC12复用为 SPI1

	  gpio_mode_set(GPIOD_BASE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0); /* PD0 CS */
    gpio_output_options_set(GPIOD_BASE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	
		spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI2);
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_128;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI2, &spi_init_struct);
    spi_enable(SPI2);	
}
