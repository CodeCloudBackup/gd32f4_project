#include "timer.h"
#include "tcp.h"
uint8_t TIMER1_flag = 0; 

static uint32_t time_count = 0;

extern void USART5_TIM_1ms(void);
void TIMER1_IRQHandler(void)
{	
		if(timer_interrupt_flag_get(TIMER1, TIMER_FLAG_UP)){		// ����ж�
			timer_flag_clear(TIMER1,TIMER_FLAG_UP);  //����жϱ�־λ  
			TCP_TIM_1ms();
			USART5_TIM_1ms();
			time_count++;
			if(time_count%2 == 0){
				TIMER1_flag |= 0x01;
			}
			if(time_count%5 == 0){
				TIMER1_flag |= 0x02;
				
			}
			if(time_count%10 == 0){
				TIMER1_flag |= 0x04;
			}
			
			if(time_count%20 == 0){
				TIMER1_flag |= 0x08;
			}
			
			if(time_count%100 == 0){
				TIMER1_flag |= 0x10;
			}
			if(time_count%200 == 0){
				TIMER1_flag |= 0x20;
			}
			if(time_count%1000 == 0){
				TIMER1_flag |= 0x40;
				time_count = 0;
			}
		}
		
}

BOOL TIMER1_5ms(void)
{
	if(TIMER1_flag&0x02){
		 TIMER1_flag &= (uint8_t)(~0x02);
		 return TRUE;
	}else
		 return FALSE;
}


BOOL TIMER1_10ms(void)
{
	if(TIMER1_flag&0x04){
		 TIMER1_flag &= (uint8_t)(~0x04);
		 return TRUE;
	}else
		 return FALSE;
}

BOOL TIMER1_50ms(void)
{
	if(TIMER1_flag&0x08){
		 TIMER1_flag &= (uint8_t)(~0x08);
		 return TRUE;
	}else
		 return FALSE;
}

BOOL TIMER1_100ms(void)
{
	if(TIMER1_flag&0x10){
		 TIMER1_flag &= (uint8_t)(~0x10);
		 return TRUE;
	}else
		 return FALSE;
}

BOOL TIMER1_200ms(void)
{
	if(TIMER1_flag&0x20){
		 TIMER1_flag &= (uint8_t)(~0x20);
		 return TRUE;
	}else
		 return FALSE;
}

BOOL TIMER1_1000ms(void)
{
	
	if(TIMER1_flag&0x40){
		 TIMER1_flag &= (uint8_t)(~0x40);
		 return TRUE;
	}else
		 return FALSE;
}
void TIMER1_Init(void)
{
	 /* -----------------------------------------------------------------------
	  ϵͳ��Ƶ168MHZ,timer_initpara.prescalerΪ167��timer_initpara.periodΪ999��Ƶ�ʾ�Ϊ1KHZ
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER1);
	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//AP1�������42MHZ,����TIME1��168M��Ҫ4��Ƶ
    timer_deinit(TIMER1);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 167;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
		nvic_irq_enable(TIMER1_IRQn, 0, 1);
		timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
}

static uint32_t time_cnt = 1;
void TIMER2_IRQHandler(void)
{
		if(timer_interrupt_flag_get(TIMER2, TIMER_FLAG_UP)){		//
				timer_flag_clear(TIMER2,TIMER_FLAG_UP);  //  
		}
}

void TIMER2_Init(void)
{
	 /* -----------------------------------------------------------------------
	  ϵͳ��Ƶ168MHZ,timer_initpara.prescalerΪ167��timer_initpara.periodΪ999��Ƶ�ʾ�Ϊ1KHZ
    ----------------------------------------------------------------------- */
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER2);
	  rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//AP1�������42MHZ,����TIME1��168M��Ҫ4��Ƶ
    timer_deinit(TIMER2);
    /* TIMER1 configuration */
    timer_initpara.prescaler         = 16700;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);
	  nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
		nvic_irq_enable(TIMER2_IRQn, 0, 1);
		timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* auto-reload preload enable */
    timer_enable(TIMER2);
}

