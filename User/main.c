/*!
    \file    main.c
    \brief   GPIO running led demo

    \version 2022-04-26, V2.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "led.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "dac.h"
#include "dcmi.h"
#include "ov2640.h"
#include "sccb.h"
#include "malloc.h"
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
00*/


//jpeg数据接收回调函数
void jpeg_dcmi_rx_callback(void)
{ 
	
}

int main(void)
{
    /* configure systick */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    systick_config();
		delay_init(200);    //初始化延时函数
		LED_Init();
		TIM1_Init(99,999); //定时器时钟100M，分频系数1000，所以100M/1000=100Khz的计数频率，计数100次为1ms  
		usart1_init(115200);
		usart2_init(115200);
		Speaker_Init();
		my_mem_init(SRAMIN);		//初始化内部内存池 
		SPEAKER_SW=1;
		while(OV2640_Init())//初始化OV2640
		{
			printf("ov2640_init failed\r\n");
			delay_ms(400);
		}
		delay_ms(2000);
		MyDCMI_Init();			//DCMI配置
		
		OV2640_Jpg_Photo();
    while(1) {
			//printf("xxxx\r\n");
			// LED_Test();
			DAC1_Test( 36,4096 );
      // usart2_test();
			
    }
}
