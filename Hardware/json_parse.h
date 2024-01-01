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
	char orderNo[10];
	u8 type;
}DELY_INFO;

typedef struct{
	DELY_INFO *dely_info;
	u8 count;
	u8 keySta;
}DELY_STA;

typedef struct{
	u8 area_vacancy;
	u8 power_remain;
	u16 power_voltage;
	char barCode[20];
	char cameSerialCode[20];
	char simCode[20];
	u8 csq;
	float lng;
	float lat;
	float acc[3];
	float gyro[3];
	u16 temp;
}DEVICE_STATUS;

typedef struct{
	char job_time[20];
	char job_id[10];
	char cam_id[2];
	u8 exposure;
	u8 brightness_thresh;
	u8 diff_thresh;
	char id[10];
	u8 method;
	u32 period_ns;
	u32 duty_ns;
}PHOTO_INFO;

extern APP_UPGRADE g_appUpgrade;
extern DEVICE_CONF g_sDeviceConf;
extern DELY_INFO g_sDelyInfo;
extern DELY_STA g_sDelySta;
extern DEVICE_STATUS *g_sDeviceSta;
extern PHOTO_INFO *g_sPhotoInfo;

void StructInit(void);
u32 DeviceStatusJsonPackage(const DEVICE_STATUS *dSta, char* out);
void DelyStatusJsonPackage(const DELY_STA *dSta, char* out);
u32 AppConfUploadJsonPackage(const DEVICE_CONF *dSta, char* out);
void AppConf_Init(char *mcu_id, char *buf);
void AppConfJsonParse(cJSON* root);
u8 ResetJsonParse(cJSON* root);
void UpgradeJsonParse(cJSON* root);
void DelyJsonParse(cJSON* root);
void PhotoJsonParse(cJSON* root);
u8 LockJsonParse(cJSON* root);
#endif
