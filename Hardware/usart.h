#ifndef GD32_USART_H
#define GD32_USART_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "gd32f4xx.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_exti.h"

#define USART0_REC_LEN  			800   	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊı 200
#define EN_USART0_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ

#define USART2_MAX_RECV_LEN		8*4096					//×î´ó½ÓÊÕ»º´æ×Ö½ÚÊı
#define USART2_MAX_SEND_LEN		800					//×î´ó·¢ËÍ»º´æ×Ö½ÚÊı
#define USART2_RX_EN 			1					//0,²»½ÓÊÕ;1,½ÓÊÕ.


extern u8  USART0_RX_BUF[USART0_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û 
extern u16 USART0_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
extern u32 USART0_RX_CNT;				//½ÓÊÕµÄ×Ö½ÚÊ

extern u16  USART0_TIM_50ms(void);

extern vu8  *USART2_RX_BUF; 		//½ÓÊÕ»º³å,×î´óUSART3_MAX_RECV_LEN×Ö½Ú
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//·¢ËÍ»º³å,×î´óUSART3_MAX_SEND_LEN×Ö½Ú
extern vu16 USART2_RX_STA;   						//½ÓÊÕÊı¾İ×´Ì¬
  
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
