#include "json_parse.h"
#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DEVICE_CONF g_sDeviceConf;
DEVICE_STATUS* g_sDeviceSta =NULL;
PHOTO_INFO *g_sPhotoInfo = NULL;
DELY_INFO g_sDelyInfo;
DELY_STA g_sDelySta;

extern char *g_barCode;
void StructInit(void)
{
	if(g_sDeviceSta==NULL)
	{
		g_sDeviceSta=(DEVICE_STATUS*)mymalloc(SRAMIN, sizeof(DEVICE_STATUS));
	}
	
	if(g_sPhotoInfo==NULL)
	{
		g_sPhotoInfo=(PHOTO_INFO*)mymalloc(SRAMIN, sizeof(PHOTO_INFO));
	}
	memcpy(g_sDelyInfo.orderNo,"0",2);
	g_sDelyInfo.type=0;
	
	g_sDelySta.dely_info=&g_sDelyInfo;
	g_sDelySta.count=0;
	g_sDelySta.keySta=0;
}


u32 DeviceStatusJsonPackage(DEVICE_STATUS *dSta, char* out)
{
	u32 len=0;
	if(out == NULL) 
		return 0;
	len = sprintf(out, "{"
	"%d"
	":%d"
	":%d"
	":%s"
//	"\"cameSerialCode\":\"xx\","
//	"\"simCode\":\"%s\","
	":%d"
	":%.4f"
	":%.4f"
	":%.2f"
	":%.2f"
	":%.2f}",
	dSta->area_vacancy,
	dSta->power_remain,dSta->power_voltage,
	g_barCode,
	dSta->csq,dSta->lng,dSta->lat,
	dSta->acc[0],
	dSta->gyro[0],
	dSta->temp
	);
	out[len] = '\0';
	return len;
}

void DelyInfoJsonPackage(DEVICE_STATUS *dSta, char* out)
{
		
}

void DelyStatusJsonPackage(DELY_STA *dSta, char* out)
{
		u32 len=0;
	if(out == NULL) 
		return;
	len = sprintf(out, 
		"{%s:%s:%d:%d:%d}",
		g_barCode,dSta->dely_info->orderNo,
		dSta->dely_info->type,
		dSta->count,dSta->keySta
		);
		out[len] = '\0';
}

void AppConf_Init(char *mcu_id, char *buf)
{
//	g_sDeviceConf.braCode=mcu_id;
	sprintf(g_sDeviceConf.ip_ini.http_ip, "101.37.89.157");
	sprintf(g_sDeviceConf.ip_ini.mqtt_ip, "101.37.89.157");
	g_sDeviceConf.ip_ini.http_port=80;
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
	g_barCode,g_sDeviceConf.ip_ini.mqtt_port,g_sDeviceConf.ip_ini.http_port,
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
	cJSON* barCodeNode=NULL;
	cJSON* orderNo=NULL;
	cJSON* type=NULL;
	
	barCodeNode=cJSON_GetObjectItem(root, "barCode");
	if(barCodeNode != NULL && barCodeNode->type == cJSON_String)
  {
//		memcpy(g_sDelyInfo.braCode, \
//				barCodeNode->valuestring, strlen(barCodeNode->valuestring));
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
	printf("Server dely: barcodeNode:%s,orderNo:%s,type:%d ",
		barCodeNode->valuestring,g_sDelyInfo.orderNo,g_sDelyInfo.type);
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
		printf("Server: reset type:%d",type);
    return type;
}

u8 LockJsonParse(cJSON* root)
{
    cJSON* lock = NULL;
    u8 type=0;
    lock = cJSON_GetObjectItem(root, "type");
    if(lock != NULL && lock->type == cJSON_String)
    {
        type=atoi(lock->valuestring);
    }
		printf("Server: lock type:%d",type);
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
		printf("Server update: :type:%s, action:%s"
				"filename:%s,version:%s ",
					g_appUpgrade.type,g_appUpgrade.action,
					g_appUpgrade.filename,g_appUpgrade.version);
}

void PhotoJsonParse(cJSON* root)
{
		cJSON* child = NULL;
		child = cJSON_GetObjectItem(root, "job_time");
		if(child != NULL && child->type == cJSON_String)
		{
			memcpy(g_sPhotoInfo->job_time, \
				child->valuestring, strlen(child->valuestring));
		}
		child = cJSON_GetObjectItem(root, "job_id");
		if(child != NULL && child->type == cJSON_String)
		{
					memcpy(g_sPhotoInfo->job_id, \
					child->valuestring, strlen(child->valuestring));
		}
		child = cJSON_GetObjectItem(root, "cam_id");
		if(child != NULL && child->type == cJSON_String)
		{
			memcpy(g_sPhotoInfo->cam_id, \
				child->valuestring, strlen(child->valuestring));
		}
		child = cJSON_GetObjectItem(root, "exposure");
		if(child != NULL && child->type == cJSON_Number)
		{
				g_sPhotoInfo->exposure = child->valueint;
		}
		child = cJSON_GetObjectItem(root, "brightness_thresh");
		if(child != NULL && child->type == cJSON_Number)
		{
				g_sPhotoInfo->brightness_thresh=child->valueint; 
		}
		child = cJSON_GetObjectItem(root, "diff_thresh");
		if(child != NULL && child->type == cJSON_Number)
		{
				g_sPhotoInfo->diff_thresh=child->valueint;
		}
		child = cJSON_GetObjectItem(root, "id");
		if(child != NULL && child->type == cJSON_String)
		{
				memcpy(g_sPhotoInfo->id, \
					child->valuestring, strlen(child->valuestring));
		}
		child = cJSON_GetObjectItem(root, "method");
		if(child != NULL && child->type == cJSON_Number)
		{
				g_sPhotoInfo->method=child->valueint;
		}
		child = cJSON_GetObjectItem(root, "period_ns");
		if(child != NULL && child->type == cJSON_Number)
		{
				g_sPhotoInfo->period_ns=child->valueint;
		}
		child = cJSON_GetObjectItem(root, "duty_ns");
		if(child != NULL && child->type == cJSON_Number)
		{
				g_sPhotoInfo->duty_ns=child->valueint;
		}
		printf("Server update: :job_time:%s, job_id:%s"
			"cam_id:%s,exposure:%d, bnt:%d"
			"dt:%d,id:%s,method:%d,period_ns:%d,duty_ns:%d",
			g_sPhotoInfo->job_time,g_sPhotoInfo->job_id,
			g_sPhotoInfo->cam_id,g_sPhotoInfo->exposure,
			g_sPhotoInfo->brightness_thresh,g_sPhotoInfo->diff_thresh,
			g_sPhotoInfo->id,g_sPhotoInfo->method,
			g_sPhotoInfo->period_ns,g_sPhotoInfo->duty_ns
		);
}
