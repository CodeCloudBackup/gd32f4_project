#ifndef __MQTT_APP_H
#define __MQTT_APP_H

#include "systick.h"
#include "MQTTPacket.h"
#include "cJSON.h"

extern Byte8 MqttSubscrTopFlag1;
extern Byte8 MqttSubscrTopFlag2;

#define MQTT_FLAG1  			MqttSubscrTopFlag1.Byte
#define MQTT_FLAG_PHOTO 				MqttSubscrTopFlag1.Bits.B0 // 发起拍照
#define MQTT_FLAG_PHOTO_THIRD 	MqttSubscrTopFlag1.Bits.B1 // 第三方拍照
#define MQTT_FLAG_DELIVER_OPEN	MqttSubscrTopFlag1.Bits.B2 // 打开投递口
#define MQTT_FLAG_DELIVER_CLOSE	MqttSubscrTopFlag1.Bits.B3 // 关闭投递口
#define MQTT_FLAG_LOCK_OPEN			MqttSubscrTopFlag1.Bits.B4	// 打开运维锁
#define MQTT_FLAG_LOCK_CLOSE		MqttSubscrTopFlag1.Bits.B5	// 关闭运维锁
#define MQTT_FLAG_CONFIG_DOWNLOAD MqttSubscrTopFlag1.Bits.B6	// 下发配置
#define MQTT_FLAG_CONFIG_UPLOAD MqttSubscrTopFlag1.Bits.B7	// 上传配置

#define MQTT_FLAG2				MqttSubscrTopFlag2.Byte
#define MQTT_FLAG_RESTART			MqttSubscrTopFlag2.Bits.B0	// 设备重启
#define MQTT_FLAG_UPGRADE			MqttSubscrTopFlag2.Bits.B1	// APP升级
#define MQTT_FLAG_LOG_LIST		MqttSubscrTopFlag2.Bits.B2	// 日志列表
#define MQTT_FLAG_LOG_INFO		MqttSubscrTopFlag2.Bits.B3	// 日志信息
#define MQTT_FLAG_REVICE   		MqttSubscrTopFlag2.Bits.B7

void MQTT_Publish(u8 dup, u8 retained, int qos);
void MQTT_HeartBeat(void);
void MQTT_Subscribe(void);
void MQTT_Publish_Analysis_Json(u8* buf, cJSON *json);
void Mqtt_TIM_10ms(void);
void MQTT_Init( char* chip_id);
int transport_getdata(unsigned char* buf, int count);
u8 Mqtt_Connack_Deserialize( u8* buf);
u8 Mqtt_Suback_Deserialize( u8* buf);
u8 Mqtt_Publish_Deserialize( u8* buf,u8* out);
u8 Mqtt_Deserialize_Handle(u8* msg_type,const u8* buf, u8* out);
u8 HM609A_Mqtt_Program(char* addr, int port);

#endif
