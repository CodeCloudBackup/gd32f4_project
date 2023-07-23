#ifndef __SCCB_H
#define __SCCB_H

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"

//IO��������
#define SCCB_SDA_IN()  {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=0<<7*2;}	//PD7 ����
#define SCCB_SDA_OUT() {GPIOD->MODER&=~(3<<(7*2));GPIOD->MODER|=1<<7*2;} 	//PD7 ���
#define SCCB_ID   			0X60  			//OV2640��ID

//IO��������	 
#define SCCB_SCL   				PDout(6)	//SCL
#define SCCB_SDA    			PDout(7)  //SDA	 
#define SCCB_READ_SDA    	PDin(7)  	//����SDA  
///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif

