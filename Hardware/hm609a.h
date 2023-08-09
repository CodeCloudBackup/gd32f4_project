#ifndef __HM609A_H
#define __HM609A_H
#include "systick.h"

void HM609A_Init(void);
void HM609A_Send_At_Cmd(const u8* cmd,u8* ack,u32 time);
void HM609A_At_Restore(char *ret_ask);
void HM609A_Check_At_Response(const char* ret_ask);
void HM609A_Rst(void);
#endif

