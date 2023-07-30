#include "esp8266.h"
#include "usart.h"

void ESP8266_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	ESP8266_RST_Pin_Periph_Clock();
	
	//GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13   复用功能输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //复用功能输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	ESP8266_Rst();
}

u8* ESP8266_Check_Cmd(u8 *str)
{
		char *strx=0;
		if(USART2_RX_STA&0X8000)		//接收到一次数据了
		{ 
			USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
			strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
		} 
		return (u8*)strx;
}

//对ESP8266模块发送AT指令
// cmd 待发送的指令
// ack1,ack2;期待的响应，为NULL表不需响应，两者为或逻辑关系
// time 等待响应时间
//返回1发送成功， 0失败
bool ESP8266_Send_AT_Cmd(const u8 *cmd, u8 *ack, u32 time)
{ 
		bool res=true;
		USART2_RX_STA=0;
		u2_printf("%s\r\n",cmd);	//发送命令
		if(ack&& time) {
				while(--time)
				{
					delay_ms(10);
					if(USART2_RX_STA&0x8000)
					{
						if(ESP8266_Check_Cmd(ack))
						{
							printf("ack:%s\r\n",(u8*)ack);
							break;
						}
						USART2_RX_STA=0;
					}
				}
				if(time==0)res=false; 
		}
		return res;
}

//发送恢复出厂默认设置指令将模块恢复成出厂设置
void ESP8266_AT_Restore(char *ret_ask)
{
    u2_printf("AT+RESTORE\r\n");
		
		strcpy(ret_ask, "OK");
}

//取消回显
void ESP8266_AT_ATEO(char *ret_ask)
{
    u2_printf("ATE0\r\n");
		
		strcpy(ret_ask, "OK");
}
//  设置波特率
void ESP8266_AT_BaudRate(char *ret_ask)
{
		u2_printf("AT+UART_DEF=115200,8,1,0,0\r\n");
		strcpy(ret_ask, "OK");
}


bool ESP8266_Check_AT_Response(const char *ret_ask)
{
	char buf[100];
	if(USART2_Revice(COMMAND, buf)){			
			if(strstr((const char*)buf, ret_ask) != NULL)
			{
					return true;
			}
			return false;
	}
	return false;
}

//复位重启
void ESP8266_Rst(void)
{
    ESP8266_RST=0;
    delay_ms(500); 
    ESP8266_RST=1;
}

bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode, char *ret_ask)
{
    switch ( enumMode )
    {
        case STA:
						u2_printf("AT+CWMODE=1\r\n");
						break;
        case AP:
						u2_printf("AT+CWMODE=2\r\n"); 
						break;
        case STA_AP:
            u2_printf("AT+CWMODE=3\r\n");
						break;	
        default:
          return false;
    }  
		strcpy(ret_ask, "OK"); 
		return true;
}

//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
bool ESP8266_AT_Quit_Trans(void)
{
	while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);	//等待发送空
	USART_DATA(USART5)='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);	//等待发送空
	USART_DATA(USART5)='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);	//等待发送空
	USART_DATA(USART5)='+';      
	delay_ms(500);					//等待500ms
	return ESP8266_Send_AT_Cmd("AT","OK",20);//退出透传判断.
}

//向ATK-ESP8266发送指定数据
//data:发送的数据(不需要添加回车了)
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)luojian
u8 ESP8266_Send_Data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(ESP8266_Check_Cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}



bool ESP8266_AT_JoinAP(const char * pSSID, const char * pPassWord, char *res_ask)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", pSSID, pPassWord );
    u2_printf(cCmd);
		strcpy(res_ask, "WIFI GOT IP");
	  return true;
}

bool ESP8266_Enable_MultipleId (EventStatus enumEnUnvarnishTx,char *res_ask)
{
    char cStr [20];
    sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
    u2_printf("%s\r\n",cStr);	//发送AT指令
		strcpy(res_ask, "OK");
		return true;
}


bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id,char* res_ask)
{
    char cStr [100] = { 0 }, cCmd [120];

    switch (  enumE )
    {
        case enumTCP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
          break;

        case enumUDP:
          sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
          break;

        default:
            break;
    }
		
    if ( id < 5 )
        sprintf ( cCmd, "AT+CIPSTART=%d,%s\r\n", id, cStr);

    else
        sprintf ( cCmd, "AT+CIPSTART=%s\r\n", cStr );
		u2_printf(cCmd);
    strcpy(res_ask, "OK"); 
		return true;
}

bool ESP8266_Close_Link_Server(void)
{
		 if (!ESP8266_Send_AT_Cmd ( "AT+CIPCLOSE", "OK", 200 ))
		 return true;
		 return false;
}

//透传使能
//设置成功返回true， 反之false
bool ESP8266_UnvarnishSend ( void )
{
    return ESP8266_Send_AT_Cmd( "AT+CIPMODE=1", "OK", 200 );
}

//ESP8266发送字符串
//enumEnUnvarnishTx是否使能透传模式
//pStr字符串
//ulStrLength字符串长度
//ucId 连接号
//设置成功返回true， 反之false
bool ESP8266_SendString(EventStatus enumEnUnvarnishTx, const char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
    char cStr [20];
    bool bRet = false;
    if ( enumEnUnvarnishTx )
    {
        u2_printf ( "%s", pStr );

        bRet = true;
    }

    else
    {
        if ( ucId < 5 )
            sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

        else
            sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );

        ESP8266_Send_AT_Cmd ( cStr, "> ",  100 );

        bRet = ESP8266_Send_AT_Cmd ( pStr, "SEND OK", 10 );
  }

    return bRet;

}


//ESP8266退出透传模式
void ESP8266_ExitUnvarnishSend ( void )
{
    delay_ms(1000);
    ESP8266_USART( "+++" );
    delay_ms( 500 );    
}


//ESP8266 检测连接状态
//返回0：获取状态失败
//返回2：获得ip
//返回3：建立连接 
//返回4：失去连接 
u8 ESP8266_Get_LinkStatus ( void )
{
//    if (ESP8266_Send_AT_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) )
//    {
//        if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:2\r\n" ) )
//            return 2;

//        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:3\r\n" ) )
//            return 3;

//        else if ( strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "STATUS:4\r\n" ) )
//            return 4;       

//    }

    return 0;
}

static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


void USART_printf ( u32  USARTx, char * Data, ... )
{
    const char *s;
    int d;   
    char buf[16];
	unsigned char TempData;


    va_list ap;
    va_start(ap, Data);

    while ( * Data != 0 )     // 判断数据是否到达结束符
    {                                         
        if ( * Data == 0x5c )  //'\'
        {                                     
            switch ( *++Data )
            {
                case 'r':                                     //回车符
								TempData=0x0d;
								usart_data_transmit(USARTx, TempData);
								while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//发送完成判断
                Data ++;
                break;

                case 'n':                                     //换行符
								TempData=0x0a;
								usart_data_transmit(USARTx, TempData);
								while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//发送完成判断						
                Data ++;
                break;

                default:
                Data ++;
                break;
            }            
        }

        else if ( * Data == '%')
        {                                     
            switch ( *++Data )
            {               
                case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++) 
                {
										TempData=*s;
										usart_data_transmit(USARTx, TempData);
									  while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//发送完成判断
                }
                Data++;
                break;

                case 'd':           
                    //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++) 
                {
                   	TempData=*s;
									  usart_data_transmit(USARTx, TempData);
									  while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//发送完成判断
                }
                     Data++;
                     break;
                default:
                     Data++;
                     break;
            }        
        }
        else 
				{
										TempData=*Data++;
								    usart_data_transmit(USARTx, TempData);
										while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//发送完成判断					
				}

    }
}

//下面为ESP8266MQTT功能指令

/*
*MQTT配置用户属性
*LinkID 连接ID,目前只支持0
*scheme 连接方式，这里选择MQTT over TCP,这里设置为1
*client_id MQTTclientID 用于标志client身份
*username 用于登录 MQTT 服务器 的 username
*password 用于登录 MQTT 服务器 的 password
*cert_key_ID 证书 ID, 目前支持一套 cert 证书, 参数为 0
*CA_ID 目前支持一套 CA 证书, 参数为 0
*path 资源路径，这里设置为""
*设置成功返回true 反之false
*/
bool ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"", pClient_Id,pUserName,PassWord );
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 500 );
}


/*
*连接指定的MQTT服务器
*LinkID 连接ID,目前只支持0
*IP：MQTT服务器上对应的IP地址
*ComNum MQTT服务器上对应的端口号，一般为1883
*设置成功返回true 反之false
*/
bool ESP8266_MQTTCONN( char * Ip, int  Num)
{
    char cCmd [120];
    sprintf ( cCmd,"AT+MQTTCONN=0,\"%s\",%d,0", Ip,Num);
    return ESP8266_Send_AT_Cmd( cCmd, "OK",  500 );
}

/*
*订阅指定连接的 MQTT 主题, 可重复多次订阅不同 topic
*LinkID 连接ID,目前只支持0
*Topic 订阅的主题名字，这里设置为Topic
*Qos值：一般为0，这里设置为1
*设置成功返回true 反之false
*/
bool ESP8266_MQTTSUB(char * Topic)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTSUB=0,\"%s\",1",Topic );
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 500 );
}


/*
*在LinkID上通过 topic 发布数据 data, 其中 data 为字符串消息
*LinkID 连接ID,目前只支持0
*Topic 订阅的主题名字，这里设置为Topic
*data：字符串信息
*设置成功返回true 反之false
*/
bool ESP8266_MQTTPUB( char * Topic,char *temp)
{
    char cCmd [120];
    sprintf (cCmd, "AT+MQTTPUB=0,\"%s\",\"%s\",1,0", Topic ,temp);
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 1000 );
}

/*
*关闭 MQTT Client 为 LinkID 的连接, 并释放内部占用的资源
*LinkID 连接ID,目前只支持0
*Topic 订阅的主题名字，这里设置为Topic
*data：字符串信息
*设置成功返回true 反之false
*/
bool ESP8266_MQTTCLEAN(void)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTCLEAN=0");
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 500 );
}

//ESP8266发送字符串
//enumEnUnvarnishTx是否使能透传模式
//pStr字符串
//ulStrLength字符串长度
//ucId 连接号
//设置成功返回true， 反之false
bool MQTT_SendString(char * pTopic,char *temp2)
{
	
    bool bRet = false;
    ESP8266_MQTTPUB(pTopic,temp2);
	  delay_ms(1000);
    bRet = true;
    return bRet;

}

