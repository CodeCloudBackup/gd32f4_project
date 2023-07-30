#include "usart.h"
#include "malloc.h"
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
	USART_DATA(USART0) = ((u16)USART_DATA_DATA & (u32)ch);  
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
u16 USART0_RX_STA=0;       	//接收状态标记	  
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

u16  USART0_TIM_50ms(void)
{
	static u16 oldcount=0;	//老的串口接收数据值
	u16 applenth=0;
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
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<0;//使能GPIOA时钟
	/* enable USART clock */
	RCU->APB2EN|=1<<4;//使能USART0时钟
	//USART5端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10; //PA8与PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA8，PA9
	
	//串口0对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA8复用为USART0
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA9复用为USART0
}

//串口发送缓存区 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
//串口接收缓存区 	
vu8* USART2_RX_BUF; 			//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
vu16 USART2_RX_STA = 0;
vu8 usart2_rev_cnt = 0;
vu8 usart2_rev_flag = 0;
vu8 usart2_rev_finish = 0;      // 串口接收完成标志
void USART2_IRQHandler(void)
{
		u8 res;
    if(RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)){				
				/* receive data */ 
			  res =  USART_DATA(USART2);
			  if(!usart2_rev_finish)
				{
						//记录接收到的值
					if(USART2_RX_STA<USART2_MAX_RECV_LEN)
					{
							if(USART2_RX_STA==0) 
								usart2_rev_flag = 1; 
						  usart2_rev_cnt = 0;
							USART2_RX_BUF[USART2_RX_STA++]= res;					
					}else
					{
							usart2_rev_finish=0;
					}
				}
			}
}

void  USART2_TIM_1ms(void)
{
		if(usart2_rev_flag)
		{
				if(++usart2_rev_cnt > 50)
				{
						usart2_rev_finish=1;
						usart2_rev_flag=0;
						usart2_rev_cnt=0;
				}
		}
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
		while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);//循环发送,直到发送完毕   
		USART_DATA(USART2)=USART2_TX_BUF[j];  
	}
}

void USART2_Send(const char* data, u16 len)
{
	while(len--)
	{
		while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);//循环发送,直到发送完毕   
		USART_DATA(USART2)=*data++;
	}
}

void USART5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<2;//使能GPIOC时钟
	/* enable USART clock */
	RCU->APB2EN|=1<<5;//使能USART5时钟
	//USART5端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PC6与PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC6，PC7
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6复用为USART5
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC7复用为USART5
}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<1;//使能GPIOB时钟
	/* enable USART clock */
	RCU->APB1EN|=1<<18;//使能USART2时钟
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //PB10与PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB10，PB11
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART2
}

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<0;//使能GPIOC时钟
	/* enable USART clock */
	RCU->APB1EN|=1<<17;//使能USART5时钟
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2与PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOB10复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOB11复用为USART1
}

void USART2_Clear(void)
{
		memset((u8*)USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
		USART2_RX_STA = 0;
		usart2_rev_finish = 0;
}

u16 USART2_Revice(DataType type, u8* data)
{
	u16 len = USART2_RX_STA;
	if(usart2_rev_finish)
	{
		usart2_rev_finish = 0;
		if(len>0)
		{
			if(type == COMMAND)
			{
				USART2_RX_BUF[len]='\0';//添加结束符
				memcpy(data, (u8*)USART2_RX_BUF, len+1);
				USART2_Clear();
				return len;
			}else if(type == DATA)
			{
				USART2_RX_BUF[len]='\0';//添加结束符
				memcpy(data, (u8*)USART2_RX_BUF, len+1);
				USART2_Clear();
				return len;
			}
			
		}else{
			USART2_Clear();
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
			case USART1:
					USART1_Config();
					usart_prior[0]=0;
					usart_prior[1]=1;
					usart_num = USART1_IRQn;
					break;
			case USART2:
					USART2_Config();
					usart_prior[0]=0;
					usart_prior[1]=1;
					usart_num = USART2_IRQn;
					USART2_RX_STA = 0;
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

