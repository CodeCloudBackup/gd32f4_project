#include "usart.h"
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(usart_flag_get(USART0,USART_FLAG_TC)==0);//循环发送,直到发送完毕 
	USART_DATA(USART0) = ((uint16_t)USART_DATA_DATA & (uint32_t)ch);  
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART0_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART0_RX_BUF[USART0_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART0_RX_STA=0;       //接收状态标记	
void USART0_IRQHandler(void)
{
	uint8_t res;
	if(usart_flag_get(USART0, USART_FLAG_RBNE)))//接收到数据
	{	 
		res=USART_DATA(USART0); 
		if((USART0_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART0_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART0_RX_STA=0;//接收错误,重新开始
				else USART0_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART0_RX_STA|=0x4000;
				else
				{
					USART0_RX_BUF[USART0_RX_STA&0X3FFF]=res;
					USART0_RX_STA++;
					if(USART0_RX_STA>(USART0_REC_LEN-1))USART0_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	} 
}
#endif	

void USART0_Config(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);//使能GPIOA时钟

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART0);//使能USART0时钟
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);//复用功能7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);//PA9配置成串口输出
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);//复用功能7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);//PA10配置成串口输入
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
	nvic_irq_enable(USART0_IRQn, 1, 0);//使能USART0中断
}

void USART5_Config(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOC);//使能GPIOA时钟

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART5);//使能USART0时钟
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_6);//复用功能7
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);//PA9配置成串口输出
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_7);//复用功能7
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);//PA10配置成串口输入
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	
	
}


void USART_Init(uint32_t usart_periph, uint32_t baud)
{
		uint8_t usart_num = 0;
		uint8_t usart_prior[2];
		switch(usart_periph){
			case USART0:
					USART0_Config();
					usart_prior[0]=1;
					usart_prior[1]=1;
					usart_num = USART0_IRQn;
					break;
			case USART5:
					USART5_Config();
					usart_prior[0]=1;
					usart_prior[1]=0;
					usart_num = USART5_IRQn;
					break;
			default:
					break;
		}
    nvic_irq_enable(usart_num, usart_prior[0], usart_prior[1]);//使能USARTx中断	
		usart_interrupt_enable(usart_periph, USART_INT_RBNE);//接收中断打开
}

