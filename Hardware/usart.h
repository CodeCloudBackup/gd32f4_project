#ifndef GD32_USART_H
#define GD32_USART_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "gd32f4xx.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_exti.h"

#define USART0_REC_LEN  			800   	//定义最大接收字节数 200
#define EN_USART0_RX 			1		//使能（1）/禁止（0）串口1接收

#define USART2_MAX_RECV_LEN		8*4096					//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		800					//最大发送缓存字节数
#define USART2_RX_EN 			1					//0,不接收;1,接收.


extern u8  USART0_RX_BUF[USART0_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART0_RX_STA;         		//接收状态标记	
extern u32 USART0_RX_CNT;				//接收的字节�

extern u16  USART0_TIM_50ms(void);

extern vu8  *USART2_RX_BUF; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern vu16 USART2_RX_STA;   						//接收数据状态
  
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
