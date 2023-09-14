#include "usart.h"
#include "delay.h"
#include "malloc.h"
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
 
#if USART2_RX_EN   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
vu16 USART2_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void usart2_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_USART2,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART2, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if USART2_RX_EN	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 											 
} 
#endif



//串口发送缓存区 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
//串口接收缓存区 	
u8 *USART1_RX_BUF=NULL;				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.


//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART1_RX_STA=0;  

vu32 g_usart1Cnt = 0;            // ??3??????
vu8 g_usart1RevTimCnt = 0;         // ??3????,??????
vu8 g_usart1RevFlag = 0;        // ??3????
vu8 g_usart1RevFinish = 0;      // ????????

u16 g_mqttReadLen=0;//mqtt

void  USART1_TIM_1ms(void)
{
		if(g_usart1RevFlag)
		{
				if(++g_usart1RevTimCnt > 50)
				{
						g_mqttReadLen=0;
						g_usart1RevFinish=1;
						g_usart1RevFlag=0;
						g_usart1RevTimCnt=0;
				}
		}
}
/**
* @brief  ??3????
* @param  cmd:??
*	        res:?????????
*         timeOut ????
*         resnum ??????
* @retval ?
* @note   0-??	1-??
*/
u8 USART1_Send_ATCmd(const char *cmd,const char *res3,u32 timeOut,u8 retime)
{
    u32 timeout;
    USART1_Clear();
    while(retime--)
    {
        timeout = timeOut;
        u1_printf(cmd);
        while(timeout--)
        {
            if(g_usart1RevFinish)							                  // ????????
            {
                if(strstr((const char *)USART1_TX_BUF, res3) != NULL)		// ????????
                {
                    USART1_Clear();										                // ????
                    return 0;
                }
            }
            delay_ms(1);
        }
    }
    return 1;
}

void USART1_Clear(void)
{
		memset((u8*)USART1_RX_BUF, 0, sizeof(USART1_RX_BUF));
		g_usart1Cnt = 0;
		g_usart1RevFinish = 0;
}

u8* g_netData=NULL;
u16 USART1_Revice(void)
{
	u16 len = g_usart1Cnt;
	if(g_usart1RevFinish)
	{
		g_usart1RevFinish = 0;
		if(len>0)
		{
				USART1_RX_BUF[len]='\0';//?????
				printf("\r\nRECV:%s\r\n",USART1_RX_BUF);
				memset(g_netData, 0, USART1_MAX_RECV_LEN);
				memcpy(g_netData, (u8*)USART1_RX_BUF, len+1);
				USART1_Clear();
				return len;	
		}else{
			USART1_Clear();
			return 0;
		}	
	}
	return 0;
}



void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)//接收到数据
	{	 	
				//USART_ClearFlag(USART1, USART_FLAG_RXNE);       
				g_usart1RevFlag = 1;                                     // ??2 ????
				g_usart1RevTimCnt = 0;	                                     // ??2 ??????
				USART1_RX_BUF[g_usart1Cnt++] = USART1->DR;                       // ??????
				if(g_usart1Cnt >= USART1_MAX_RECV_LEN)
				{
						g_usart1Cnt = 0;                                         // ???????
				}		
	}		
}   
//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart1_init(u32 bound)
{  	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOA,ENABLE); //使能GPIOB时钟
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_USART1,ENABLE);//使能USART3时钟
	
	USART_DeInit(USART1);  //复位串口1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOB11和GPIOB10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART1); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART1); //GPIOB10复用为USART3	  
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口3
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断  
	USART_Cmd(USART1, ENABLE);                    //使能串口 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART1_RX_STA=0;				//清零 
	if(USART1_RX_BUF == NULL)
		USART1_RX_BUF=mymalloc(SRAMIN,USART1_MAX_RECV_LEN);
	if(g_netData==NULL)
		g_netData=mymalloc(SRAMIN,USART1_MAX_RECV_LEN);
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u1_printf(const char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
		USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]);   
	}
}

void USART1_SendData(const u8* data,u16 len)
{
	const u8* p = data;
	while(len--)//循环发送数据
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//循环发送,直到发送完毕   
		USART_SendData(USART1,*p++);   
	}
}
void usart2_test(void)
{
	u8 t,len=0;
	if(USART2_RX_STA&0x8000)
	{					   
			len=USART2_RX_STA&0x3fff;
			printf("\r\nReceive data :\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART2, USART2_RX_BUF[t]);         
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
			}
			printf("\r\n\r\n");//????
			USART2_RX_STA=0;
	}
	printf("usart test\n");
}
