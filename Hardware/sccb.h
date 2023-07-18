#ifndef __SCCB_H
#define __SCCB_H

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"

#define SCCB_SDA_IN() gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
#define SCCB_SDA_OUT() gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
#define SCCB_ID   			0X60  			//OV2640的ID

//IO操作函数	 
#define SCCB_SCL   				PDout(6)	//SCL
#define SCCB_SDA    			PDout(7)  //SDA	 
#define SCCB_READ_SDA    	PDin(7)  	//输入SDA  
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

