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
//定时器3中断服务程序	 
void TIMER1_IRQHandler(void)
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
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位   	  
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
	
	NVIC_InitStructure.NVIC_IRQChannel=TIMER1_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);							 
	
}

//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_TIM3,ENABLE);  	//TIM14时钟使能    
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOD, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;           //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //初始化PD12
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	//PWM模式1,CCR1越大，低电平越多
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM14
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
	TIM_SetCompare1(TIM3,led0pwmval);	//修改比较值，修改占空比	
}
