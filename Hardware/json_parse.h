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
	u8 *braCode;
	IP_INI ip_ini;
	CNF_INI con_ini;
}DEVICE_CONF;

extern DEVICE_CONF g_sDeviceConf;
#endif
