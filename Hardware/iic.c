#include "iic.h"

#define I2C0_SLAVE_ADDRESS7    0x46

void IIC_Config(uint32_t iic_periph)
{
	
    /* configure I2C1 clock */
//    i2c_clock_config(iic_periph, 100000, I2C_DTCY_2);
//    /* configure I2C1 address */
//    i2c_mode_addr_config(iic_periph, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
//    /* enable I2C1 */
//    i2c_enable(iic_periph);
//    /* enable acknowledge */
//    i2c_ack_config(iic_periph, I2C_ACK_ENABLE);
}


void IIC0_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	I2C_InitTypeDef   I2C_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	 //GPIOB8,B9?????
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//??????
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
  GPIO_Init(GPIOB, &GPIO_InitStructure);//???
	//
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C0); //GPIOB10???IIC
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C0); //GPIOB11???IIC
	
	//////////////////////////??????//////////////////////////////////
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_I2C0,ENABLE);//使能USART1时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1= I2C0_SLAVE_ADDRESS7;
	I2C_Init(I2C0, &I2C_InitStructure);
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = EXTI5_9_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
		/* configure key EXTI line */
//	exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_RISING);//?????
//	exti_interrupt_flag_clear(EXTI_5);
	delay_ms(180);//??180ms
}

void IIC1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU->AHB1EN|=1<<1;//??GPIOB??
	 //GPIOB8,B9?????
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
  GPIO_Init(GPIOB, &GPIO_InitStructure);//???
	//??1????????
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C1); //GPIOB10???USART2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C1); //GPIOB11???USART2
	
	RCU->APB1EN|=1<<22;//I2C1????
	//IIC_Config(I2C1);
	//////////////////////////??????//////////////////////////////////
	delay_ms(180);//??180ms
}
