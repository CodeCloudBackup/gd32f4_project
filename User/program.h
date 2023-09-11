#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

#include "led.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "dac.h"
#include "dcmi.h"
#include "ov2640.h"
#include "sccb.h"
#include "malloc.h"
#include "adc.h"
#include "hm609a.h"	
#include "mos_sw.h"
#include "icm42670.h"
#include "spi.h"
#include "f35sqa512m.h"
#include "mqtt_app.h"
#include "http_app.h"
#include "cJSON.h"

#define BATTERY_CAP  7000  // 电池标准电量 单位mAH
#define BATTERY_VLOT  7    // 电池标准电压

typedef struct{
	u8 area_vacancy;
	u8 power_remain;
	u16 power_voltage;
	char cameSerialCode[12];
	char *simCode;
	u8 csq;
	float lng;
	float lat;
	float acc[3];
	float gyro[3];
	float temp;
	
}DEVICE_STATUS;

extern DEVICE_STATUS device_sta;
void Data_Program(void);
void MQTT_Data_Program(void);
void Program_Init(void);
#endif
