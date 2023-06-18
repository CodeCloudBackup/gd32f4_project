#ifndef __TCP_H
#define __TCP_H 			   
#include "gd32f4xx.h"
#include "systick.h"
/*
*以下参数需要用户自行修改才能测试用过
*/

#define User_ESP8266_SSID     "Xiaomi201A"          //wifi名
#define User_ESP8266_PWD      "cai110123"      //wifi密码

#define TCPServer_IP     "101.37.89.157"     //服务器IP
#define TCPServer_PORT   "8086"      //服务器端口号

extern Byte8 _esp8266_flag;
#define esp8266_flag 					_esp8266_flag.Byte
#define esp8266_conn_flag 		_esp8266_flag.Bits.B0
#define esp8266_reg_flag 			_esp8266_flag.Bits.B1
#define esp8266_send_return 	_esp8266_flag.Bits.B2

void TCP_Program(void);
void TCP_Restart(void);
void TCP_TIM_1ms(void);
uint8_t TCP_InAT(void);
uint8_t TCP_gprs(void);
uint8_t TCP_config(void);
uint8_t TCP_Connect(void);// 建立TCP连接
uint8_t TCP_Restart_Program(void);
void TCP_Send_Data(char *data,  uint16_t len);
#endif

