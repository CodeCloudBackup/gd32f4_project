#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "gd32f4xx.h"
#include "systick.h"
#include "oled.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "esp8266.h"
#include "tcp.h"
#include "http.h"
#include "gdflash.h"
#include "w25qxx.h"
#include "spi.h"
#include "malloc.h"
#include "delay.h"
#include "sccb.h"
#include "ov2640.h"

typedef struct
{
	u16 flash_size;
	u32 IC_ID[3];
}IC_INFO;


extern IC_INFO g_icInfo;
extern APP_INFO g_appInfo;
void IC_Informatin(void);
void Program_Init(void);

#endif
