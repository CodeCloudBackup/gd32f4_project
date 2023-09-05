#ifndef __HM609A_H
#define __HM609A_H
#include "systick.h"

#define DG_PWRKEY PFout(11)	// DS1	
#define DG_PWRKEY_OFF DG_RESET=1
#define DG_PWRKEY_ON  DG_RESET=0

#define DG_RESET  PFout(12)	// DS0 
#define DG_RESET_OFF DG_RESET=1
#define DG_RESET_ON  DG_RESET=0

#define hm609a_flag             g_hm609aFlag.Byte
#define hm609a_connect_flag     g_hm609aFlag.Bits.B0    //�������ӳɹ���־
#define hm609a_mqtt_conn_flag   g_hm609aFlag.Bits.B1    //ע��ɹ���־
#define hm609a_send_return      g_hm609aFlag.Bits.B2    //�ȴ����ͷ��ر�־
#define hm609a_http_conn_flag   g_hm609aFlag.Bits.B3    //�ȴ����ͷ��ر�־
#define hm609a_http_return_flag   g_hm609aFlag.Bits.B3

extern Byte8 g_hm609aFlag;

void  HM609A_Restart(void);
void HM609A_Init(void);
u8 HM609A_Config(void);
u8 HM609A_Connect(u8 sockid, char* addr, int port);
u8 HM609A_Tcp_Off(u8 sockid);// �ر�TCP����
void HM609A_Tcp_Program(u8 sockid, char* addr, int port);
void HM609A_Send_Data(u8 sockid, u8* data, u16 len,u8 flag);
#endif

