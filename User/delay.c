#include "delay.h"

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数

//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为AHB时钟的1/8
//SYSCLK:系统时钟频率
void delay_init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//不论是否使用OS,fac_us都需要使用
	fac_ms=(u16)fac_us*1000;				//非OS下,代表每个ms需要的systick时钟数   
}								    

//??nus
//nus?????us?.	
//??:nus??,????798915us(????2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//????	  		 
	SysTick->VAL=0x00;        				//?????
	SysTick->CTRL=0x01 ;      				//???? 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//??????   
	SysTick->CTRL=0x00;      	 			//?????
	SysTick->VAL =0X00;       				//????? 
}

//??nms
//??nms???
//SysTick->LOAD?24????,??,?????:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK???Hz,nms???ms
//?168M???,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//????(SysTick->LOAD?24bit)
	SysTick->VAL =0x00;           			//?????
	SysTick->CTRL=0x01 ;          			//????  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//??????   
	SysTick->CTRL=0x00;       				//?????
	SysTick->VAL =0X00;     		  		//?????	  	    
} 
//??nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//???540,??????????????,
											//?????248M???,delay_xms??????541ms???
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
