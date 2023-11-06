#ifndef __USART_H
#define __USART_H 

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "systick.h"

//NSS 
#define USART1_MAX_RECV_LEN		32*1024					//�����ջ����ֽ���
#define USART1_MAX_SEND_LEN		200					//����ͻ����ֽ���
#define USART1_RX_EN 			1					//0,������;1,����.

extern u8  *USART1_RX_BUF; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART1_RX_STA;   						//��������״̬
extern u16 g_mqttReadLen;//mqtt
extern vu8 g_usart1RevFinish;
extern vu32 g_usart1Cnt;

void usart1_init(u32 bound);
void u1_printf(const char* fmt,...);
u16 USART1_Revice(u8 *buf, u32 buflen);
void USART1_Clear(void);
void USART1_SendData(const u8* data,u16 len);
// debug output 
#define USART2_MAX_RECV_LEN		10					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		200					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART2_RX_STA;   						//��������״̬
void usart2_init(u32 bound);

void usart2_test(void);
#endif
