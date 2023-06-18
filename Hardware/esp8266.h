#ifndef __ESP8266_H
#define __ESP8266_H 			   
#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include "systick.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
#if defined ( __CC_ARM   )
#pragma anon_unions
#endif



//ESP8266模式选择
typedef enum
{
    STA,
    AP,
    STA_AP  
}ENUM_Net_ModeTypeDef;

//网络传输层协议，枚举类型
typedef enum{
     enumTCP,
     enumUDP,
} ENUM_NetPro_TypeDef;
//连接号，指定为该连接号可以防止其他计算机访问同一端口而发生错误
typedef enum{
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

#define ESP8266_RST_Pin                   GPIO_PIN_13    //复位管脚
#define ESP8266_RST_Pin_Port              GPIOC    //复位 
#define ESP8266_RST_Pin_Periph_Clock()   rcu_periph_clock_enable(RCU_GPIOC)       //PC时钟




#define ESP8266_RST_Pin_SetH     gpio_bit_set(ESP8266_RST_Pin_Port, ESP8266_RST_Pin)
#define ESP8266_RST_Pin_SetL     gpio_bit_reset(ESP8266_RST_Pin_Port, ESP8266_RST_Pin)




#define ESP8266_USART(fmt, ...)  USART_printf (USART5, fmt, ##__VA_ARGS__)    
#define PC_USART(fmt, ...)       printf(fmt, ##__VA_ARGS__)       //这是串口打印函数，串口1，执行printf后会自动执行fput函数，重定向了printf。



#define RX_BUF_MAX_LEN 1024       //最大字节数
extern struct STRUCT_USART_Fram   //数据帧结构体
{
    char Data_RX_BUF[RX_BUF_MAX_LEN];
    union 
    {
        __IO u16 InfAll;
        struct 
        {
            __IO u16 FramLength       :15;                               // 14:0 
            __IO u16 FramFinishFlag   :1;                                // 15 
        }InfBit;
    }; 
	
}ESP8266_Fram_Record_Struct;


// 初始化和TCP功能函数
void ESP8266_Init(void);
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
uint8_t* ESP8266_Check_Cmd(uint8_t *str);
// 发送AT指令
bool ESP8266_Send_AT_Cmd(const uint8_t *cmd,uint8_t *ack,u32 time);
// 取消回显
void ESP8266_AT_ATEO(char *ret_ask);
// 设置波特率
void ESP8266_AT_BaudRate(char *ret_ask);
// 恢复出厂设置
void ESP8266_AT_Restore(char *ret_ask);
// 退出透传
bool ESP8266_AT_Quit_Trans(void);
// 收到AT指令应答数据并校验
bool ESP8266_Check_AT_Response(const char *ret_ask);

void ESP8266_Rst(void);

//获取ATK-ESP8266模块的连接状态
//返回值:0,未连接;1,连接成功.
u8 ESP8266_Consta_Check(void);

//选择ESP8266的工作模式
// enumMode 模式类型
//成功返回true，失败返回false
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode,char *ret_ask);

//ESP8266连接外部的WIFI
//pSSID WiFi帐号
//pPassWord WiFi密码
//设置成功返回true 反之false
bool ESP8266_AT_JoinAP(const char * pSSID, const char * pPassWord, char *res_ask);

//ESP8266 
//enumEnUnvarnishTx  是否多连接，bool类型
//设置成功返回true，反之false
bool ESP8266_Enable_MultipleId ( EventStatus enumEnUnvarnishTx, char *res_ask);

//ESP8266 连接服务器
//enumE  网络类型
//ip ，服务器IP
//ComNum  服务器端口
//id，连接号，确保通信不受外界干扰
//设置成功返回true，反之fasle
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id ,char* res_ask);

bool ESP8266_SendString(EventStatus enumEnUnvarnishTx,const char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId);
bool ESP8266_UnvarnishSend ( void );
void ESP8266_ExitUnvarnishSend ( void );
u8 ESP8266_Get_LinkStatus ( void );
void USART_printf( uint32_t USARTx, char * Data, ... );

//MQTT功能函数
bool ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord);
bool ESP8266_MQTTCONN( char * Ip, int Num);
bool ESP8266_MQTTSUB(char * Topic);
bool ESP8266_MQTTPUB( char * Topic,char *temp);
bool ESP8266_MQTTCLEAN(void);
bool MQTT_SendString(char * pTopic,char *temp2);

#endif
