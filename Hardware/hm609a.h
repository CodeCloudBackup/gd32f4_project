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
#define hm609a_connect_flag     g_hm609aFlag.Bits.B0    //网络连接成功标志
#define hm609a_reg_flag         g_hm609aFlag.Bits.B1    //注册成功标志
#define hm609a_send_return      g_hm609aFlag.Bits.B2    //等待发送返回标志

void HM609A_Send_At_Cmd(const u8* cmd,u8* ack,u32 time);
void HM609A_At_Restore(char *ret_ask);
void HM609A_Check_At_Response(const char* ret_ask);

void HM609A_Init(void);
u8 HM609A_Config(void);
u8 HM609A_Connect(int sockid, char* addr, int port);
void HM609A_Program(char* addr, int port);
#endif

