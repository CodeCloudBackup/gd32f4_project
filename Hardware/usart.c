#include "usart.h"
#include "delay.h"
#include "malloc.h"
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
 
#if USART2_RX_EN   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
vu16 USART2_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void usart2_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART2); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART2); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if USART2_RX_EN	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 											 
} 
#endif



//���ڷ��ͻ����� 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
u8 *USART1_RX_BUF=NULL;				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
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
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)//���յ�����
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
//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart1_init(u32 bound)
{  	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOBʱ��
	RCU_APB1PeriphClockCmd(RCU_APB1Periph_USART1,ENABLE);//ʹ��USART3ʱ��
	
	USART_DeInit(USART1);  //��λ����1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOB11��GPIOB10��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART1); //GPIOB11����ΪUSART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART1); //GPIOB10����ΪUSART3	  
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������3
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�  
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART1_RX_STA=0;				//���� 
	if(USART1_RX_BUF == NULL)
		USART1_RX_BUF=mymalloc(SRAMIN,USART1_MAX_RECV_LEN);
	if(g_netData==NULL)
		g_netData=mymalloc(SRAMIN,USART1_MAX_RECV_LEN);
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u1_printf(const char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
		USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]);   
	}
}

void USART1_SendData(const u8* data,u16 len)
{
	const u8* p = data;
	while(len--)//ѭ����������
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//ѭ������,ֱ���������   
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
