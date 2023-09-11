#include "json_parse.h"

DEVICE_CONF g_sDeviceConf;

void AppConf_Init(char *mcu_id)
{
	g_sDeviceConf.braCode=mcu_id;
	sprintf(g_sDeviceConf.ip_ini.http_ip, "127.0.0.1");
	sprintf(g_sDeviceConf.ip_ini.mqtt_ip, "127.0.0.1");
	g_sDeviceConf.ip_ini.http_port=80;
	g_sDeviceConf.ip_ini.mqtt_port=1883;
	g_sDeviceConf.con_ini.led=0;
	g_sDeviceConf.con_ini.led_num=0;
	g_sDeviceConf.con_ini.ulterasonic_triger=0;
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
			if(mqtt_port != NULL && mqtt_port->type == cJSON_String)
			{
				g_sDeviceConf.ip_ini.mqtt_port=atoi(mqtt_port->valuestring);
				printf("mqtt port %d\r\n",g_sDeviceConf.ip_ini.mqtt_port);
			}
			http_port = cJSON_GetObjectItem(port, "port_http");
			if(http_port != NULL && http_port->type == cJSON_String)
			{
				g_sDeviceConf.ip_ini.http_port=atoi(http_port->valuestring);
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
					mqtt_ip->string, strlen(mqtt_ip->string));
				printf("mqtt ip %s\r\n",g_sDeviceConf.ip_ini.mqtt_ip);
			}
			http_ip = cJSON_GetObjectItem(ip, "http_ip");
			if(http_ip != NULL && http_ip->type == cJSON_String)
			{
				memcpy(g_sDeviceConf.ip_ini.mqtt_ip, \
					 mqtt_ip->string, strlen(mqtt_ip->string));
				printf("http ip %d\r\n",g_sDeviceConf.ip_ini.http_ip);
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


void UpgradeJsonParse(cJSON* root)
{
    cJSON* upgrade = NULL;
    char update_type[10];
    char action[10];
    char filename[20];
    char version[10];
    upgrade = cJSON_GetObjectItem(root, "type");
    if(reset != NULL && reset->type == cJSON_String)
    {
        type=atoi(reset->valuestring);
    }
    return type;
}

