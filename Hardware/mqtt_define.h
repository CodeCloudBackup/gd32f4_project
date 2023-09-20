#ifndef __MQTT_DEFINE_H
#define __MQTT_DEFINE_H
#include "systick.h"

extern char *g_barCode;

typedef struct 
{
	char barCode[20];
	char job_time[15];
	char job_id[2];
	char cam_id[12];
	u32 exposure;
	u32 period_ns;
	u32 duty_ns;
	u8 job_result_id;
	u8 brightness_thresh;
	u8 diff_thresh;
	u8 method;
}TAKEPHOTO;

typedef struct 
{
	char barCode[20];
	char iob_job_result[5];
	char id;
	char cam_ID[12];
	char cap_time[20];
	char format[5];
	char resolution[10];
	u8 brightness;
  u8 frame_diff;
	u8 error_code;
	u8 method;
	u8 gradient;	
	u32 exposure;
	u32	size;
	u32 power_remain;
	char file_name[40];
}PHOTO;

typedef struct 
{
	char job_id[2];
	char order_no[10];
	char job_time[15];
	char cam_id[12];
	u32 exposure;
	u32 period_ns;
	u32 duty_ns;
	u8 method;
	u8 category;
	u8 confidence;
	u8 diff_switch;
	u8 box_action;
	u8 brightness_thresh;
	u8 diff_thresh;
}THIRD_TAKE_PHOTO;

typedef struct 
{
	char barCode[20];
	u32 orderNo;
}OPEN_DELIVER;

typedef struct 
{
	char barCode[20];
	u32 orderNo;
	int count;
	u8 type;
}CLOSE_DELIVER;

typedef struct 
{
	u8 type;
}OPEN_LOCK;

typedef struct 
{
	u16 port_mqtt;
	u16 port_sock;
}PORT;

typedef struct 
{
	char mqtt_ip[16];
	char http_ip[16];
}IP;

typedef struct 
{
	PORT port;
	IP ip;
}IP_INI;

typedef struct 
{
	u16 ledNum;
	u16 ultrasonic_triger2;
	u16 led;
}CONFIG_INI;

typedef struct 
{
	IP_INI ip_ini;
	CONFIG_INI conf_ini;
}CONFIG_DATA;

typedef struct 
{
	char req[10];	
	CONFIG_DATA data;
}CONFIG;

typedef struct 
{
	char req[10];
	char result[5];
	char barCode[20];
}CONFIG_RESULT;

typedef struct 
{
	u8 update_type;
	u8 action;
	char filename[20];
	char version[10];
}APP_UPGRADE;

// …Ë±∏∂À
typedef struct 
{
	char barCode[20];
	u32 orderNo;
	u8 type;
}DELIVER_RESULT;

typedef struct 
{
	char barCode[20];
	u32 orderNo;
	int count;
	u8 type;
	u8 keyOpenStatus;
}DELIVER_STATUS;

typedef struct 
{
	char context[20];
	u8 area_vacancy;
	u8 power_remain;
	u32 power_voltage;
	char barCode[20];
}DEVICE_ALERT;



typedef struct 
{
	char barCode[20];
	u8 video0;
	u8 video1;
}CAMERA_STATUS;

typedef struct 
{
	char barCode[20];
	char version[10];
	char buildDate[12];
	char buildDate[20];
}VERSION;

#endif
