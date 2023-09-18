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
#include "gdflash.h"
#include "json_parse.h"

#define BATTERY_CAP  7000  				// ��ر�׼���ￄ1�7 ��λmAH
#define BATTERY_VLOT  7.0    				// ��ر�׼��ф1�7
#define FLASH_CUR_CONFIG_ADDR  0x08008000 	// 设置FLASH 保存地址(必须为偶数，且所在扇匄1�7,要大于本代码扢�占用到的扇区.
#define FLASH_BIN_CONFIG_ADDR  0x0800C000 	// 设置FLASH 保存地址
#define FLASH_APPBIN_ADDR      0x08020000 	// 设置FLASH BIN文件保存地址.

extern Byte8 ProgramFlag;
#define PROGRAM_FLAG  			ProgramFlag.Byte
#define PROGRAM_TAKE_PHOTO_FLAG 	ProgramFlag.Bits.B0  // ��������
#define PROGRAM_ICM_DATA_FLAG 		ProgramFlag.Bits.B1  // ����������
#define PROGRAM_OPEN_DELY_FLAG		ProgramFlag.Bits.B2  // ��Ͷ�ݿ�
#define PROGRAM_CLOSE_DELY_FLAG		ProgramFlag.Bits.B3 
#define PROGRAM_RESERT_FLAG			ProgramFlag.Bits.B4  // �ر�Ͷ�ݿ�
#define PROGRAM_APPFLASH_FLAG		ProgramFlag.Bits.B5	 // ����ά��
#define PROGRAM_SPEAKER_FLAG		ProgramFlag.Bits.B6	 // �ر���ά��

void Program_Flag_Init(void);
void Data_Recv_Program(void);
void MQTT_Data_Program(void);
void HTTP_Data_Program(void);
void Program_Init(void);
void Device_Program(void);
#endif
