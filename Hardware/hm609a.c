#include "hm609a.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"
#include "http_app.h"
#include <stdio.h>
#include <string.h>

void HM609A_Init(void)
{
	u8 status=0;
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	GPIO_ResetBits(GPIOF,GPIO_Pin_12); // RESET,High, HM609A,Low time >300ms reset
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp_IN,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // W_DISABLE Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //AP_READY high:mcu sleep state
	 //�ȳ�ʼ��ADC1ͨ��5 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PA5 ͨ��5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 
	
	status = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3);
	if(!status){
		DG_PWRKEY_ON;	// Pwrkey,High, HM609A,Low time >550ms on/off
		delay_ms(1000);
		DG_PWRKEY_OFF;
	}
	printf("\r\nHM609A Init successed.status:%d\n",status);
}


u32  g_hm609aTim[7] = {0};        // ?????
u16  g_hm609aMqttWaitTim=0;	// ?????
u16  g_hm609aHttpWaitTim=0;
u16 g_hm609aHttp=0;
Byte8 g_hm609aFlag;
/*
????1ms????
*/
void HM609A_TIM_1ms(void)
{
		if(g_hm609aTim[0])g_hm609aTim[0]--;
    if(g_hm609aTim[1])g_hm609aTim[1]--;
		if(g_hm609aTim[2])g_hm609aTim[2]--;
		if(g_hm609aTim[3])g_hm609aTim[3]--;
    if(hm609a_mqtt_wait_flag)
    {
      if(g_hm609aMqttWaitTim)g_hm609aMqttWaitTim--;
    }

    if(g_hm609aHttpWaitTim)g_hm609aHttpWaitTim--;
		
}

/*
*����ģ�鴦����
*
*/
static void HM609A_Restart(void)
{
    DG_RESET=0;             		// ģ�鸴λ
		memset(g_hm609aTim,0,sizeof(g_hm609aTim));   // ģ�鵹��ʱ����
    hm609a_config_flag=0;				// ģ�����ñ�־����
		hm609a_mqtt_conn_flag=0;		// �������ӳɹ���־����
    hm609a_mqtt_reg_flag=0;   	// mqttע��ɹ���־
		hm609a_mqtt_wait_flag=0;
		hm609a_http_conn_flag=0;		//http���ӳɹ���־
		hm609a_http_wait_flag=0;
		delay_ms(300);
}

/*
*ģ�������������
*������Ϻ󷵻�1,���򷵻�0
*
*/
static u8 HM609A_Restart_Program(void)//ģ����������
{
    static u8 sign = 0;  //��������
    if(g_hm609aTim[0] <= 0) //����ʱ������������̴���,���򷵻�0δ���
    {
        switch (sign)//AIR208״̬����
        {
        case 0: //�ϵ翪ʼ
        {
            DG_RESET_OFF;         //�ر�ģ���Դ
            g_hm609aTim[0] = 300;  //�ϵ�ʱ��ms
            sign++;            //������һ������
        }
        break;
        case 1: //�ϵ�
        {
            DG_RESET_ON;          //��ģ���Դ
            g_hm609aTim[0] = 5000;  //�ϵ��ʼ��ʱ��ms
            sign++;            //������һ������
        }
        break;
        default://�ϵ��ʼ�����
        {
            sign = 0;          //�ϵ��ʼ�����,�ָ�����
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
static u8 HM609A_Config(void)
{
	static char res_at[20];
	static u8 count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
	char buf[100];
	u16 len = 0;
	if(g_hm609aTim[0] == 0) //Ϊ0ʱ���Ͳ�������
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
						g_hm609aTim[0] = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("ATE1\r\n");  //����ATָ��
					}
					break;
					case 1: // ��ѯ����ע����������İ���Զ�ע�����磬�ϵ絽ע���� 10s ����
          {
							printf("A|ATI\r\n");
							g_hm609aTim[0] = 2000;				//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nATI\r\n"); //����ATָ��
          }
					break;
					case 2: // ��ѯ����ע����������İ���Զ�ע�����磬�ϵ絽ע���� 10s ����
          {
							printf("A|AT+CGMR\r\n");
							g_hm609aTim[0] = 2000;				//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CGMR\r\n"); //����ATָ��
          }
					break;
					case 3: // ��ѯ��������Ϣ
					{
							printf("A|AT+CFUN?\r\n");
							g_hm609aTim[0] = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CFUN?\r\n"); //����ATָ��
					}
					break;
					case 4: // ��ѯ��������Ϣ
					{
							printf("A|AT+CPIN?\r\n");
							g_hm609aTim[0] = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CPIN?\r\n"); //����ATָ��
					}
					break;
					case 5:  // ��ѯϵͳ��Ϣ��SIM״̬
          {
						 printf("A|AT^SYSINFO\r\n");
             g_hm609aTim[0] = 2000;				//��ʱʱ��ms
             cnt = 30;   //�ظ�������,*air208_Tim��ʱ����ʱ��
             strcpy(res_at, "^SYSINFO");	//���÷����жϹؼ���
             u1_printf("\r\nAT^SYSINFO\r\n");  //����ATָ��
					}
					break;
					case 6: // ��ѯSNֵ
					{
							printf("A|AT+CGSN\r\n");
							g_hm609aTim[0] = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CGSN\r\n"); //����ATָ��
					}
					break;
					case 7: // ��ѯIPֵ
					{
							printf("A|AT+IPDNSR=\"debug.armlogic.tech\"\r\n");
							g_hm609aTim[0] = 5000;          	//��ʱʱ��ms
							cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+IPDNSR=\"debug.armlogic.tech\"\r\n"); //����ATָ��
					}
					break;
					case 8:
					{
						printf("\r\nA|ATE0\r\n");
						cnt = 3;
						g_hm609aTim[0] = 2000;
						strcpy(res_at, "OK");		//���÷����жϹؼ���
						u1_printf("\r\nATE0\r\n");  //����ATָ��
					}
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
		if(USART1_Revice((u8*)buf, 100))         //�Ӵ���3��ȡ����
		{
			if(strstr(buf, (const char *)res_at) != NULL) //����Ƿ�����ؼ���
			{
					g_hm609aTim[0] = 0; //��ʱ����
					count = 0;      //���Դ�������
					Signs++;        //��һ������
			}
		}
	}
	return 0;
	
}

/*
*ģ��TCP����
*�ɹ�����1,ִ���з���0,ʧ�ܷ��ش������>=20
*/
static u8 HM609A_Connect(u8 sockid, char* addr, int port, NET_PROT protocol)
{
	static char res_at[10];
	static u8 count[7] = {0}, Signs[7] = {0}, cnt[7] = {0};
	char buf[100];
	if(g_hm609aTim[sockid] == 0)
	{
		if(count[sockid] > 0 && count[sockid] >= cnt[sockid]) //��������ظ�����
		{
				count[sockid] = 0;      //��������
				cnt[sockid] = 1;        //��������λ
				Signs[sockid] = 0;      //��������
				return Signs[sockid] + 30;     //���ش�����
		} else {
			count[sockid]++;
			switch (Signs[sockid])//AIR208״̬����
      {
				case 0: //
        {
					printf("\r\nA|AT+IPOPEN=%d,\"TCP\",%s,%d,0,0\r\n",sockid,addr,port);
					cnt[sockid] = 3;
					g_hm609aTim[sockid] = 5000;
					strcpy(res_at,"OK"); 
					u1_printf("\r\nAT+IPOPEN=%d,\"TCP\",\"%s\",%d,0,0\r\n",sockid,addr,port);  //����ATָ��
				}
				break;
				case 1: //
        {
					cnt[sockid] = 3;
					g_hm609aTim[sockid] = 5000;
					
					if(protocol == MQTT_PROT)
					{
						printf("\r\nA|AT+IPSWTMD=%d,1\r\n",sockid);
						strcpy(res_at,"OK"); 
						u1_printf("\r\nAT+IPSWTMD=%d,1\r\n",sockid);  //����ATָ��
					} 
					else if (protocol == HTTP_PROT)
					{
						printf("\r\nA|AT+IPSWTMD=%d,2\r\n",sockid);
						strcpy(res_at,"CONNECT"); 
						u1_printf("\r\nAT+IPSWTMD=%d,2\r\n",sockid);  //����ATָ��
					}
					
				}
				break;
				default:// �ȴ����ӽ����ɹ�
				{
						count[sockid] = 0;      //���Դ�������
            Signs[sockid] = 0;      //��������
            cnt[sockid] = 1;        //��������λ
            return 1;       //�����������
				}
			}
		}
	}
	else
	{
		if(USART1_Revice( (u8*)buf, 100))        //�Ӵ���3��ȡ����
		{
			if(strstr(buf, res_at) != NULL) //����Ƿ�����ؼ���
			{
				g_hm609aTim[sockid] = 0; //��ʱ����
				count[sockid] = 0;      //���Դ�������
				Signs[sockid]++;        //��һ������
			}
		}
	}
	return 0;
}

/*
*�ر�TCP����
*
*/
static u8 HM609A_Tcp_Off(u8 sockid)// �ر�TCP����
{
    static u8 count[7] = {0}, state[7] = {0}, cnt[7] =  {1,1,1,1,1,1,1}; //�ظ�����,��������
		char buf[50];
    if(g_hm609aTim[sockid] == 0)
    {
       if(count[sockid] > 0 && count[sockid] >= cnt[sockid] ) //��������ظ�����
       {
           count[sockid] = 0;      //��������
					 cnt[sockid] = 1;
					 state[sockid]=0;
           return 41;     //���ش�����
       }
       else
			{
					count[sockid]++;
					switch(state[sockid]){
					case 0:
					{
							u1_printf("\r\n+++\r\n"); 
							printf("A|AT+IPCLOSE=%d",sockid);    // �ر�TCP����
							g_hm609aTim[sockid] = 1000;      //��ʱʱ��ms
							cnt[sockid]=3;
							u1_printf("\r\nAT+IPCLOSE=%d\r\n",sockid);     //����ATָ��
					}
					break;
					default:// �ȴ����ӽ����ɹ�
					{
							count[sockid] = 0;      //���Դ�������
							state[sockid] = 0;      //��������
							cnt[sockid] = 1;        //��������λ
							return 1;       //�����������
					}
				}
           
       }
    }
    else
    {
			if(USART1_Revice((u8*)buf, 50))         //�Ӵ���3��ȡ����
			{
				if(strstr((const char *)buf, "OK") != NULL) //�������óɹ�
				{
					count[sockid] = 0;      //���Դ�������
					return 1;     //����ִ�����
				}
				else if(strstr((const char *)buf, "913") != NULL) //�������óɹ�
				{
					count[sockid] = 0;      //���Դ�������
					return 1;     //����ִ�����
				}
				
			}
    }
    return 0;
}

static FunctionalState GetConnStatus( NET_PROT protocol)
{
		if(protocol == MQTT_PROT)
		{
			if(hm609a_mqtt_wait_flag && g_hm609aMqttWaitTim <= 0)
			{
				hm609a_mqtt_wait_flag=0;
				hm609a_mqtt_conn_flag=0;
				hm609a_mqtt_reg_flag=0;
			}
			if(!hm609a_mqtt_conn_flag) 
				return DISABLE;
		} else if (protocol == HTTP_PROT)
		{
			if(hm609a_http_wait_flag && g_hm609aHttpWaitTim <= 0)
			{
				hm609a_http_wait_flag=0;
				hm609a_http_conn_flag=0;
			}
			if(!hm609a_http_conn_flag) 
				return DISABLE;
		}
		return ENABLE;
}
/*
��ѭ������ÿ��ѭ��ִ��һ��
*/
void HM609A_Tcp_Program(u8 sockid, char* addr, int port, NET_PROT protocol)
{
	u8 err = 0;    //���صô������
	static u8 count=0,state=0;      // ????,???????
	switch (state)//AIR208״̬����
  {
		case 0:
		{
			if(!hm609a_config_flag)
			{
				if(HM609A_Restart_Program())
        {
            state++;
            count=0;
        }
			}else{
					state++;
					count=0;
			}
		}
		break;
		case 1:     //����ģ��
    {
				if(hm609a_config_flag)
					err=1;
				else
					err = HM609A_Config();
        switch(err)
        {
        case 1:
        {
            //����AT�ɹ�,������һ������
            state++;
						hm609a_config_flag=1;
            count=0;
					
        }
        break;
        case 2:
        {
						state=0;
						count=0;
            HM609A_Restart();
        }
        break;
        default:
            break;
        }
    }
    break;
		case 2:     //ģ�����AT
    {
			err = HM609A_Connect(sockid, addr, port,protocol);
			switch(err)
      {
				 case 0:break; 		//��������,ֱ������
         case 1:
				 {
					 //���ӳɹ�,������һ������
           count=0;
					 hm609a_mqtt_conn_flag=1;
					 hm609a_mqtt_reg_flag=0;
           state+=10;
				 }
				 break;
				 default:
         {
            //����ʧ��
            if(count>=3)
            {
              //�������Դ���������ģ��
              state=0;
							count=0;	
              HM609A_Restart();
            }
            else
            {      
              count++;//���Դ���+1
              state++;
            }
          }
          break;
			}
		}
		break;
		case 3:
		{
			if(HM609A_Tcp_Off(sockid))
				state--;//�ر����Ӻ���������
		}
		break;
		default://TCP���ӳɹ�����ʼ�����շ�
    {
        if(!GetConnStatus(protocol))
					state=3;//������ӶϿ���ִ�жϿ�TCP�����´���TCP����
    }break;
	}
}
u8* g_netData=NULL;

u8 HM609A_Http_Request(const u8 sockid, const char *host,const u32 port)
{
	static u8 count = 0, sign = 0, cnt = 1; //�ظ�����,��������
	u16 len = 0;
	char buf[10];

	if(g_hm609aHttpWaitTim == 0) //Ϊ0ʱ���Ͳ�������
	{
		if(count > 0 && count >= cnt) //��������ظ�����
		{
				count = 0;      //��������
				cnt = 1;        //��������λ
				len = sign + 20; //����������
				sign = 0;      //��������
				return len;     //���ش�����
		}
		else {
			count++;
			switch (sign)//AIR208״̬����
      {
				case 0:
				{
					printf("\r\nsend http request");
					cnt = 3;
					g_hm609aHttpWaitTim = 5000;
					Http_Send_Resquest(sockid,host,port); 
				}
				break;
				default:
				{
						count= 0;      //���Դ�������
            sign = 0;      //��������
            cnt = 1;        //��������λ
            return 1;       //�����������
				}
				break;
			}
		}
	}
	else 
	{
		if(HTTP_Recvice(g_netData, 800))        //�Ӵ���3��ȡ����
		{
			
			if (g_sHttpCmdSta.sta_equip_ident == 1) {
				if (strstr((const char *)g_netData, "Set-Cookie: session="))
				{
					g_sHttpCmdSta.sta_equip_ident=2;
				}
			}
			if (g_sHttpCmdSta.sta_download_bin == 1) {
				if (strstr((const char *)g_netData, "application/octet-stream"))
				{
					g_sHttpCmdSta.sta_download_bin = 2;
				}
			}
			
			g_hm609aHttpWaitTim = 0; //��ʱ����
			count = 0;      //���Դ�������
			sign++;        //��һ������
		}
	}
	return 0;
}

/*
��ѭ������ÿ��ѭ��ִ��һ��
*/
void HM609A_TcpHttp_Program(u8 sockid, char* addr, int port, NET_PROT protocol)
{
	u8 err = 0;    //���صô������
	static u8 state = 0,count = 0;      // ????,???????
	switch (state)//AIR208״̬����
  {
		case 0:
    {
			err = HM609A_Connect(sockid, addr, port, protocol);
			switch(err)
      {
				 case 0:break; 		//��������,ֱ������
         case 1:
				 {
					 //���ӳɹ�,������һ������
           count=0;
					 hm609a_http_conn_flag=1;
           state++;
				 }
				 break;
				 default:
         {
            //����ʧ��
            if(count>=3)
            {
              //�������Դ���������ģ��
              state=0;
							count=0;	
              HM609A_Restart();
            }
            else
            {      
              count++;//���Դ���+1
              state++;
            }
          }
          break;
			}
		}
		break;
		case 1:     //ģ�����AT
    {
			err = HM609A_Http_Request(sockid, addr, port);
			switch(err)
      {
				 case 0:break; 		//��������,ֱ������
         case 1:
				 {
					 //���ӳɹ�,������һ������
           count=0;
           state++;
				 }
				 break;
				 default:
         {
            //����ʧ��
            if(count>=3)
            {
              //�������Դ���������ģ��
              state=0;
							count=0;	
              HM609A_Restart();
            }
            else
            {      
              count++;//���Դ���+1
              state++;
            }
          }
          break;
			}
		}
		break;
		case 2:
		{
			if(HM609A_Tcp_Off(sockid))
			{
				g_sHttpCmdSta.sta_cmd=0;
				state++;//�ر����Ӻ���������
			}

		}
		break;
		default://TCP���ӳɹ�����ʼ�����շ�
    {
      if(hm609a_http_wait_flag && g_hm609aHttpWaitTim <= 0)
			{
				hm609a_http_wait_flag=0;
				hm609a_http_conn_flag=0;
			}
			if(!hm609a_http_conn_flag && g_sHttpCmdSta.sta_cmd) 
					state=0;//������ӶϿ���ִ�жϿ�TCP�����´���TCP����
    }break;
	}
}

void HM609A_Send_Data(u8 sockid, const u8* data, u16 len, u8 flag, NET_PROT protocol)
{
	char *hexStr=NULL;
	if(protocol == MQTT_PROT)
	{	
		if(!hm609a_mqtt_conn_flag)
			return;//δ���ӣ���ֹ����
	}else if (protocol == HTTP_PROT)
	{
		if(!hm609a_http_conn_flag)
			return;//δ���ӣ���ֹ����
	}
	
	if(flag)
	{
		printf("SEND %s\r\n",data);
		USART1_SendData(data,len);	
	}
	else{
		if(hexStr == NULL)
			hexStr = mymalloc(SRAMIN,600);
		to_hex(data, len, hexStr);
		hexStr[len*2] = '\0';
		printf("AT+IPSENDEX=%d,%s, len:%d\r\n",sockid,hexStr,strlen(hexStr));
		u1_printf("\r\nAT+IPSENDEX=%d,\"%s\"\r\n",sockid, hexStr);
	}
	if (protocol == HTTP_PROT)
	{
		g_hm609aHttpWaitTim=10000;
		hm609a_http_wait_flag=1;
	}
	if(hexStr != NULL)
	{
		myfree(SRAMIN,hexStr);
		hexStr=NULL;
	}
		
}

void HM609A_test(void)
{
	
}
