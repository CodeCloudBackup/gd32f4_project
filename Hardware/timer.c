#include "timer.h"
#include "led.h"
#include "delay.h"
static u32 g_timCnt = 0;
u8 g_timFlag = 0;
extern u16  g_hm609aHeartBeat; 
extern u16 g_mqttHeartbeatNum;
extern void HM609A_TIM_1ms(void);
extern void  USART1_TIM_1ms(void);
void TIM_1msProgram(void)
{
	USART1_TIM_1ms();
	HM609A_TIM_1ms();
}

void TIM_10msProgram(void)
{
	
}

void TIM_100msProgram(void)
{
}

void TIM_1000msProgram(void)
{
	LED_R = !LED_R;
	g_hm609aHeartBeat++;
	g_mqttHeartbeatNum++;
}
//��ʱ��3�жϷ������	 
void TIMER1_IRQHandler(void)
{ 		  
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)//����ж� 
	{
		TIM_1msProgram();
		g_timCnt++;
		if(g_timCnt%2 == 0) //2ms
		{
				g_timFlag |= 0x01;
		}
		if(g_timCnt%5 == 0) //5ms
		{
				g_timFlag |= 0x02;
		}
		if(g_timCnt%10 == 0) //10ms
		{
				TIM_10msProgram();
				g_timFlag |= 0x04;
		}
		if(g_timCnt%20 == 0) //20ms
		{
				g_timFlag |= 0x08;
		}
		if(g_timCnt%100 == 0) //100ms
		{
				TIM_100msProgram();
				g_timFlag |= 0x10;
		}
		if(g_timCnt%200 == 0) //200ms
		{
				g_timFlag |= 0x20;
		}
		if(g_timCnt%1000 == 0) //1000ms
		{
				TIM_1000msProgram();
				g_timFlag |= 0x40;
				g_timFlag = 0;
		}
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ   	  
	}				    		  			    
}

//ͨ�ö�ʱ��6�жϳ�ʼ��
//RCU_CFG1 TIMERSEL  
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ50M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 

void TIM1_Init(u16 arr,u16 psc)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_TIM1,ENABLE);  ///ʹ��TIM1ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��1�����ж�
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIMER1_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);							 
	
}

//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_TIM3,ENABLE);  	//TIM14ʱ��ʹ��    
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12����Ϊ��ʱ��3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PD12
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	//PWMģʽ1,CCR1Խ�󣬵͵�ƽԽ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM14
}
	
void TIM3_Output_PWM(void)
{
	static u16 led0pwmval=0;  
	static u8 dir=1;
	delay_ms(100);
	if(dir)led0pwmval++;
	else led0pwmval--;	 
	if(led0pwmval>3000)dir=0;
	if(led0pwmval==300)dir=1;	   					 
	TIM_SetCompare1(TIM3,led0pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�	
}
