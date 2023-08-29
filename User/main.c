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

#include "program.h"
#include "MQTTPacket.h"
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
00*/
#define CLI() __set_PRIMASK(1)//关闭总中断  
#define SEI() __set_PRIMASK(0)//打开总中断

int main(void)
{
	u32 flash_id = 0;
	//	SEI();
	//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);//中断向量地址偏移0x100000
		Program_Init();
    /* configure systick */	
	//	OV2640_Jpg_Photo();
	char *host = "101.37.89.157";//主机
	int port = 1883;


		printf("publishing reading\n");//读取发布

  while(1) {
			
	  	HM609A_Program(host,port);
			HM609A_Mqtt_Program(host,port);
			//LED_PWM_Test();
//				flash_id=F35SQA_ReadID();	//读取FLASH ID.
//				printf("F35SQA_ID:%x",flash_id);
//				delay_ms(1000);
		//	get_imu_data();
			//Sensor_Adc_Test();
			//Open_Lock_Test();
			// LED_Test();
			//DAC1_Test( 36,4096 );
      // usart2_test();
			
    }
}
