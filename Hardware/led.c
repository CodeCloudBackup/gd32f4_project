#include "led.h" 
#include "delay.h"

static u8 ledState = 0;
void LED_Tim_1000ms()
{
		ledState = !ledState;
		LED_B = ledState;
}

//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOF,GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4);//GPIOF9,F10���øߣ�����

}

void LED_PWM_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);//GPIOF9,F10���øߣ�����
}

void LED_Test(void)
{
		LED_B=!LED_B;
		delay_ms(200);
		LED_G=!LED_G;
		delay_ms(200);
}

void LED_PWM_Test(void)
{
		static u16 led0pwmval=0;    
		static u8 dir=1;
		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;	 
		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	   					 
		TIM_SetCompare1(TIM3,led0pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}