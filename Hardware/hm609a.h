#ifndef __HM609A_H
#define __HM609A_H
#include "systick.h"

#define DG_PWRKEY PFout(11)	// DS1	
#define DG_PWRKEY_OFF DG_PWRKEY=1
#define DG_PWRKEY_ON  DG_PWRKEY=0

#define DG_RESET  PFout(12)	// DS0 
#define DG_RESET_OFF DG_RESET=1
#define DG_RESET_ON  DG_RESET=0

#define hm609a_flag             g_hm609aFlag.Byte
#define hm609a_config_flag     g_hm609aFlag.Bits.B0    //网络连接成功标志
#define hm609a_mqtt_conn_flag   g_hm609aFlag.Bits.B1 
#define hm609a_mqtt_reg_flag  	g_hm609aFlag.Bits.B2   //注册成功标志
#define hm609a_mqtt_wait_flag   g_hm609aFlag.Bits.B3

#define hm609a_http_conn_flag   g_hm609aFlag.Bits.B4    //等待发送返回标志
#define hm609a_http_wait_flag   g_hm609aFlag.Bits.B5    //等待发送返回标志


typedef enum
{
	MQTT_PROT=0,
	HTTP_PROT
}NET_PROT;

extern Byte8 g_hm609aFlag;

void  HM609A_Restart(void);
void HM609A_Init(void);
u8 HM609A_Config(void);
u8 HM609A_Connect(u8 sockid, char* addr, int port, NET_PROT protocol);
u8 HM609A_Tcp_Off(u8 sockid);// 关闭TCP连接
void HM609A_Tcp_Program(u8 sockid, char* addr, int port, NET_PROT protocol);
void HM609A_Send_Data(u8 sockid, const u8* data, u16 len,u8 flag, NET_PROT protocol);
#endif

