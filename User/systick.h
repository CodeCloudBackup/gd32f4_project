/*!
    \file    systick.h
    \brief   the header file of systick
    
    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>
#include "gd32f4xx.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO口地址映射
#define GPIOA_OCTL_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_OCTL_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_OCTL_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_OCTL_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_OCTL_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_OCTL_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_OCTL_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_OCTL_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_OCTL_Addr    (GPIOI_BASE+20) //0x40022014 

#define GPIOA_ISTAT_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_ISTAT_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_ISTAT_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_ISTAT_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_ISTAT_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_ISTAT_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_ISTAT_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_ISTAT_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_ISTAT_Addr    (GPIOI_BASE+16) //0x40022010 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_OCTL_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_ISTAT_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_OCTL_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_ISTAT_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_OCTL_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_ISTAT_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_OCTL_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_ISTAT_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_OCTL_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_ISTAT_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_OCTL_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_ISTAT_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_OCTL_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_ISTAT_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_OCTL_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_ISTAT_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_OCTL_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_ISTAT_Addr,n)  //输入

typedef union {
		volatile uint8_t Byte;
		struct {
			volatile uint8_t B0:1;
			volatile uint8_t B1:1;
			volatile uint8_t B2:1;
			volatile uint8_t B3:1;
			volatile uint8_t B4:1;
			volatile uint8_t B5:1;
			volatile uint8_t B6:1;
			volatile uint8_t B7:1;
		}Bits;
}Byte8;

typedef union 
{
	u8 u8_data[4];
	u32 u32_data;
} U8_U32;

typedef struct
{
	U8_U32 App_Version;
	U8_U32 App_Size;
}APP_INFO;

#define U8ToU32(u8_buf) ((*(u8_buf)<<24)|(*(u8_buf+1)<<16)|(*(u8_buf+2)<<8)|*(u8_buf+3))
void U32ToU8Array(uint8_t* buf, uint32_t u32Val);
/* configure systick */
void systick_config(void);
/* delay a time in milliseconds */
void delay_1ms(uint32_t count);
/* delay decrement */
void delay_decrement(void);

#endif /* SYS_TICK_H */
