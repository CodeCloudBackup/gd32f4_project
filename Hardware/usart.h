#ifndef GD32_USART_H
#define GD32_USART_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "gd32f4xx.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_exti.h"

#define USART0_REC_LEN  			800   	//�����������ֽ��� 200
#define EN_USART0_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define USART2_MAX_RECV_LEN		8*4096					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		800					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.


extern u8  USART0_RX_BUF[USART0_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART0_RX_STA;         		//����״̬���	
extern u32 USART0_RX_CNT;				//���յ��ֽ��

extern u16  USART0_TIM_50ms(void);

extern vu8  *USART2_RX_BUF; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART2_RX_STA;   						//��������״̬
  
typedef enum DATA_TYPE
{
		COMMAND = 0,
		DATA
}DataType;	

void USART_Init(u32 usart_periph, u32 baud);
void u2_printf(char* fmt,...);
void USART2_Send(const char* data, u16 len);
void USART2_Clear(void);
u16 USART2_Revice(DataType type,u8* data);
extern void  USART2_TIM_1ms(void);
#endif
