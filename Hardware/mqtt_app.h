#ifndef __MQTT_APP_H
#define __MQTT_APP_H

#include "systick.h"
#include "MQTTPacket.h"
#include "cJSON.h"

extern Byte8 MqttSubscrTopFlag1;
extern Byte8 MqttSubscrTopFlag2;
extern Byte8 MqttSubscrTopFlag3;

#define MQTT_FLAG1  			MqttSubscrTopFlag1.Byte
#define MQTT_FLAG_PHOTO 					MqttSubscrTopFlag1.Bits.B0 // 发起拍照
#define MQTT_FLAG_PHOTO_THIRD 		MqttSubscrTopFlag1.Bits.B1 // 第三方拍照
#define MQTT_FLAG_DELY_OPEN				MqttSubscrTopFlag1.Bits.B2 // 打开投递口
#define MQTT_FLAG_DELY_CLOSE			MqttSubscrTopFlag1.Bits.B3 // 关闭投递口
#define MQTT_FLAG_LOCK_OPEN				MqttSubscrTopFlag1.Bits.B4	// 打开运维锁
#define MQTT_FLAG_LOCK_CLOSE			MqttSubscrTopFlag1.Bits.B5	// 关闭运维锁
#define MQTT_FLAG_CFG_DOWNLOAD 		MqttSubscrTopFlag1.Bits.B6	// 下发配置
#define MQTT_FLAG_CFG_UPLOAD 			MqttSubscrTopFlag1.Bits.B7	  // 上传配置

#define MQTT_FLAG2				MqttSubscrTopFlag2.Byte
#define MQTT_FLAG_RESTART					MqttSubscrTopFlag2.Bits.B0	// 设备重启
#define MQTT_FLAG_UPGRADE					MqttSubscrTopFlag2.Bits.B1	// APP升级
#define MQTT_FLAG_LOG_LIST				MqttSubscrTopFlag2.Bits.B2	// 日志列表
#define MQTT_FLAG_LOG_INFO				MqttSubscrTopFlag2.Bits.B3	// 日志信息
#define MQTT_FLAG_UP_ALARM				MqttSubscrTopFlag2.Bits.B4	// 上传告警
#define MQTT_FLAG_UP_APPVERSION		MqttSubscrTopFlag2.Bits.B5	// 上传app版本
#define MQTT_FLAG_UP_LOG_LIST			MqttSubscrTopFlag2.Bits.B6	// 上传日志列表
#define MQTT_FLAG_RECEIVE   			MqttSubscrTopFlag2.Bits.B7	// 接收到服务器发布数据

#define MQTT_FLAG3				MqttSubscrTopFlag3.Byte
#define MQTT_FLAG_UP_PHOTO_RES					MqttSubscrTopFlag3.Bits.B0	// 上传拍照结果
#define MQTT_FLAG_UP_DELY_OPEN_RES			MqttSubscrTopFlag3.Bits.B1	// 上传传递口结果
#define MQTT_FLAG_UP_DELY_STA						MqttSubscrTopFlag3.Bits.B2	// 上传传递口状态
#define MQTT_FLAG_UP_LOCK_STA						MqttSubscrTopFlag3.Bits.B3	// 上传运维锁状态
#define MQTT_FLAG_UP_DEVICE_STA					MqttSubscrTopFlag3.Bits.B4	// 上传设备状态
#define MQTT_FLAG_UP_CAMERA_STA					MqttSubscrTopFlag3.Bits.B5	// 上传相机状态
#define MQTT_FLAG_UP_PARAM_RES					MqttSubscrTopFlag3.Bits.B6	// 上传设备状态
#define MQTT_FLAG_UP_PARAM							MqttSubscrTopFlag3.Bits.B7	// 上传设备参数

void MQTT_Publish(u8 sockid, u8 dup, u8 retained, int qos, char* topic, char* json_buf);
void MQTT_HeartBeat(u8 sockid);
void MQTT_Subscribe(u8 sockid);
void MQTT_Publish_Analysis_Json(u8* buf, cJSON *json);
void Mqtt_TIM_10ms(void);
void MQTT_Init( char* chip_id);
int transport_getdata(u8* buf, int count);
u8 Mqtt_Connack_Deserialize( u8* buf);
u8 Mqtt_Suback_Deserialize( u8* buf);
u8 Mqtt_Publish_Deserialize( u8* buf,u8* out);
u8 Mqtt_Deserialize_Handle(u8* msg_type, u8* buf, u8* out);
u8 HM609A_Mqtt_Program(u8 sockid);

#endif
