#ifndef GD32_USART_H
#define GD32_USART_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "gd32f4xx_usart.h"
#include "gd32f4xx_exti.h"

#define USART0_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART0_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define USART5_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define USART5_MAX_SEND_LEN		800					//����ͻ����ֽ���
#define USART5_RX_EN 			1					//0,������;1,����.


extern __IO uint8_t  USART5_RX_BUF[USART5_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern uint8_t  USART5_TX_BUF[USART5_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern __IO uint16_t USART5_RX_STA;   						//��������״̬
 
void USART_Init(uint32_t usart_periph, uint32_t baud);
void u5_printf(char* fmt,...);
void USART5_Send(const char* data, uint16_t len);
void USART5_Clear(void);
uint16_t USART5_Revice(char *data);
extern void  USART5_TIM_1ms(void);
#endif
