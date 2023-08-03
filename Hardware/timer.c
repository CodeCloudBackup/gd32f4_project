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
//定时器3中断服务程序	 
void TIM1_IRQHandler(void)
{ 		  
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)//溢出中断 
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
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位   	  
	}				    		  			    
}

//通用定时器6中断初始化
//RCU_CFG1 TIMERSEL  
//这里时钟选择为APB1的2倍，而APB1为50M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 

void TIM1_Init(u16 arr,u16 psc)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_TIM1,ENABLE);  ///使能TIM1时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);							 
	
}

