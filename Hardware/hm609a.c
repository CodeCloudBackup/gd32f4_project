#include "hm609a.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"
#include "MQTTPacket.h"
#include <stdio.h>
#include <string.h>

void HM609A_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOF,GPIO_Pin_11);	// Pwrkey,High, HM609A,Low time >550ms on/off
	delay_ms(1000);
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	GPIO_ResetBits(GPIOF,GPIO_Pin_12); // RESET,High, HM609A,Low time >300ms reset
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp_IN,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // W_DISABLE Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //AP_READY high:mcu sleep state
	 //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 
	printf("HM609A Init successed.\n");
}

char res_at[20];

u16  g_hm609aState = 0;      // ????,???????
u32  g_hm609aTim = 0;        // ?????
u16  g_hm609aReturnTim = 0;	// ?????
u8   g_hm609aSignal = 0;     // GPRS????
u16  g_hm609aHeartBeat = 0;  // ????
Byte8 g_hm609aFlag;
/*
????1ms????
*/
void HM609A_TIM_1ms(void)
{
    if(g_hm609aTim)g_hm609aTim--;
    if(hm609a_send_return)
    {
        if(g_hm609aReturnTim)g_hm609aReturnTim--;
    }
}

/*
*����ģ�鴦����
*
*/
void HM609A_Restart(void)
{
    g_hm609aState = 0;       //״̬��־���¿�ʼ
    DG_RESET=0;             //�ر�ģ���Դ
    g_hm609aTim = 0;         //ģ�鵹��ʱ����
    hm609a_connect_flag = 0;//�������ӳɹ���־
    hm609a_reg_flag = 0;    //IPע��ɹ���־
		delay_ms(300);
}

/*
*ģ�������������
*������Ϻ󷵻�1,���򷵻�0
*
*/
u8 HM609A_Restart_Program(void)//ģ����������
{
    static uint8_t Signs = 0;  //��������
    if(g_hm609aTim <= 0) //����ʱ������������̴���,���򷵻�0δ���
    {
        switch (Signs)//AIR208״̬����
        {
        case 0: //�ϵ翪ʼ
        {
            DG_RESET_OFF;         //�ر�ģ���Դ
            g_hm609aTim = 300;  //�ϵ�ʱ��ms
            Signs++;            //������һ������
        }
        break;
        case 1: //�ϵ�
        {
            DG_RESET_ON;          //��ģ���Դ
            g_hm609aTim = 5000;  //�ϵ��ʼ��ʱ��ms
            Signs++;            //������һ������
        }
        break;
        default://�ϵ��ʼ�����
        {
            Signs = 0;          //�ϵ��ʼ�����,�ָ�����
            return 1;           //�������,����1
        }
        }
    }
    return 0;
}

/*
*ģ������
*�ɹ�����1,ִ���з���0,ʧ�ܷ��ش������>=20
*/
u8 HM609A_config(void)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
	u16 len = 0;
	char buf[200]={0};
	if(g_hm609aTim == 0) //Ϊ0ʱ���Ͳ�������
	{
			if(count > 0 && count >= cnt) //��������ظ�����
			{
				count = 0;      //��������
				cnt = 1;        //��������λ
				len = Signs + 20; //����������
				Signs = 0;      //��������
				return len;     //���ش�����
			}
		  else
			{
				count++;
				switch (Signs)//AIR208״̬����
        {
					case 0: //ȡ�����İ���Թ���
          {
						printf("A|ATE1\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("ATE1\r\n");  //����ATָ��
					}
					break;
					case 1: // ��ѯ����ע����������İ���Զ�ע�����磬�ϵ絽ע���� 10s ����
          {
							printf("A|ATI\r\n");
							g_hm609aTim = 2000;				//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nATI\r\n"); //����ATָ��
          }
					break;
					case 2: // ��ѯ����ע����������İ���Զ�ע�����磬�ϵ絽ע���� 10s ����
          {
							printf("A|AT+CGMR\r\n");
							g_hm609aTim = 2000;				//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CGMR\r\n"); //����ATָ��
          }
					break;
					case 3: // ��ѯ��������Ϣ
					{
							printf("A|AT+CFUN?\r\n");
							g_hm609aTim = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CFUN?\r\n"); //����ATָ��
					}
					break;
					case 4: // ��ѯ��������Ϣ
					{
							printf("A|AT+CPIN?\r\n");
							g_hm609aTim = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CPIN?\r\n"); //����ATָ��
					}
					break;
					case 5:  // ��ѯϵͳ��Ϣ��SIM״̬
          {
						 printf("A|AT^SYSINFO\r\n");
             g_hm609aTim = 2000;				//��ʱʱ��ms
             cnt = 30;   //�ظ�������,*air208_Tim��ʱ����ʱ��
             strcpy(res_at, "^SYSINFO");	//���÷����жϹؼ���
             u1_printf("\r\nAT^SYSINFO\r\n");  //����ATָ��
					}
					break;
					case 6: // ��ѯSNֵ
					{
							printf("A|AT+CGSN\r\n");
							g_hm609aTim = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CGSN\r\n"); //����ATָ��
					}
					break;
					default:// ״̬����ִ�����
					{
						count = 0;      //���Դ�������
						Signs = 0;      //��������
						cnt = 1;        //��������λ
						return 1;       //�����������
					}
				}
			}
	}
	else
	{
		if(USART1_Revice((u8*)buf))         //�Ӵ���3��ȡ����
		{
				printf("Recv:%s",buf);
				if(strstr((const char *)buf, (const char *)res_at) != NULL) //����Ƿ�����ؼ���
				{
					g_hm609aTim = 0; //��ʱ����
					count = 0;      //���Դ�������
					Signs++;        //��һ������
				}
		}
	}
	return 0;
	
}

char getCommond[100]= "GET /iob/download/test.txt HTTP/1.1\r\nHost:101.37.89.157\r\n\r\n";
u8 HM609A_Tcp(void)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
	u16 len = 0;
	char buf[200];
	if(g_hm609aTim == 0) //Ϊ0ʱ���Ͳ�������
	{
		if(count > 0 && count >= cnt) //��������ظ�����
			{
				count = 0;      //��������
				cnt = 1;        //��������λ
				len = Signs + 20; //����������
				Signs = 0;      //��������
				return len;     //���ش�����
			}
		  else
			{
				count++;
				switch (Signs)//HTTP���ò�ѯ
        {
					case 0: //
          {
						printf("A|AT+IPOPEN?\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+IPOPEN?\r\n");  //����ATָ��
					}
					break;
					case 1: //
          {
						printf("A|AT+IPOPEN=\"TCP\"\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						
						u1_printf("\r\nAT+IPOPEN=1,\"TCP\",\"101.37.89.157\",80,0,0\r\n");  //����ATָ��
					}
					break;
					case 2: //
          {
						printf("A|AT+IPSWTMD=1,1\r\n");
						g_hm609aTim = 5000;          //��ʱʱ��ms
						cnt = 6;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						
						u1_printf("\r\nAT+IPSWTMD=1,1\r\n");  //����ATָ��
					}
					break;
					case 3: //ȡ�����İ���Թ���
          {
						u16 len = sizeof(getCommond);
						getCommond[len] = '\0';
						printf("\r\nA|AT+IPSEND=1\r\n");
						g_hm609aTim = 5000;          //��ʱʱ��ms
						cnt = 10;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"CONNECT"); 
						u1_printf("\r\nAT+IPSEND=1,%d\r\n",strlen(getCommond)+1);  //����ATָ��
						
					}
					break;
					
					case 4: //
          {
						printf("\r\nA|send data\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1; 
						strcpy(res_at,"Content-Length"); 
						USART1_SendData((u8*)getCommond,strlen(getCommond)+1);  //����ATָ��
					}
					break;
					case 5:
					{
						
						printf("\r\nA|AT+IPRD\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 10;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+IPRD=1,20\r\n");  //����ATָ��
					}
					break;
					case 6: //
          {
						printf("A|AT+IPCLOSE=1\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						
						u1_printf("AT+IPCLOSE=1\r\n");  //����ATָ��
					}
					break;
					default:// ״̬����ִ�����
					{
						count = 0;      //���Դ�������
						Signs = 0;      //��������
						cnt = 1;        //��������λ
						return 1;       //�����������
					}
				}
			}
	}
	else
	{
		if(USART1_Revice((u8*)buf))         //�Ӵ���3��ȡ����
		{
				printf("Recv:%s",buf);
				if(strstr((const char *)buf, (const char *)res_at) != NULL) //����Ƿ�����ؼ���
				{
					g_hm609aTim = 0; //��ʱ����
					count = 0;      //���Դ�������
					Signs++;        //��һ������
				}
		}
	}
	return 0;
}	


u8 HM609A_Http(void)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
	u16 len = 0;
	char buf[200];
	if(g_hm609aTim == 0) //Ϊ0ʱ���Ͳ�������
	{
		if(count > 0 && count >= cnt) //��������ظ�����
			{
				count = 0;      //��������
				cnt = 1;        //��������λ
				len = Signs + 20; //����������
				Signs = 0;      //��������
				return len;     //���ش�����
			}
		  else
			{
				count++;
				switch (Signs)//HTTP���ò�ѯ
        {
					case 0: //
          {
						printf("A|AT+HTTPCFG?\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=?\r\n");  //����ATָ��
					}
					break;
					case 1: //
          {
						char *url = "http://101.37.89.157/iob/download/test.txt";
						u16 len = strlen(url); 
						printf("A|AT+HTTPURL\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						
						u1_printf("AT+HTTPURL=%d,20\r\n",len);  //����ATָ��
						u1_printf("%s",url);
					}
					break;
					case 2: //ȡ�����İ���Թ���
          {
						printf("A|AT+HTTPGET=10\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"CONNECT"); 
						u1_printf("AT+HTTPGET=10\r\n");  //����ATָ��
					}
					break;
					default:// ״̬����ִ�����
					{
						count = 0;      //���Դ�������
						Signs = 0;      //��������
						cnt = 1;        //��������λ
						return 1;       //�����������
					}
				}
			}
	}
	else
	{
		if(USART1_Revice((u8*)buf))         //�Ӵ���3��ȡ����
		{
				printf("Recv:%s",buf);
				if(strstr((const char *)buf, (const char *)res_at) != NULL) //����Ƿ�����ؼ���
				{
					g_hm609aTim = 0; //��ʱ����
					count = 0;      //���Դ�������
					Signs++;        //��һ������
				}
		}
	}
	return 0;
}	

MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
MQTTString topicString = MQTTString_initializer;

void MQTT_Init(void)
{	
	data.clientID.cstring = "124";//�ͻ���ID
	data.keepAliveInterval = 60;//�������������ʱ��
	data.cleansession = 1;//����Ự
	data.username.cstring = "124";//�û���
	data.password.cstring = "124";//����
}
u8 p [200];
int buflen = 200;
char* payload = "iob/iob/job/x";
u8 revicebuf[200];

int transport_getdata(unsigned char* buf, int count)
{
	memcpy(revicebuf, buf,count);
	return count;
}



u8 Mqtt_Connack_Deserialize(u8* buf)
{
	unsigned char sessionPresent, connack_rc;

	if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
	{
		printf("Unable to connect, return code %d\n", connack_rc);
		return 1;
	}
		return 0;
}

u8 Mqtt_Suback_Deserialize(u8* buf)
{
	unsigned short submsgid;
		int subcount;
		int granted_qos;

		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			printf("granted qos != 0, %d\n", granted_qos);
			return 1;
		}
		return 0;
}

u8 Mqtt_Publish_Deserialize(u8* buf)
{
		unsigned char dup;
		int qos;
		unsigned char retained;
		unsigned short msgid;
		int payloadlen_in;
		unsigned char* payload_in;
		MQTTString receivedTopic;

		MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
				&payload_in, &payloadlen_in, buf, buflen);
		printf("message arrived %.*s\n", payloadlen_in, payload_in); //��Ϣ����
		return 0;	
}

u8 Mqtt_Deserialize_Handle(u8 msg_type, u8* buf)
{
	u8 ret=0;
	switch(msg_type)
	{
		case CONNACK:
			ret = Mqtt_Connack_Deserialize(buf);
		break;
		case SUBACK:
			ret = Mqtt_Suback_Deserialize(buf);
		break;
		case PUBLISH:
			ret = Mqtt_Publish_Deserialize(buf);
		break;
		default:// ״̬����ִ�����
		{
			printf("err message type %d",msg_type);
		}
	}
	return ret;
}


static u8 flag = 0;
u8 HM609A_Mqtt_Program(char* addr, int port)
{
	static uint8_t count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
	static int len = 0;
	u16 errCode = 0;
	static u8 msg_type = 0;
	int payloadlen = strlen(payload);
	
	if(g_hm609aTim == 0) //Ϊ0ʱ���Ͳ�������
	{
		if(count > 0 && count >= cnt) //��������ظ�����
			{
				count = 0;      //��������
				cnt = 1;        //��������λ
				errCode = Signs + 20; //����������
				Signs = 0;      //��������
				return errCode;     //���ش�����
			}
		  else
			{
				count++;
				switch (Signs)//AIR208״̬����
        {
					case 0: //
          {
						flag=0;
						printf("\r\nAT+IPOPEN=2,\"TCP\",\"%s\",1883,0,0\r\n",addr);
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 3;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("\r\nAT+IPOPEN=2,\"TCP\",\"101.37.89.157\",1883,0,0\r\n");  //����ATָ��
					}
					break;
					case 1: //
          {
						flag=0;
						printf("A|AT+IPSWTMD=2,1\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 6;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 				
						u1_printf("\r\nAT+IPSWTMD=2,1\r\n");  //����ATָ��
					}
					break;
					case 2: //
          {
						flag=0;
						memset(p,0,buflen);
						MQTT_Init();
						msg_type = CONNACK;
						len = MQTTSerialize_connect(p, buflen, &data);
						printf("A|AT+IPSENDEX=2,\"101900044d51545404c2003c000331323400033132340003313234\"\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"+IPURC"); 
						
						u1_printf("\r\nAT+IPSENDEX=2,\"101900044d51545404c2003c000331323400033132340003313234\"\r\n");  //����ATָ��
					}
					break;
					case 3: //
					{
						g_hm609aTim = 20000; 
						cnt = 100;
						flag=1;
					}
					break;
//						int req_qos = 0;//QOS
//						int msgid = 1;
//						flag =0;
//						memset(p,0,buflen);
//						msg_type = CONNACK;
//						topicString.cstring = "iob/iob/job/x";
//						len=MQTTSerialize_subscribe(p, buflen, 0, msgid, 1, &topicString, &req_qos);
//						printf("A|AT+IPSEND=2,%d\r\n",len);
//						g_hm609aTim = 5000;          //��ʱʱ��ms
//						cnt = 6;   //�ظ�������,*air208_Tim��ʱ����ʱ��
//						strcpy(res_at,"OK"); 
//						u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);  //����ATָ��
//					}
//					break;
//					case 5: //��������
//          {
//						
//						msg_type = SUBACK;
//						printf("A|send data%s\r\n",p);
//						g_hm609aTim = 2000;          //��ʱʱ��ms
//						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
//						p[len]= '\0';
//						u1_printf("%s",p);
//					}
//					break;
//					case 6: //��������
//          {
//						flag =0;
//						len = MQTTSerialize_publish(p, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//						printf("A|AT+IPSEND=2,%d\r\n",len);
//						cnt = 1; 
//						g_hm609aTim = 2000;          //��ʱʱ��ms
//						strcpy(res_at,"OK"); 
//						u1_printf("\r\nAT+IPSEND=2,%d\r\n",len);  //����ATָ��
//					}
//					break;
//					case 7: //��������
//          {
//						flag =1;
//						msg_type = PUBLISH;
//						printf("A|send data%s\r\n",p);
//						g_hm609aTim = 2000;          //��ʱʱ��ms
//						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
//						p[len]= '\0';
//						USART1_SendData(p, len+1);
//					}
					break;
					default:// ״̬����ִ�����
					{
						count = 0;      //���Դ�������
						Signs = 0;      //��������
						cnt = 1;        //��������λ
						return 1;       //�����������
					}
				}
			}
	}
	else
	{
		if(USART1_Revice(revicebuf))         //�Ӵ���3��ȡ����
		{
				printf("Recv:%s",revicebuf);
				if(flag){
					unsigned char buf[200];
					int buflen = sizeof(buf);
					g_hm609aTim = 0; //��ʱ����
					count = 0;      //���Դ�������
				//	Signs++;        //��һ������
//					if(MQTTPacket_read(buf, buflen, transport_getdata)==msg_type){
//						Mqtt_Deserialize_Handle(msg_type, buf);
//					}
					
				}else{
					if(strstr((const char *)revicebuf, (const char *)res_at) != NULL) //����Ƿ�����ؼ���
					{
						g_hm609aTim = 0; //��ʱ����
						count = 0;      //���Դ�������
						Signs++;        //��һ������
					}
				}

		}
	}
	return 0;
}	
/*
��ѭ������ÿ��ѭ��ִ��һ��
*/
void HM609A_Program(char* addr, int port)
{
	u8 err = 0;    //���صô������
	static u8 count = 0;  //�ظ�����
	switch (g_hm609aState)//AIR208״̬����
  {
		case 0:
		{
				if(HM609A_Restart_Program())
        {
            g_hm609aState++;
            count=0;
        }
		}
		break;
		case 1:     //ģ�����AT
    {
        err = HM609A_config();
        switch(err)
        {
        case 1:
        {
            //����AT�ɹ�,������һ������
            g_hm609aState++;
            count=0;
        }
        break;
        case 2:
        {
            HM609A_Restart();
        }
        break;
        default:
            break;
        }
    }
    break;
		case 2:
		{
			err = HM609A_Tcp();
			switch(err)
      {
         case 0:break; 		//��������,ֱ������
         case 1:
         {
						//��ʼ��,������һ������
						count=0;
						g_hm609aState++;
          }
          break;
          default:
          {
                //��ʼ��ʧ�ܣ�����ģ��
             HM609A_Restart();
          }
          break;
      }
		}
		break;
		case 3:
		{
			char heart[2]= {0xC0, 0x00};
			err = HM609A_Mqtt_Program(addr, port);
			switch(err)
      {
         case 0:
							if(g_hm609aHeartBeat == 55 &&flag)
							{
								printf("\r\nAT+IPSENDEX=2,\"C000\"\r\n");
								u1_printf("\r\nAT+IPSENDEX=2,\"C000\"\r\n");
								g_hm609aHeartBeat=0;
							}
							hm609a_connect_flag = 1;
				 case 1:break; 		//��������,ֱ������
         case 2:
         {
						//��ʼ��,������һ������
						count=0;
						g_hm609aState++;
          }
          break;
          default:
          {
                //��ʼ��ʧ�ܣ�����ģ��
             HM609A_Restart();
          }
          break;
      }
		}
		break;
		default://TCP���ӳɹ�����ʼ�����շ�
    {
        if(hm609a_send_return&&g_hm609aReturnTim<=0)
        {
             //���ͳ�ʱ�����½�������
            hm609a_send_return=0;
            hm609a_connect_flag=0;
            hm609a_reg_flag=0;
        }
        //���ӳɹ���������ӶϿ�����������
        if(!hm609a_connect_flag)g_hm609aState=3;//������ӶϿ���ִ�жϿ�TCP�����´���TCP����
    }break;
	}
}

void HM609A_test(void)
{
	
}
