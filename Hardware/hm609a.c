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
	GPIO_SetBits(GPIOF,GPIO_Pin_13); // WakeUp,High, HM609A,High;
	GPIO_SetBits(GPIOF,GPIO_Pin_14); // Fly Mode,High, HM609A,High
	GPIO_ResetBits(GPIOF,GPIO_Pin_15); //high:mcu sleep state
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
*ģ�����ATģʽ
*����ɹ�����1,ִ���з���0,����ʧ�ܷ���10
*/
u8 air208_InAt(void)//ģ�����ATģʽ
{
    static uint8_t count = 0, Signs = 0; //�ظ�����,��������
    char buf[200];
    switch (Signs)//AIR208״̬����
    {
        case 0: //��ʱ1.5s
        {
            count = 0;      //���Դ�������
            Signs++;        //������һ������
            g_hm609aTim =1500;           //��ʱʱ��ms
            USART1_Clear();             //���USART3���ջ���
        }
        break;
        case 1: //����Ƿ���ATģʽ
        {
            if(g_hm609aTim == 0) //���Ͳ�������+++
            {
                if(count>=2)
                {
                    count = 0;      //���Դ�������
                    Signs=0;        //������һ������
                }
                else
                {
                    printf("+++\r\n����AT����ͨ���Ƿ�����%d\r\n",count+1);  //���Ի��� ,�����ǿɻر�
                    g_hm609aTim =1500;           //��ⳬʱʱ��ms
                    count++;                    //���Դ���+1
                    u1_printf("+++");    //����AT����ָ��
                }
            }
        }
        break;
        case 2: //����AT
        {
            if(g_hm609aTim == 0) //����AT�Ƿ�����
            {
                if(count >= 5)
                {
                    //��ʱδ��⵽ATָ��
                    count = 0;      //���Դ�������
                    Signs = 0 ;     //������һ������
                    return 10;  
                }
                else
                {
                   printf("AT\r\n����ATģʽ%d��\r\n", count + 1); //����AT�Ƿ�����
                    g_hm609aTim = 1500;      //��ⳬʱʱ��ms
                    count++;                //���Դ���+1
                    u1_printf("+++");//����AT����ָ��
                }
            }
            else
            {
                if(USART1_Revice(buf))         //�Ӵ���3��ȡ����
                {
										printf("Rev:%s\n",buf);
                    if(strstr((const char *)buf, "OK") != NULL) //����Ƿ�����ؼ���
                    {
                        g_hm609aTim = 0; //��ʱ����
                        count = 0;      //���Դ�������
                        Signs = 0;      //��������
                        return 1;       //���ؽ���AT�ɹ�
                    }
                }
            }
        }
        break;
        default://����AT�ɹ�
        {
            count = 0;      //���Դ�������
            Signs = 0;      //��������
            return 10;      //���ؽ�������ʧ��
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
					case 0: //ȡ�����İ���Թ���
          {
						printf("A|ATE0\r\n");
						g_hm609aTim = 2000;          //��ʱʱ��ms
						cnt = 1;   //�ظ�������,*air208_Tim��ʱ����ʱ��
						strcpy(res_at,"OK"); 
						u1_printf("ATS0?");  //����ATָ��
					}
					break;
					case 1:  // ��ѯ���İ��Ƿ������
          {
						 printf("A|AT+CPIN?\r\n");
             g_hm609aTim = 2000;				//��ʱʱ��ms
             cnt = 30;   //�ظ�������,*air208_Tim��ʱ����ʱ��
             strcpy(res_at, "+CPIN: READY");	//���÷����жϹؼ���
             u1_printf("\r\nAT+CPIN?\r\n");  //����ATָ��
					}
					break;
					case 3: // ��ѯ����ע����������İ���Զ�ע�����磬�ϵ絽ע���� 10s ����
          {
							printf("A|AT+CGREG?\r\n");
							g_hm609aTim = 2000;				//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "+CGREG:0,1");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CGREG?\r\n"); //����ATָ��
          }
					case 4: // ��ѯ�Ƿ�������
					{
							printf("A|AT+CGATT?\r\n");
							g_hm609aTim = 2000;          	//��ʱʱ��ms
							cnt = 60;   //�ظ�������,*air208_Tim��ʱ����ʱ��
							strcpy(res_at, "+CGATT:1");		//���÷����жϹؼ���
							u1_printf("\r\nAT+CGATT?\r\n"); //����ATָ��
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
		if(USART1_Revice(buf))         //�Ӵ���3��ȡ����
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
        err = air208_InAt();
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
            //����ʧ��,����ģ��
            count=0;
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
			err = HM609A_config();
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
               // air208_restart();
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
