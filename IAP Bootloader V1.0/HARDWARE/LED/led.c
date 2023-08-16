#include "led.h" 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6核心板
//LED驱动代码	   
//版本：V1.0				  							  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
	RCC->AHB1ENR|=1<<4;//使能PORTF时钟 
	GPIO_Set(GPIOF,PIN2|PIN3|PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF9,PF10设置
}






