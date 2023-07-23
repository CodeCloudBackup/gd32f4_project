#include "iic.h"

#define I2C0_SLAVE_ADDRESS7    0x46

void IIC_Config(uint32_t iic_periph)
{
    /* configure I2C1 clock */
    i2c_clock_config(iic_periph, 100000, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(iic_periph, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(iic_periph);
    /* enable acknowledge */
    i2c_ack_config(iic_periph, I2C_ACK_ENABLE);
}


void IIC0_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU->AHB1EN|=1<<1;//使能GPIOB时钟

		gpio_af_set(GPIOB_BASE, GPIO_AF_4, GPIO_PIN_6);//复用功能4
		gpio_af_set(GPIOB_BASE, GPIO_AF_4, GPIO_PIN_7);//复用功能4
	  gpio_mode_set(GPIOB_BASE, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_6);//PB6,PB7配置成I2C
    gpio_output_options_set(GPIOB_BASE, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_mode_set(GPIOB_BASE, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_7);
    gpio_output_options_set(GPIOB_BASE, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	 //GPIOB8,B9初始化设置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
//	//串口1对应引脚复用映射
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_4); //GPIOB10复用为IIC
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_4); //GPIOB11复用为IIC
	
	RCU->APB1EN|=1<<21;//I2C0时钟使能
	IIC_Config(I2C0);
	//////////////////////////外部中断配置//////////////////////////////////
	RCU->AHB1EN|=1<<0;//使能GPIOA时钟
	
	gpio_mode_set(GPIOA_BASE, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_5);//PA5配置成下拉输入

	nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);//中断使能
		/* configure key EXTI line */
	exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_RISING);//上升沿触发
	exti_interrupt_flag_clear(EXTI_5);
	delay_ms(180);//等待180ms
}

void IIC1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU->AHB1EN|=1<<1;//使能GPIOB时钟
	 //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_4); //GPIOB10复用为USART2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_4); //GPIOB11复用为USART2
	
	RCU->APB1EN|=1<<22;//I2C1时钟使能
	IIC_Config(I2C1);
	//////////////////////////外部中断配置//////////////////////////////////
	delay_ms(180);//等待180ms
}

