#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "gd32f4xx.h"
#include "gd32f4xx_libopt.h"
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
#include "nand_flash.h"
#include "spi.h"
#include "malloc.h"

typedef struct
{
	u16 flash_size;
	u32 IC_ID[3];
}IC_INFO;

typedef struct
{
	uint32_t App_Version;
	uint32_t App_Size;
}APP_INFO;

typedef union 
{
	u8 u8_data[4];
	u32 u32_data;
} U8_U32;


extern IC_INFO g_icInfo;
extern APP_INFO g_appInfo;
void IC_Informatin(void);
void Program_Init(void);

#endif
