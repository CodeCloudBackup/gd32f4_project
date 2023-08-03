#include "timer.h"
#include "led.h"
static u32 g_timCnt = 0;
u8 g_timFlag = 0;

void TIM_1msProgram(void)
{

}

void TIM_10msProgram(void)
{

}

void TIM_100msProgram(void)
{

}

void TIM_1000msProgram(void)
{
	LED_R=!LED_R;
}
//��ʱ��3�жϷ������	 
void TIM1_IRQHandler(void)
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
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ   	  
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
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);							 
	
}

