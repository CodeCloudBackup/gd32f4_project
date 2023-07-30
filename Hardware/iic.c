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
	RCU->AHB1EN|=1<<1;//ʹ��GPIOBʱ��
	 //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C0); //GPIOB10����ΪIIC
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C0); //GPIOB11����ΪIIC
	
	RCU->APB1EN|=1<<21;//I2C0ʱ��ʹ��
	IIC_Config(I2C0);
	//////////////////////////�ⲿ�ж�����//////////////////////////////////
	RCU->AHB1EN|=1<<0;//ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	//gpio_mode_set(GPIOA_BASE, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_5);//PA5���ó���������

	nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);//�ж�ʹ��
		/* configure key EXTI line */
	exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_RISING);//�����ش���
	exti_interrupt_flag_clear(EXTI_5);
	delay_ms(180);//�ȴ�180ms
}

void IIC1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU->AHB1EN|=1<<1;//ʹ��GPIOBʱ��
	 //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C1); //GPIOB10����ΪUSART2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C1); //GPIOB11����ΪUSART2
	
	RCU->APB1EN|=1<<22;//I2C1ʱ��ʹ��
	IIC_Config(I2C1);
	//////////////////////////�ⲿ�ж�����//////////////////////////////////
	delay_ms(180);//�ȴ�180ms
}

