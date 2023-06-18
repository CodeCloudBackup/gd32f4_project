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



//ESP8266ģʽѡ��
typedef enum
{
    STA,
    AP,
    STA_AP  
}ENUM_Net_ModeTypeDef;

//���紫���Э�飬ö������
typedef enum{
     enumTCP,
     enumUDP,
} ENUM_NetPro_TypeDef;
//���Ӻţ�ָ��Ϊ�����Ӻſ��Է�ֹ�������������ͬһ�˿ڶ���������
typedef enum{
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;

#define ESP8266_RST_Pin                   GPIO_PIN_13    //��λ�ܽ�
#define ESP8266_RST_Pin_Port              GPIOC    //��λ 
#define ESP8266_RST_Pin_Periph_Clock()   rcu_periph_clock_enable(RCU_GPIOC)       //PCʱ��




#define ESP8266_RST_Pin_SetH     gpio_bit_set(ESP8266_RST_Pin_Port, ESP8266_RST_Pin)
#define ESP8266_RST_Pin_SetL     gpio_bit_reset(ESP8266_RST_Pin_Port, ESP8266_RST_Pin)




#define ESP8266_USART(fmt, ...)  USART_printf (USART5, fmt, ##__VA_ARGS__)    
#define PC_USART(fmt, ...)       printf(fmt, ##__VA_ARGS__)       //���Ǵ��ڴ�ӡ����������1��ִ��printf����Զ�ִ��fput�������ض�����printf��



#define RX_BUF_MAX_LEN 1024       //����ֽ���
extern struct STRUCT_USART_Fram   //����֡�ṹ��
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


// ��ʼ����TCP���ܺ���
void ESP8266_Init(void);
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
uint8_t* ESP8266_Check_Cmd(uint8_t *str);
// ����ATָ��
bool ESP8266_Send_AT_Cmd(const uint8_t *cmd,uint8_t *ack,u32 time);
// ȡ������
void ESP8266_AT_ATEO(char *ret_ask);
// ���ò�����
void ESP8266_AT_BaudRate(char *ret_ask);
// �ָ���������
void ESP8266_AT_Restore(char *ret_ask);
// �˳�͸��
bool ESP8266_AT_Quit_Trans(void);
// �յ�ATָ��Ӧ�����ݲ�У��
bool ESP8266_Check_AT_Response(const char *ret_ask);

void ESP8266_Rst(void);

//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 ESP8266_Consta_Check(void);

//ѡ��ESP8266�Ĺ���ģʽ
// enumMode ģʽ����
//�ɹ�����true��ʧ�ܷ���false
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode,char *ret_ask);

//ESP8266�����ⲿ��WIFI
//pSSID WiFi�ʺ�
//pPassWord WiFi����
//���óɹ�����true ��֮false
bool ESP8266_AT_JoinAP(const char * pSSID, const char * pPassWord, char *res_ask);

//ESP8266 
//enumEnUnvarnishTx  �Ƿ�����ӣ�bool����
//���óɹ�����true����֮false
bool ESP8266_Enable_MultipleId ( EventStatus enumEnUnvarnishTx, char *res_ask);

//ESP8266 ���ӷ�����
//enumE  ��������
//ip ��������IP
//ComNum  �������˿�
//id�����Ӻţ�ȷ��ͨ�Ų���������
//���óɹ�����true����֮fasle
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id ,char* res_ask);

bool ESP8266_SendString(EventStatus enumEnUnvarnishTx,const char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId);
bool ESP8266_UnvarnishSend ( void );
void ESP8266_ExitUnvarnishSend ( void );
u8 ESP8266_Get_LinkStatus ( void );
void USART_printf( uint32_t USARTx, char * Data, ... );

//MQTT���ܺ���
bool ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord);
bool ESP8266_MQTTCONN( char * Ip, int Num);
bool ESP8266_MQTTSUB(char * Topic);
bool ESP8266_MQTTPUB( char * Topic,char *temp);
bool ESP8266_MQTTCLEAN(void);
bool MQTT_SendString(char * pTopic,char *temp2);

#endif
