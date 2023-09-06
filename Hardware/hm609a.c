#include "hm609a.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"
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
		GPIO_SetBits(GPIOF,GPIO_Pin_11);	// Pwrkey,High, HM609A,Low time >550ms on/off
		delay_ms(1000);
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	}
	printf("\r\nHM609A Init successed.status:%d\n",status);
}

char res_at[20];

u32  g_hm609aTim = 0;        // ?????
u16  g_hm609aMqttWaitTim=0;	// ?????
u16  g_hm609aHttpWaitTim=0;
Byte8 g_hm609aFlag;
/*
????1ms????
*/
void HM609A_TIM_1ms(void)
{
    if(g_hm609aTim)g_hm609aTim--;
	
    if(hm609a_mqtt_wait_flag)
    {
      if(g_hm609aMqttWaitTim)g_hm609aMqttWaitTim--;
    }
		
	 if(hm609a_http_wait_flag)
    {
      if(g_hm609aHttpWaitTim)g_hm609aHttpWaitTim--;
    }
		
}

/*
*����ģ�鴦����
*
*/
void HM609A_Restart(void)
{
    DG_RESET=0;             		// ģ�鸴λ
    g_hm609aTim=0;         		// ģ�鵹��ʱ����
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
u8 HM609A_Restart_Program(void)//ģ����������
{
    static u8 sign = 0;  //��������
    if(g_hm609aTim <= 0) //����ʱ������������̴���,���򷵻�0δ���
    {
        switch (sign)//AIR208״̬����
        {
        case 0: //�ϵ翪ʼ
        {
            DG_RESET_OFF;         //�ر�ģ���Դ
            g_hm609aTim = 300;  //�ϵ�ʱ��ms
            sign++;            //������һ������
        }
        break;
        case 1: //�ϵ�
        {
            DG_RESET_ON;          //��ģ���Դ
            g_hm609aTim = 5000;  //�ϵ��ʼ��ʱ��ms
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
u8 HM609A_Config(void)
{
	static u8 count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
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
					case 7: // ��ѯIPֵ
					{
							printf("A|AT+IPDNSR=\"debug.armlogic.tech\"\r\n");
							g_hm609aTim = 3000;          	//��ʱʱ��ms
							cnt = 10;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nAT+IPDNSR=\"debug.armlogic.tech\"\r\n"); //����ATָ��
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

/*
*ģ��TCP����
*�ɹ�����1,ִ���з���0,ʧ�ܷ��ش������>=20
*/
u8 HM609A_Connect(u8 sockid, char* addr, int port)
{
	static u8 count = 0, Signs = 0, cnt = 1;
	char buf[200]={0};
	if(g_hm609aTim == 0)
	{
		if(count > 0 && count >= cnt) //��������ظ�����
		{
				count = 0;      //��������
				cnt = 1;        //��������λ
				Signs = 0;      //��������
				return Signs + 30;     //���ش�����
		} else {
			count++;
			switch (Signs)//AIR208״̬����
      {
				case 0: //
        {
					printf("\r\nA|AT+IPOPEN=%d,\"TCP\",%s,%d,0,0\r\n",sockid,addr,port);
					cnt = 3;
					g_hm609aTim = 5000;
					strcpy(res_at,"OK");
					u1_printf("\r\nAT+IPOPEN=%d,\"TCP\",\"%s\",%d,0,0\r\n",sockid,addr,port);  //����ATָ��
				}
				break;
				case 1:
				{
					printf("\r\nA|AT+IPSWTMD=%d,1\r\n",sockid);
					cnt = 10;
					g_hm609aTim = 2000;
					strcpy(res_at,"OK");
					u1_printf("\r\nAT+IPSWTMD=%d,1\r\n",sockid);  //����ATָ��
				}
				break;
				case 2:
				{
					printf("\r\nA|ATE0\r\n");
					cnt = 3;
					g_hm609aTim = 2000;
					strcpy(res_at,"OK");
					u1_printf("\r\nATE0\r\n");  //����ATָ��
				}
				break;
				default:// �ȴ����ӽ����ɹ�
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

/*
*�ر�TCP����
*
*/
u8 HM609A_Tcp_Off(u8 sockid)// �ر�TCP����
{
    static u8 count = 0; //�ظ�����,��������
    char buf[50];
    if(g_hm609aTim == 0)
    {
        if(count >= 2) //��������ظ�����
        {
            count = 0;      //��������
            return 41;     //���ش�����
        }
        else{
            printf("A|AT+IPCLOSE=%d",sockid);    // �ر�TCP����
            g_hm609aTim = 1000;      //��ʱʱ��ms
            count++;
            u1_printf("\r\nAT+IPCLOSE=%d\r\n",sockid);     //����ATָ��
        }
    }
    else
    {
        if(USART1_Revice((u8*)buf))         //�Ӵ���3��ȡ����
        {
            if(strstr((const char *)buf, "OK") != NULL) //�������óɹ�
            {
                count = 0;      //���Դ�������
                return 1;     //����ִ�����
            }
        }
    }
    return 0;
}

static FunctionalState GetConnStatus( NET_PROTOCOL protocol)
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
void HM609A_Tcp_Program(u8 sockid, char* addr, int port, NET_PROTOCOL protocol)
{
	u8 err = 0;    //���صô������
	static u8 count = 0;  //�ظ�����
	static u16 state = 0;      // ????,???????
	switch (state)//AIR208״̬����
  {
		case 0:
		{
				if(HM609A_Restart_Program())
        {
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
			err = HM609A_Connect(sockid, addr, port);
			switch(err)
      {
				 case 0:break; 		//��������,ֱ������
         case 1:
				 {
					 //���ӳɹ�,������һ������
           count=0;
					 if (protocol == MQTT_PROT){
							hm609a_mqtt_conn_flag=1;
							hm609a_mqtt_reg_flag=0;
					 }else if (protocol == HTTP_PROT){
							hm609a_http_conn_flag=1;
					 }
           state+=10;
				 }
				 break;
				 default:
         {
            //����ʧ��
            if(count>=3)
            {
              //�������Դ���������ģ��
              count=0;
							state=0;
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


void HM609A_Send_Data(u8 sockid, const u8* data, u16 len, u8 flag, NET_PROTOCOL protocol)
{
	char *hexStr;
	if(protocol == MQTT_PROT)
	{	
		if(!hm609a_mqtt_conn_flag)
			return;//δ���ӣ���ֹ����
	}else if (protocol == HTTP_PROT)
	{
		if(!hm609a_http_conn_flag)
			return;//δ���ӣ���ֹ����
	}
	
	hexStr = mymalloc(SRAMIN,600);
	to_hex(data, len, hexStr);
	hexStr[len*2] = '\0';
	if(flag)
	{
		printf("\r\nAT+IPSEND=%d,%d\r\n",sockid,len);
		USART1_SendData(data,len);
	}
	else{
		printf("\r\nAT+IPSENDEX=%d,%s\r\n",sockid,hexStr);
		u1_printf("\r\nAT+IPSENDEX=%d,\"%s\"\r\n",sockid, hexStr);
	}
	
	if(protocol == MQTT_PROT)
	{	
		// release�汾��Ϊ1000
		g_hm609aMqttWaitTim=10000;
		hm609a_mqtt_wait_flag=1;
	}else if (protocol == HTTP_PROT)
	{
		g_hm609aHttpWaitTim=10000;
		hm609a_http_wait_flag=1;
	}
	myfree(SRAMIN,hexStr);
}

void HM609A_test(void)
{
	
}
