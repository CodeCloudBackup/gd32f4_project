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

/*
	自定义缩写词
	protocol PROT
	status   STA
	deliver  DELY
	result   RES
	config   CFG
*/

#include "program.h"
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
00*/
#define CLI() __set_PRIMASK(1)//关闭总中断  
#define SEI() __set_PRIMASK(0)//打开总中断

extern u8 g_identFlag;
int main(void)
{
	SEI();
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);//中断向量地址偏移0x100000
	Program_Init();
    /* configure systick */	
	//	OV2640_Jpg_Photo();
	char *http_ip = g_sDeviceConf.ip_ini.http_ip;
	char *mqtt_ip = g_sDeviceConf.ip_ini.mqtt_ip;
	u32 mqttPort = g_sDeviceConf.ip_ini.mqtt_port;
	u32 httpPort = g_sDeviceConf.ip_ini.http_port;
	u32 httpUploadPort = g_sDeviceConf.ip_ini.http_port;
	u8 httpSockId = 1;
	u8 mqttSockId = 2;
	
  while(1) {
			
	  HM609A_Tcp_Program(mqttSockId, mqtt_ip, mqttPort, MQTT_PROT);
		HM609A_Mqtt_Program(mqttSockId);
		MQTT_Data_Program();
		if(g_sHttpCmdSta.sta_cmd)
		{
			if(hm609a_mqtt_reg_flag)
			{
				HM609A_Tcp_Program(httpSockId, http_ip, httpPort, HTTP_PROT);
				HM609A_Http_Program(httpSockId, http_ip, httpPort);	
			}
			else
			{
				g_sHttpCmdSta.sta_cmd=0;
			}						
		}
		Data_Recv_Program();
		Device_Program();
    }
}
