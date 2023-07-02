#include "usart.h"

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
u8 USART0_RX_BUF[USART0_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 USART0_RX_STA=0;       //接收状态标记	
void USART0_IRQHandler(void)
{
	uint8_t res;
	if(usart_flag_get(USART0, USART_FLAG_RBNE))//接收到数据
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
}

void USART_Init(u32 usart_periph, u32 baud)
{
		u8 usart_num = 0;
		u8 usart_prior[2];
		switch(usart_periph){
			case USART0:
					USART0_Config();
					usart_prior[0]=0;
					usart_prior[1]=0;
					usart_num = USART0_IRQn;
					break;
			default:
					break;
		}
		/* USART configure */
		usart_deinit(usart_periph);
		usart_baudrate_set(usart_periph,baud);
		usart_word_length_set(usart_periph, USART_WL_8BIT);
		usart_stop_bit_set(usart_periph, USART_STB_1BIT);
		usart_parity_config(usart_periph, USART_PM_NONE);

		usart_transmit_config(usart_periph, USART_TRANSMIT_ENABLE);
		usart_receive_config(usart_periph, USART_RECEIVE_ENABLE);
		usart_enable(usart_periph);//使能USART5
    nvic_irq_enable(usart_num, usart_prior[0], usart_prior[1]);//使能USARTx中断	
		usart_interrupt_enable(usart_periph, USART_INT_RBNE);//接收中断打开
}
