#include "esp8266.h"
#include "usart.h"

void ESP8266_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	ESP8266_RST_Pin_Periph_Clock();
	
	//GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13   ���ù������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //���ù������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	ESP8266_Rst();
}

u8* ESP8266_Check_Cmd(u8 *str)
{
		char *strx=0;
		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{ 
			USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
			strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
		} 
		return (u8*)strx;
}

//��ESP8266ģ�鷢��ATָ��
// cmd �����͵�ָ��
// ack1,ack2;�ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
// time �ȴ���Ӧʱ��
//����1���ͳɹ��� 0ʧ��
bool ESP8266_Send_AT_Cmd(const u8 *cmd, u8 *ack, u32 time)
{ 
		bool res=true;
		USART2_RX_STA=0;
		u2_printf("%s\r\n",cmd);	//��������
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

//���ͻָ�����Ĭ������ָ�ģ��ָ��ɳ�������
void ESP8266_AT_Restore(char *ret_ask)
{
    u2_printf("AT+RESTORE\r\n");
		
		strcpy(ret_ask, "OK");
}

//ȡ������
void ESP8266_AT_ATEO(char *ret_ask)
{
    u2_printf("ATE0\r\n");
		
		strcpy(ret_ask, "OK");
}
//  ���ò�����
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

//��λ����
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

//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
bool ESP8266_AT_Quit_Trans(void)
{
	while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);	//�ȴ����Ϳ�
	USART_DATA(USART5)='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);	//�ȴ����Ϳ�
	USART_DATA(USART5)='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while(usart_flag_get(USART5, USART_FLAG_TC) == RESET);	//�ȴ����Ϳ�
	USART_DATA(USART5)='+';      
	delay_ms(500);					//�ȴ�500ms
	return ESP8266_Send_AT_Cmd("AT","OK",20);//�˳�͸���ж�.
}

//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 ESP8266_Send_Data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(ESP8266_Check_Cmd(ack))break;//�õ���Ч���� 
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
    u2_printf("%s\r\n",cStr);	//����ATָ��
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

//͸��ʹ��
//���óɹ�����true�� ��֮false
bool ESP8266_UnvarnishSend ( void )
{
    return ESP8266_Send_AT_Cmd( "AT+CIPMODE=1", "OK", 200 );
}

//ESP8266�����ַ���
//enumEnUnvarnishTx�Ƿ�ʹ��͸��ģʽ
//pStr�ַ���
//ulStrLength�ַ�������
//ucId ���Ӻ�
//���óɹ�����true�� ��֮false
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


//ESP8266�˳�͸��ģʽ
void ESP8266_ExitUnvarnishSend ( void )
{
    delay_ms(1000);
    ESP8266_USART( "+++" );
    delay_ms( 500 );    
}


//ESP8266 �������״̬
//����0����ȡ״̬ʧ��
//����2�����ip
//����3���������� 
//����4��ʧȥ���� 
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

    while ( * Data != 0 )     // �ж������Ƿ񵽴������
    {                                         
        if ( * Data == 0x5c )  //'\'
        {                                     
            switch ( *++Data )
            {
                case 'r':                                     //�س���
								TempData=0x0d;
								usart_data_transmit(USARTx, TempData);
								while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//��������ж�
                Data ++;
                break;

                case 'n':                                     //���з�
								TempData=0x0a;
								usart_data_transmit(USARTx, TempData);
								while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//��������ж�						
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
                case 's':                                         //�ַ���
                s = va_arg(ap, const char *);
                for ( ; *s; s++) 
                {
										TempData=*s;
										usart_data_transmit(USARTx, TempData);
									  while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//��������ж�
                }
                Data++;
                break;

                case 'd':           
                    //ʮ����
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++) 
                {
                   	TempData=*s;
									  usart_data_transmit(USARTx, TempData);
									  while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//��������ж�
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
										while (RESET == usart_flag_get(USARTx, USART_FLAG_TC));//��������ж�					
				}

    }
}

//����ΪESP8266MQTT����ָ��

/*
*MQTT�����û�����
*LinkID ����ID,Ŀǰֻ֧��0
*scheme ���ӷ�ʽ������ѡ��MQTT over TCP,��������Ϊ1
*client_id MQTTclientID ���ڱ�־client���
*username ���ڵ�¼ MQTT ������ �� username
*password ���ڵ�¼ MQTT ������ �� password
*cert_key_ID ֤�� ID, Ŀǰ֧��һ�� cert ֤��, ����Ϊ 0
*CA_ID Ŀǰ֧��һ�� CA ֤��, ����Ϊ 0
*path ��Դ·������������Ϊ""
*���óɹ�����true ��֮false
*/
bool ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"", pClient_Id,pUserName,PassWord );
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 500 );
}


/*
*����ָ����MQTT������
*LinkID ����ID,Ŀǰֻ֧��0
*IP��MQTT�������϶�Ӧ��IP��ַ
*ComNum MQTT�������϶�Ӧ�Ķ˿ںţ�һ��Ϊ1883
*���óɹ�����true ��֮false
*/
bool ESP8266_MQTTCONN( char * Ip, int  Num)
{
    char cCmd [120];
    sprintf ( cCmd,"AT+MQTTCONN=0,\"%s\",%d,0", Ip,Num);
    return ESP8266_Send_AT_Cmd( cCmd, "OK",  500 );
}

/*
*����ָ�����ӵ� MQTT ����, ���ظ���ζ��Ĳ�ͬ topic
*LinkID ����ID,Ŀǰֻ֧��0
*Topic ���ĵ��������֣���������ΪTopic
*Qosֵ��һ��Ϊ0����������Ϊ1
*���óɹ�����true ��֮false
*/
bool ESP8266_MQTTSUB(char * Topic)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTSUB=0,\"%s\",1",Topic );
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 500 );
}


/*
*��LinkID��ͨ�� topic �������� data, ���� data Ϊ�ַ�����Ϣ
*LinkID ����ID,Ŀǰֻ֧��0
*Topic ���ĵ��������֣���������ΪTopic
*data���ַ�����Ϣ
*���óɹ�����true ��֮false
*/
bool ESP8266_MQTTPUB( char * Topic,char *temp)
{
    char cCmd [120];
    sprintf (cCmd, "AT+MQTTPUB=0,\"%s\",\"%s\",1,0", Topic ,temp);
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 1000 );
}

/*
*�ر� MQTT Client Ϊ LinkID ������, ���ͷ��ڲ�ռ�õ���Դ
*LinkID ����ID,Ŀǰֻ֧��0
*Topic ���ĵ��������֣���������ΪTopic
*data���ַ�����Ϣ
*���óɹ�����true ��֮false
*/
bool ESP8266_MQTTCLEAN(void)
{
    char cCmd [120];
    sprintf ( cCmd, "AT+MQTTCLEAN=0");
    return ESP8266_Send_AT_Cmd( cCmd, "OK", 500 );
}

//ESP8266�����ַ���
//enumEnUnvarnishTx�Ƿ�ʹ��͸��ģʽ
//pStr�ַ���
//ulStrLength�ַ�������
//ucId ���Ӻ�
//���óɹ�����true�� ��֮false
bool MQTT_SendString(char * pTopic,char *temp2)
{
	
    bool bRet = false;
    ESP8266_MQTTPUB(pTopic,temp2);
	  delay_ms(1000);
    bRet = true;
    return bRet;

}

