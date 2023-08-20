#include "hm609a.h"
#include "delay.h"
#include "usart.h"

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
					case 1:  // ��ѯϵͳ��Ϣ��SIM״̬
          {
						 printf("A|AT^SYSINFO\r\n");
             g_hm609aTim = 2000;				//��ʱʱ��ms
             cnt = 30;   //�ظ�������,*air208_Tim��ʱ����ʱ��
             strcpy(res_at, "^SYSINFO");	//���÷����жϹؼ���
             u1_printf("\r\nAT^SYSINFO\r\n");  //����ATָ��
					}
					break;
					case 2: // ��ѯ����ע����������İ���Զ�ע�����磬�ϵ絽ע���� 10s ����
          {
							printf("A|ATI\r\n");
							g_hm609aTim = 2000;				//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "OK");		//���÷����жϹؼ���
							u1_printf("\r\nATI\r\n"); //����ATָ��
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
					case 5: // ��ѯSNֵ
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
				switch (Signs)//AIR208״̬����
        {
					case 0: //
          {
						printf("A|AT+HTTPCFG,resquestheader\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=\"requestheader\",0\r\n");  //����ATָ��
					}
					break;
					case 1: //
          {
						printf("A|AT+HTTPCFG,responseheader\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=\"responseheader\",1\r\n");  //����ATָ��
					}
					break;
					case 2: //
          {
						printf("A|AT+HTTPCFG,rspout auto\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=\"rspout/auto\",0\r\n");  //����ATָ��
					}
					break;
					case 3: //ȡ�����İ���Թ���
          {
						printf("A|AT+HTTPURL=118,80\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"CONNECT"); 
						u1_printf("AT+HTTPCFG=\"rspout/auto\",0\r\n");  //����ATָ��
					}
					break;
					case 4: //ȡ�����İ���Թ���
          {
						printf("A|AT+HTTPURL=118,80\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"CONNECT"); 
						u1_printf("http://101.57.89.157\r\n");  //����ATָ��
					}
					break;
					case 5: //ȡ�����İ���Թ���
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

u8 HM609A_Mqtt(void)
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
				switch (Signs)//AIR208״̬����
        {
					case 0: //
          {
						printf("A|AT+HTTPCFG,resquestheader\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=\"requestheader\",0\r\n");  //����ATָ��
					}
					break;
					case 1: //
          {
						printf("A|AT+HTTPCFG,responseheader\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=\"responseheader\",1\r\n");  //����ATָ��
					}
					break;
					case 2: //
          {
						printf("A|AT+HTTPCFG,rspout auto\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("AT+HTTPCFG=\"rspout/auto\",0\r\n");  //����ATָ��
					}
					break;
					case 3: //ȡ�����İ���Թ���
          {
						printf("A|AT+HTTPURL=118,80\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"CONNECT"); 
						u1_printf("AT+HTTPCFG=\"rspout/auto\",0\r\n");  //����ATָ��
					}
					break;
					case 4: //ȡ�����İ���Թ���
          {
						printf("A|AT+HTTPURL=118,80\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"CONNECT"); 
						u1_printf("http://101.57.89.157\r\n");  //����ATָ��
					}
					break;
					case 5: //ȡ�����İ���Թ���
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
/*
��ѭ������ÿ��ѭ��ִ��һ��
*/
void HM609A_Program(void)
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
			err = HM609A_Http();
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
			err = HM609A_Mqtt();
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
        if(!hm609a_connect_flag)g_hm609aState=5;//������ӶϿ���ִ�жϿ�TCP�����´���TCP����
    }break;
	}
}

void HM609A_test(void)
{
	
}
