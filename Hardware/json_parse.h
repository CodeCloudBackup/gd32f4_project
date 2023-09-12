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

extern APP_UPGRADE g_appUpgrade;
extern DEVICE_CONF g_sDeviceConf;
extern DELY_INFO dely_info;

void AppConf_Init(char *mcu_id, char *buf);
void AppConfJsonParse(cJSON* root);
u8 ResetJsonParse(cJSON* root);
void UpgradeJsonParse(cJSON* root);
#endif
