#include "usart.h"
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
	USART_DATA(USART0) = ((uint16_t)USART_DATA_DATA & (uint32_t)ch);  
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART0_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
uint8_t USART0_RX_BUF[USART0_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART0_RX_STA=0;       //����״̬���	
void USART0_IRQHandler(void)
{
	uint8_t res;
	if(usart_flag_get(USART0, USART_FLAG_RBNE)))//���յ�����
	{	 
		res=USART_DATA(USART0); 
		if((USART0_RX_STA&0x8000)==0)//����δ���
		{
			if(USART0_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART0_RX_STA=0;//���մ���,���¿�ʼ
				else USART0_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART0_RX_STA|=0x4000;
				else
				{
					USART0_RX_BUF[USART0_RX_STA&0X3FFF]=res;
					USART0_RX_STA++;
					if(USART0_RX_STA>(USART0_REC_LEN-1))USART0_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	} 
}
#endif	

void USART0_Config(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);//ʹ��GPIOAʱ��

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART0);//ʹ��USART0ʱ��
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);//���ù���7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);//PA9���óɴ������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
	gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);//���ù���7
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);//PA10���óɴ�������
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
	nvic_irq_enable(USART0_IRQn, 1, 0);//ʹ��USART0�ж�
}

void USART5_Config(void)
{
	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOC);//ʹ��GPIOAʱ��

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_USART5);//ʹ��USART0ʱ��
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_6);//���ù���7
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);//PA9���óɴ������
	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
	gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_7);//���ù���7
	gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);//PA10���óɴ�������
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
    nvic_irq_enable(usart_num, usart_prior[0], usart_prior[1]);//ʹ��USARTx�ж�	
		usart_interrupt_enable(usart_periph, USART_INT_RBNE);//�����жϴ�
}

