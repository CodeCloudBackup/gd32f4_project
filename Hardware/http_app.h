#ifndef __MQTT_APP_H
#define __MQTT_APP_H

#include "systick.h"
extern Byte8 HttpFlag;
#define HTTP_FLAG  			HttpFlag.Byte
#define HTTP_FLAG_EQUIP_CERT 			HttpFlag.Bits.B0 // ��������
#define HTTP_FLAG_DOWNLOAD_BIN		HttpFlag.Bits.B1 // ����������
#define MQTT_FLAG_UPLOAD_PHOTO		HttpFlag.Bits.B2 // ��Ͷ�ݿ�
#define MQTT_FLAG_UPLOAD_LOGFILE	HttpFlag.Bits.B3 // �ر�Ͷ�ݿ�

u8 HM609A_Http_Program(char* addr, u32 port, u32 up_port);

#endif
