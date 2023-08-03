#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//串口1初始化 
////////////////////////////////////////////////////////////////////////////////// 

#define USART_REC_LEN  			120*1024 //定义最大接收字节数 120K
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u32 USART_RX_CNT;				//接收的字节数	

void uart_init(u32 pclk2,u32 bound); 
#endif	   
















