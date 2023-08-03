#include "key.h"
#include "delay.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6核心板
//按键输入驱动代码	   
//版本：V1.0							  
////////////////////////////////////////////////////////////////////////////////// 	 

//按键初始化函数
void KEY_Init(void)
{
	RCC->AHB1ENR|=1<<0;     //使能PORTA时钟 
	RCC->AHB1ENR|=1<<4;     //使能PORTE时钟
	GPIO_Set(GPIOE,PIN4,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);			//PE4设置下拉输入
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_IN,0,0,GPIO_PUPD_PD); 		//PA0设置为下拉输入 
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//4，KEY_UP按下 即WK_UP
//注意此函数有响应优先级,KEY0>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==1||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==1)return KEY0_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==0&&WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}




















