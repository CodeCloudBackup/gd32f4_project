#ifndef __LED_H
#define __LED_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6���İ�
//LED��������	   
//�汾��V1.0							  
////////////////////////////////////////////////////////////////////////////////// 	

//LED�˿ڶ���
#define LEDR PFout(2)	// LED0
#define LEDG PFout(3)	// LED1	 
#define LEDB PFout(4)	// LED1	

void LED_Init(void);//LED��ʼ��		 				    
#endif

















