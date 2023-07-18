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
	RCU->AHB1EN|=1<<1;//使能GPIOB时钟
	gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_6);//复用功能4
	gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_7);//复用功能4
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_6);//PB6,PB7配置成I2C
	gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
  gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_7);
  gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	
	RCU->APB1EN|=1<<21;//I2C0时钟使能
	IIC_Config(I2C0);
	//////////////////////////外部中断配置//////////////////////////////////
	RCU->AHB1EN|=1<<0;//使能GPIOB时钟
	gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_5);//PA5配置成下拉输入
	nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);//中断使能
		/* configure key EXTI line */
	exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_RISING);//上升沿触发
	exti_interrupt_flag_clear(EXTI_5);
	delay_1ms(180);//等待180ms
}

void IIC1_Init(void)
{
	RCU->AHB1EN|=1<<1;//使能GPIOB时钟
	gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_0);//复用功能4
	gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_1);//复用功能4
	gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_0);//PB6,PB7配置成I2C
	gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
  gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_1);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
	
	
	RCU->APB1EN|=1<<22;//I2C1时钟使能
	IIC_Config(I2C1);
	//////////////////////////外部中断配置//////////////////////////////////
	delay_ms(180);//等待180ms
}
