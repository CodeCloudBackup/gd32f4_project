#ifndef _USART_H_
#define _USART_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_exti.h"

#define USART0_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART0_RX 			1		//使能（1）/禁止（0）串口1接收

void USART_Init(u32 usart_periph, u32 baud);

#endif
