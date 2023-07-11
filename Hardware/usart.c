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
	USART_DATA(USART0) = ((uint16_t)USART_DATA_DATA & (u32)ch);  
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART0_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART0_RX_BUF[USART0_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART0_RX_STA=0;       	//接收状态标记	  
u32 USART0_RX_CNT=0;			//接收的字节数	

void USART0_IRQHandler(void)
{
	u8 res;
	if(usart_flag_get(USART0, USART_FLAG_RBNE))//接收到数据
	{	 
		res=USART_DATA(USART0); 
		if(USART0_RX_CNT < USART0_REC_LEN)//接收未完成
		{
			USART0_RX_BUF[USART0_RX_CNT+8]=res;
			USART0_RX_CNT++;
		}  		 									     
	} 
}

uint16_t  USART0_TIM_50ms(void)
{
	static uint16_t oldcount=0;	//老的串口接收数据值
	uint16_t applenth=0;
	if(USART0_RX_CNT)
	{
			if(oldcount == USART0_RX_CNT)
			{
				applenth=USART0_RX_CNT;
				oldcount=0;
				USART0_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}else oldcount=USART0_RX_CNT;
	}
	return applenth;
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

//串口发送缓存区 	
__align(8) u8 USART5_TX_BUF[USART5_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
//串口接收缓存区 	
__IO u8 USART5_RX_BUF[USART5_MAX_RECV_LEN] __attribute__((at(0X20001000))); 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
__IO uint16_t USART5_RX_STA = 0;
__IO u8 usart5_rev_cnt = 0;
__IO u8 usart5_rev_flag = 0;
__IO u8 usart5_rev_finish = 0;      // 串口接收完成标志
void USART5_IRQHandler(void)
{
		u8 res;
    if(RESET != usart_interrupt_flag_get(USART5, USART_INT_FLAG_RBNE)){				
				/* receive data */ 
			  res =  USART_DATA(USART5);
			  if(!usart5_rev_finish)
				{
						//记录接收到的值
					if(USART5_RX_STA<USART5_MAX_RECV_LEN)
					{
							if(USART5_RX_STA==0) 
								usart5_rev_flag = 1; 
						  usart5_rev_cnt = 0;
							USART5_RX_BUF[USART5_RX_STA++]= res;					
					}else
					{
							usart5_rev_finish=0;
					}
				}
			}
}

void  USART5_TIM_1ms(void)
{
		if(usart5_rev_flag)
		{
				if(++usart5_rev_cnt > 50)
				{
						usart5_rev_finish=1;
						usart5_rev_flag=0;
						usart5_rev_cnt=0;
				}
		}
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u5_printf(char* fmt,...)  
{  
	uint16_t i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART5_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
		while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);//循环发送,直到发送完毕   
		USART_DATA(USART5)=USART5_TX_BUF[j];  
	}
}

void USART5_Send(const char* data, uint16_t len)
{
		while(len--)
		{
			while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);//循环发送,直到发送完毕   
			USART_DATA(USART5)=*data++;
		}
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
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	
	
}
void USART5_Clear(void)
{
		memset((int8_t*)USART5_RX_BUF, 0, sizeof(USART5_RX_BUF));
		USART5_RX_STA = 0;
		usart5_rev_finish = 0;
}

uint16_t USART5_Revice(char *data)
{
	uint16_t len = USART5_RX_STA;
	if(usart5_rev_finish)
	{
		usart5_rev_finish = 0;
		if(len>0)
		{
			USART5_RX_BUF[len]='\0';//添加结束符
			memcpy(data, (char*)USART5_RX_BUF, len+1);
			USART5_Clear();
			return len;
		}else{
			USART5_Clear();
			return 0;
		}	
	}
	return 0;
}

void USART_Init(u32 usart_periph, u32 baud)
{
		u8 usart_num = 0;
		u8 usart_prior[2];
		switch(usart_periph){
			case USART0:
					USART0_Config();
					usart_prior[0]=1;
					usart_prior[1]=1;
					usart_num = USART0_IRQn;
					break;
			case USART5:
					USART5_Config();
					usart_prior[0]=0;
					usart_prior[1]=0;
					usart_num = USART5_IRQn;
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

