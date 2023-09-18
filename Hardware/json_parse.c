#include "json_parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
DEVICE_CONF g_sDeviceConf;
DELY_INFO g_sDelyInfo;
DEVICE_STATUS g_sDeviceSta;

void DeviceStatusJsonPackage(DEVICE_STATUS *dSta, char* out)
{
	if(out == NULL) 
		return;
	sprintf(out, "{"
	"\"area_vacancy\":%d,"
	"\"power_remain\":%d,"
	"\"power_voltage\":%d,"
	"\"barCode\":\"%s\","
	"\"cameSerialCode\":\"%s\","
	"\"simCode\":\"%s\","
	"\"csq\":%d,"
	"\"lng\":%.4f,"
	"\"lat\":%.4f,"
	"\"acc\":%.2f %.2f %.2f,"
	"\"gyro\":%.2f %.2f %.2f,"
	"\"temp\":%.2f}",
	dSta->area_vacancy,dSta->power_remain,
	dSta->power_voltage,dSta->barCode,
	dSta->cameSerialCode,dSta->simCode,
	dSta->csq,dSta->lng,dSta->lat,
	dSta->acc[0],dSta->acc[1],dSta->acc[2],
	dSta->gyro[0],dSta->gyro[1],dSta->gyro[2],
	dSta->temp
	);
}


void AppConf_Init(char *mcu_id, char *buf)
{
	g_sDeviceConf.braCode=mcu_id;
	sprintf(g_sDeviceConf.ip_ini.http_ip, "183.129.134.242");
	sprintf(g_sDeviceConf.ip_ini.mqtt_ip, "101.37.89.157");
	g_sDeviceConf.ip_ini.http_port=7091;
	g_sDeviceConf.ip_ini.mqtt_port=1883;
	g_sDeviceConf.con_ini.led=0;
	g_sDeviceConf.con_ini.led_num=0;
	g_sDeviceConf.con_ini.ulterasonic_triger=0;
	
	sprintf(buf,"{\r\n"
	"\"barCode\":\"%s\",\r\n"
	"\"data\":{\r\n"
	"\"ip.ini\":{\"PORT\":{\"port_mqtt\":%d, \"port_http\":%d},"
							"\"IP\":{\"mqtt_ip\":\"%s\",\"http_ip\":\"%s\"}},\r\n"
	"\"cnf.ini\":{\"COMMON\":{\"ledNum\":%d},"
							 "\"GPIO\":{\"ultrasonic_triger2\": 22,\"led\":32}}\r\n"
	"}\r\n}\r\n",
	mcu_id,g_sDeviceConf.ip_ini.mqtt_port,g_sDeviceConf.ip_ini.http_port,
	g_sDeviceConf.ip_ini.mqtt_ip,g_sDeviceConf.ip_ini.http_ip,
	g_sDeviceConf.con_ini.led_num);
	
	printf("app conf init success.\r\n");
	//printf("%s",buf);
}

void AppConfJsonParse(cJSON* root)
{
  cJSON* item = NULL;
	cJSON* ap_ini = NULL;
	cJSON* cnf_ini = NULL;
  item = cJSON_GetObjectItem(root, "data");
	if(!item)
	{
		printf("Err Data NULL\r\n");
	}

	ap_ini = cJSON_GetObjectItem(item, "ip.ini");
	if(!ap_ini)
	{
		printf("Err Ip.ini NULL\r\n");
	} 
	else 
	{
		cJSON* port = NULL;
		cJSON* ip = NULL;
		port = cJSON_GetObjectItem(ap_ini, "PORT");
		if(port != NULL)
		{
			cJSON* mqtt_port = NULL;
			cJSON* http_port = NULL;
			mqtt_port = cJSON_GetObjectItem(port, "port_mqtt");
			if(mqtt_port != NULL )
			{
				g_sDeviceConf.ip_ini.mqtt_port=mqtt_port->valueint;
				printf("mqtt port %d\r\n",g_sDeviceConf.ip_ini.mqtt_port);
			}
			http_port = cJSON_GetObjectItem(port, "port_http");
			if(http_port != NULL )
			{
				g_sDeviceConf.ip_ini.http_port=http_port->valueint;
				printf("http port %d\r\n",g_sDeviceConf.ip_ini.http_port);
			}
		}
		ip = cJSON_GetObjectItem(ap_ini, "IP");
		if(ip != NULL)
		{
			cJSON* mqtt_ip = NULL;
			cJSON* http_ip = NULL;
			mqtt_ip = cJSON_GetObjectItem(ip, "mqtt_ip");
			if(mqtt_ip != NULL && mqtt_ip->type == cJSON_String)
			{
				memcpy(g_sDeviceConf.ip_ini.mqtt_ip, \
					mqtt_ip->valuestring, strlen(mqtt_ip->valuestring));
				printf("mqtt ip %s\r\n",g_sDeviceConf.ip_ini.mqtt_ip);
			}
			http_ip = cJSON_GetObjectItem(ip, "http_ip");
			if(http_ip != NULL && http_ip->type == cJSON_String)
			{
				memcpy(g_sDeviceConf.ip_ini.http_ip, \
					 http_ip->valuestring, strlen(http_ip->valuestring));
				printf("http ip %s\r\n",g_sDeviceConf.ip_ini.http_ip);
			}
		}
	}

	cnf_ini = cJSON_GetObjectItem(item, "cnf.ini");
	if(!cnf_ini)
	{
		printf("Err Cnf.ini NULL\r\n");
	}
	else
	{
		cJSON* common = NULL;
		cJSON* gpio = NULL;	
		common = cJSON_GetObjectItem(cnf_ini, "COMMON");
		if(common != NULL)
		{
			cJSON* ledNum = NULL;
			ledNum = cJSON_GetObjectItem(common, "COMMON");
			if(ledNum != NULL && ledNum->type == cJSON_String)
			{
				g_sDeviceConf.con_ini.led_num=atoi(ledNum->valuestring);
			}
		}
		gpio = cJSON_GetObjectItem(cnf_ini, "GPIO");
		if(gpio != NULL)
		{
			cJSON* ut = NULL;
			cJSON* led = NULL;
			ut = cJSON_GetObjectItem(gpio, "ultrasonic_triger2");
			if(ut != NULL && ut->type == cJSON_String)
			{
				g_sDeviceConf.con_ini.ulterasonic_triger=atoi(ut->valuestring);
			}
			led = cJSON_GetObjectItem(gpio, "led");
			if(led != NULL && led->type == cJSON_String)
			{
				g_sDeviceConf.con_ini.led=atoi(led->valuestring);
			}
		}
	}

}


void DelyJsonParse(cJSON* root)
{
	cJSON* barCode=NULL;
	cJSON* orderNo=NULL;
	cJSON* type=NULL;

	barCode=cJSON_GetObjectItem(root, "barCode");
	if(barCode != NULL && barCode->type == cJSON_String)
    {
		memcpy(g_sDelyInfo.braCode, \
				barCode->valuestring, strlen(barCode->valuestring));
	}
	orderNo=cJSON_GetObjectItem(root, "orderNo");
	if(orderNo != NULL && orderNo->type == cJSON_String)
    {
		memcpy(g_sDelyInfo.orderNo, \
				orderNo->valuestring, strlen(orderNo->valuestring));
	}
	type=cJSON_GetObjectItem(root, "type");
	if(type != NULL && type->type == cJSON_String)
    {
		g_sDelyInfo.type=atoi(type->valuestring);
	}
}

u8 ResetJsonParse(cJSON* root)
{
    cJSON* reset = NULL;
    u8 type=0;
    reset = cJSON_GetObjectItem(root, "type");
    if(reset != NULL && reset->type == cJSON_String)
    {
        type=atoi(reset->valuestring);
    }
    return type;
}

APP_UPGRADE g_appUpgrade;

void UpgradeJsonParse(cJSON* root)
{
    cJSON* upgrade_type = NULL;
	cJSON* action = NULL;
	cJSON* filename = NULL;
	cJSON* version = NULL;
   
    upgrade_type = cJSON_GetObjectItem(root, "upgrade_type");
    if(upgrade_type != NULL && upgrade_type->type == cJSON_String)
    {
        memcpy(g_appUpgrade.type, \
				upgrade_type->valuestring, strlen(upgrade_type->valuestring));
    }
		 action = cJSON_GetObjectItem(root, "action");
    if(action != NULL && action->type == cJSON_String)
    {
        memcpy(g_appUpgrade.action, \
				action->valuestring, strlen(action->valuestring));
    }
		 filename = cJSON_GetObjectItem(root, "filename");
    if(filename != NULL && filename->type == cJSON_String)
    {
        memcpy(g_appUpgrade.filename, \
				filename->valuestring, strlen(filename->valuestring));
    }
		 version = cJSON_GetObjectItem(root, "version");
    if(version != NULL && version->type == cJSON_String)
    {
        memcpy(g_appUpgrade.version, \
				version->valuestring, strlen(version->valuestring));
    }
}

