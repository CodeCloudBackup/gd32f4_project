#ifndef __LCD_H
#define __LCD_H		
#include "systick.h"
//LED�˿ڶ���
#define LED_R PFout(2)	// DS0
#define LED_G PFout(3)	// DS1	 
#define LED_B PFout(4)	// DS1

void LED_Init(void);//��ʼ��		 				    
#endif
