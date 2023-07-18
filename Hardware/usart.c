#include "usart.h"
#include "malloc.h"
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(usart_flag_get(USART0,USART_FLAG_TC)==0);//ѭ������,ֱ��������� 
	USART_DATA(USART0) = ((u16)USART_DATA_DATA & (u32)ch);  
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART0_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART0_RX_BUF[USART0_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART0_RX_STA=0;       	//����״̬���	  
u32 USART0_RX_CNT=0;			//���յ��ֽ���	

void USART0_IRQHandler(void)
{
	u8 res;
	if(usart_flag_get(USART0, USART_FLAG_RBNE))//���յ�����
	{	 
		res=USART_DATA(USART0); 
		if(USART0_RX_CNT < USART0_REC_LEN)//����δ���
		{
			USART0_RX_BUF[USART0_RX_CNT+8]=res;
			USART0_RX_CNT++;
		}  		 									     
	} 
}

u16  USART0_TIM_50ms(void)
{
	static u16 oldcount=0;	//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;
	if(USART0_RX_CNT)
	{
			if(oldcount == USART0_RX_CNT)
			{
				applenth=USART0_RX_CNT;
				oldcount=0;
				USART0_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
			}else oldcount=USART0_RX_CNT;
	}
	return applenth;
}

#endif	


void USART0_Config(void)
{
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<0;//ʹ��GPIOAʱ��
	/* enable USART clock */
	RCU->APB2EN|=1<<4;//ʹ��USART0ʱ��
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);//���ù���7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);//PA9���óɴ������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);//���ù���7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);//PA10���óɴ�������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
}

//���ڷ��ͻ����� 	
__align(8) u8 USART5_TX_BUF[USART5_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
vu8* USART5_RX_BUF; 			//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
vu16 USART5_RX_STA = 0;
vu8 usart5_rev_cnt = 0;
vu8 usart5_rev_flag = 0;
vu8 usart5_rev_finish = 0;      // ���ڽ�����ɱ�־
void USART2_IRQHandler(void)
{
		u8 res;
    if(RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)){				
				/* receive data */ 
			  res =  USART_DATA(USART2);
			  if(!usart5_rev_finish)
				{
						//��¼���յ���ֵ
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

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u5_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART5_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);//ѭ������,ֱ���������   
		USART_DATA(USART2)=USART5_TX_BUF[j];  
	}
}

void USART5_Send(const char* data, u16 len)
{
	while(len--)
	{
		while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);//ѭ������,ֱ���������   
		USART_DATA(USART2)=*data++;
	}
}

void USART5_Config(void)
{
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<2;//ʹ��GPIOCʱ��
	/* enable USART clock */
	RCU->APB2EN|=1<<5;//ʹ��USART5ʱ��
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_6);//���ù���7
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);//PA9���óɴ������
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_7);//���ù���7
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);//PA10���óɴ�������
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
}

void USART2_Config(void)
{
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<1;//ʹ��GPIOBʱ��
	/* enable USART clock */
	RCU->APB1EN|=1<<18;//ʹ��USART2ʱ��
	gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_10);//���ù���7
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);//PA9���óɴ������
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
	gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_11);//���ù���7
	gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);//PA10���óɴ�������
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
}

void USART1_Config(void)
{
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<0;//ʹ��GPIOCʱ��
	/* enable USART clock */
	RCU->APB1EN|=1<<17;//ʹ��USART5ʱ��
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2);//���ù���7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);//PA9���óɴ������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_3);//���ù���7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);//PA10���óɴ�������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
}

void USART5_Clear(void)
{
		memset((u8*)USART5_RX_BUF, 0, sizeof(USART5_RX_BUF));
		USART5_RX_STA = 0;
		usart5_rev_finish = 0;
}

u16 USART5_Revice(DataType type, u8* data)
{
	u16 len = USART5_RX_STA;
	if(usart5_rev_finish)
	{
		usart5_rev_finish = 0;
		if(len>0)
		{
			if(type == COMMAND)
			{
				USART5_RX_BUF[len]='\0';//��ӽ�����
				memcpy(data, (u8*)USART5_RX_BUF, len+1);
				USART5_Clear();
				return len;
			}else if(type == DATA)
			{
				USART5_RX_BUF[len]='\0';//��ӽ�����
				memcpy(data, (u8*)USART5_RX_BUF, len+1);
				USART5_Clear();
				return len;
			}
			
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
		usart_enable(usart_periph);//ʹ��USART5
    nvic_irq_enable(usart_num, usart_prior[0], usart_prior[1]);//ʹ��USARTx�ж�	
		usart_interrupt_enable(usart_periph, USART_INT_RBNE);//�����жϴ�
}

