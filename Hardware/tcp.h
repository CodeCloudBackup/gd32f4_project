#ifndef __TCP_H
#define __TCP_H 			   
#include "gd32f4xx.h"
#include "systick.h"
/*
*���²�����Ҫ�û������޸Ĳ��ܲ����ù�
*/

#define User_ESP8266_SSID     "Xiaomi_401"          //wifi��
#define User_ESP8266_PWD      "cai110123"      //wifi����

#define TCPServer_IP     "101.37.89.157"     //������IP
#define TCPServer_PORT   "80"      //�������˿ں�

extern Byte8 _esp8266_flag;
#define esp8266_flag 					_esp8266_flag.Byte
#define esp8266_conn_flag 		_esp8266_flag.Bits.B0
#define esp8266_reg_flag 			_esp8266_flag.Bits.B1
#define esp8266_send_return 	_esp8266_flag.Bits.B2

extern 

void TCP_Program(void);
void TCP_Restart(void);
void TCP_TIM_1ms(void);
u8 TCP_InAT(void);
u8 TCP_gprs(void);
u8 TCP_config(void);
u8 TCP_Connect(void);// ����TCP����
u8 TCP_Restart_Program(void);
BOOL TCP_Send_Data(char *data,  u16 len);
#endif

