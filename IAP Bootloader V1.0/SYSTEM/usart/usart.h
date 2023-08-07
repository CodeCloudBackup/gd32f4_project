#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//����1��ʼ�� 
////////////////////////////////////////////////////////////////////////////////// 

#define USART_REC_LEN  			120*1024 //�����������ֽ��� 120K
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u32 USART_RX_CNT;				//���յ��ֽ���	

void uart_init(u32 pclk2,u32 bound); 
#endif	   















