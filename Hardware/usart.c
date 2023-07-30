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
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<0;//ʹ��GPIOAʱ��
	/* enable USART clock */
	RCU->APB2EN|=1<<4;//ʹ��USART0ʱ��
	//USART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10; //PA8��PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA8��PA9
	
	//����0��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA8����ΪUSART0
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA9����ΪUSART0
}

//���ڷ��ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
vu8* USART2_RX_BUF; 			//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
vu16 USART2_RX_STA = 0;
vu8 usart2_rev_cnt = 0;
vu8 usart2_rev_flag = 0;
vu8 usart2_rev_finish = 0;      // ���ڽ�����ɱ�־
void USART2_IRQHandler(void)
{
		u8 res;
    if(RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE)){				
				/* receive data */ 
			  res =  USART_DATA(USART2);
			  if(!usart2_rev_finish)
				{
						//��¼���յ���ֵ
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

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);//ѭ������,ֱ���������   
		USART_DATA(USART2)=USART2_TX_BUF[j];  
	}
}

void USART2_Send(const char* data, u16 len)
{
	while(len--)
	{
		while(usart_flag_get(USART2, USART_FLAG_TC) == RESET);//ѭ������,ֱ���������   
		USART_DATA(USART2)=*data++;
	}
}

void USART5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<2;//ʹ��GPIOCʱ��
	/* enable USART clock */
	RCU->APB2EN|=1<<5;//ʹ��USART5ʱ��
	//USART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PC6��PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC6��PC7
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6����ΪUSART5
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC7����ΪUSART5
}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<1;//ʹ��GPIOBʱ��
	/* enable USART clock */
	RCU->APB1EN|=1<<18;//ʹ��USART2ʱ��
	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //PB10��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PB10��PB11
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART2
}

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO clock */
	RCU->AHB1EN|=1<<0;//ʹ��GPIOCʱ��
	/* enable USART clock */
	RCU->APB1EN|=1<<17;//ʹ��USART5ʱ��
	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2��PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOB10����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOB11����ΪUSART1
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
				USART2_RX_BUF[len]='\0';//��ӽ�����
				memcpy(data, (u8*)USART2_RX_BUF, len+1);
				USART2_Clear();
				return len;
			}else if(type == DATA)
			{
				USART2_RX_BUF[len]='\0';//��ӽ�����
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
		usart_enable(usart_periph);//ʹ��USART5
    nvic_irq_enable(usart_num, usart_prior[0], usart_prior[1]);//ʹ��USARTx�ж�	
		usart_interrupt_enable(usart_periph, USART_INT_RBNE);//�����жϴ�
}

