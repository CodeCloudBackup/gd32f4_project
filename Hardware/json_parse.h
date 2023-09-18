#ifndef __JSON_PARSE_H
#define __JSON_PARSE_H

#include "cJSON.h"
#include "systick.h"

typedef struct{
	u32 mqtt_port;
	u32 http_port;
	char mqtt_ip[16];
	char http_ip[16];
}IP_INI;

typedef struct{
	u32 led_num;
	u32 ulterasonic_triger;
	u32 led;
}CNF_INI;

typedef struct{
	char *braCode;
	IP_INI ip_ini;
	CNF_INI con_ini;
}DEVICE_CONF;

typedef struct{
	char type[10];
	char action[10];
	char filename[20];
	char version[10];
}APP_UPGRADE;

typedef struct{
	char *braCode;
	char orderNo[10];
	u8 type;
}DELY_INFO;

typedef struct{
	u8 area_vacancy;
	u8 power_remain;
	u16 power_voltage;
	char* barCode;
	char cameSerialCode[12];
	char simCode[11];
	u8 csq;
	float lng;
	float lat;
	float acc[3];
	float gyro[3];
	float temp;
}DEVICE_STATUS;

extern APP_UPGRADE g_appUpgrade;
extern DEVICE_CONF g_sDeviceConf;
extern DELY_INFO g_sDelyInfo;
extern DEVICE_STATUS g_sDeviceSta;

void DeviceStatusJsonPackage(DEVICE_STATUS *dSta, char* out);
void AppConf_Init(char *mcu_id, char *buf);
void AppConfJsonParse(cJSON* root);
u8 ResetJsonParse(cJSON* root);
void UpgradeJsonParse(cJSON* root);
void DelyJsonParse(cJSON* root);
#endif
