#ifndef _USART_H_
#define _USART_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_exti.h"

#define USART0_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART0_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

void USART_Init(u32 usart_periph, u32 baud);

#endif
