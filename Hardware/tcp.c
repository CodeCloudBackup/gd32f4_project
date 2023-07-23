#include "tcp.h"
#include "esp8266.h"
#include <stdio.h>
#include <string.h>
#include "systick.h"
#include "usart.h"
#include "delay.h"

char res_at[20];

vu8 TcpClosedFlag = 0;
u16 g_state = 0;
u16 g_send_return_tim = 0;
u8 g_signal = 0;
u16 g_heartbeat = 0;
u32 g_tcpTime = 0;
Byte8 _esp8266_flag;

void TCP_TIM_10ms(void)
{
		if(g_tcpTime) g_tcpTime--;
		if(esp8266_send_return) {
				if(g_send_return_tim)g_send_return_tim--;
		}
}

void TCP_Restart(void)
{
		g_state = 0;
		ESP8266_RST=0;
		g_tcpTime = 0;
		esp8266_conn_flag = 0;
		esp8266_reg_flag = 0;
}
	
u8 TCP_Restart_Program(void)
{
	static u8 signs = 0;
	if(g_tcpTime <= 0){
		switch(signs)
		{
			case 0:
			{
				// �ر�ģ���Դ
				ESP8266_RST=0;
				g_tcpTime = 50;
				signs++;
			}
			break;
			case 1:
			{
				// ��ģ���Դ
				ESP8266_RST=1;
				g_tcpTime = 1500;
				signs++;
			}
			break;
			default:
			{
				signs = 0;
				return 1;
			}
		}	
	}
	return 0;
}

u8 TCP_InAT(void)
{
		static u8 cnt = 0, sign = 0;
		switch(sign)
		{
			case 0:
			{
				cnt = 0;
				sign++;
				g_tcpTime = 150;
				USART2_Clear(); //��ս��ջ���
			}
			break;
			case 1:
			{
				if(g_tcpTime == 0)
				{
					if(cnt >= 2)	// ���Ͳ�������++
					{
						cnt = 0;
						sign++;
					}else{
						printf("+++\r\nAT�������%d\r\n",cnt+1);
						g_tcpTime = 150;
						cnt++;
						u2_printf("+++");
					}
				}
			}
			break;
			case 2:
			{
				if(g_tcpTime == 0)
				{
					if(cnt >= 5)
					{
						cnt = 0;
						sign = 0;
						return 10;
					}else {
					 	printf("AT\r\n����ATģʽ%d\r\n",cnt+1);
						g_tcpTime = 150;
						cnt++;
						u2_printf("AT\r\n");
					}
				}else{
					if(ESP8266_Check_AT_Response("OK")){
							g_tcpTime = 0;	//��ʱ����
							cnt = 0;		//���Դ�������
							sign = 0;	  //��������
							return 1;   //���ؽ���AT�ɹ�
					}
				}
			}
			break;
			default:
			{
					cnt = 0;
					sign = 0;
					return 10;
			}
		}
		return 0;
}

u8 TCP_gprs(void)//��ȡGPRS��λ��Ϣ
{
	return 1;
}

/*
*�ر�TCP����
*
*/
u8 ESP8266_tcp_off(void)// �ر�TCP����
{
    static u8 count = 0; //�ظ�����,��������
		if(g_tcpTime == 0)
    {
        if(count >= 2) //��������ظ�����
        {
            count = 0;      //��������
            return 41;     //���ش�����
        }
        else{
//            printf("A|AT+CIPCLOSE");    // �ر�TCP����
            g_tcpTime = 100;      //��ʱʱ��ms
            count++;
            u2_printf("AT+CIPCLOSE\r\n");     //����ATָ��
        }
    }
    else
    {
				if(ESP8266_Check_AT_Response("OK"))
				{
						count = 0;      //���Դ�������
						return 1;     //����ִ�����
				}
				
    }
    return 0;
}

/*
*ģ������
*�ɹ�����1,ִ���з���0,ʧ�ܷ��ش������>=20
*/

static char res_at[20];
u8 TCP_config(void)
{
    static u8 count = 0, Signs = 0, cnt = 1; //�ظ�����,��������
    u16 len = 0;
    if(g_tcpTime == 0) //Ϊ0ʱ���Ͳ�������
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
										ESP8266_AT_ATEO(res_at);
                    g_tcpTime = 200;          //��ʱʱ��ms
                    cnt = 3;   //�ظ�������,*air208_Tim��ʱ����ʱ��

                }
                break;
                case 1: //  ���ò�����
                {
										ESP8266_AT_BaudRate(res_at);
                   printf("A|��AT+IPR=115200;&W\r\n");
                    g_tcpTime = 200;          //��ʱʱ��ms
                    cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
                }
                break;
                case 2: // ����ΪSTAģʽ
                {
                   printf("A|AT+CIPMODE=0\r\n");
                    g_tcpTime = 200;          	//��ʱʱ��ms
                    cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
                    ESP8266_Net_Mode_Choose(STA,res_at);
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
				if(ESP8266_Check_AT_Response(res_at))
				{
					 g_tcpTime = 0; //��ʱ����
           count = 0;      //���Դ�������
           Signs++;        //��һ������
				}
    }
    return 0;
}

u8 TCP_Connect(void)// ����TCP����
{
	static u8 count = 0, sign = 0,cnt = 1;
	u16 len = 0;
	char response[100];
	if(g_tcpTime == 0)
	{
		 if(count > 0 && count >= cnt) //��������ظ�����
     {
            count = 0;      //��������
            cnt = 1;        //��������λ
            len = sign + 20; //����������
            sign = 0;      //��������
            return len;     //���ش�����
     }
		 else
		 {	
					count++;
					switch(sign)
					{
						case 0:
						{
							  g_tcpTime = 200;          //��ʱʱ��ms
                cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
								printf("Connect to WIFI %s\r\n",User_ESP8266_PWD);
								ESP8266_AT_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD, res_at);
						}
						break;
						case 1:
						{
								g_tcpTime = 200;          	//��ʱʱ��ms
								cnt = 5;   //�ظ�������,*air208_Tim��ʱ����ʱ��
                ESP8266_Enable_MultipleId(DISABLE,res_at);
						}
						break;
						case 2:
						{
								cnt = 3;
								g_tcpTime  = 100*5;
								printf("Connect to Server %s\r\n",TCPServer_IP);		
								ESP8266_Link_Server(enumTCP,TCPServer_IP,TCPServer_PORT,Single_ID_0,res_at);
						}
						break;
						default:// ״̬����ִ�����
            {
                    count = 0;      //���Դ�������
                    sign = 0;      //��������
                    cnt = 1;        //��������λ
                    return 1;       //�����������
            }
					}
			}

		}
		else
		{
				if(USART2_Revice(COMMAND,response))         //�Ӵ���3��ȡ����
        {
            if(strstr((const char *)response, (const char *)res_at) != NULL) //����Ƿ�����ؼ���
            {
                g_tcpTime = 0; //��ʱ����
                count = 0;      //���Դ�������
                sign++;        //��һ������
            }else if (strstr((const char *)response, "ALREADY CONNECT") != NULL){
								g_tcpTime = 0; //��ʱ����
                count = 0;      //���Դ�������
                sign++;        //��һ������
						}else if(strstr((const char *)response, "ERROR") != NULL)
						{
								g_tcpTime = 0; //��ʱ����
                count = 0;      //���Դ�������
                sign=0;        //���̸�λ
                return 32;     //���ش�����
						}
        } 
		}		
	return 0;
}

void TCP_Program(void)
{
		u8 errCode = 0;
		static u8 cnt = 0;
		switch(g_state)
		{
			case 0:
			{
				if(TCP_Restart_Program()){
						g_state++;
						cnt=0;
				}
			}
			break;
			case 1:
			{
				errCode = TCP_InAT();
				switch(errCode)
				{
					case 1:
					{
						// ����AT�ɹ�,������һ��
						g_state++;
						cnt=0;
					}
					break;
					case 2:
					{
						// ����ʧ��,����ģ��
						cnt = 0;
						TCP_Restart();
					}
					break;
					default:
						break;
				}
			}
			break;
			case 2: // ģ���ʼ��
			{
					errCode = TCP_config();
					switch(errCode)
					{
						case 0:break;
						case 1:
						{
							cnt = 0;
							g_state++;
						}
						break;
						default:
						{
							TCP_Restart();
						}
						break;
					}
			}
			break;
			case 3: // ��ȡGPS��λ��Ϣ
			{
				errCode = TCP_gprs();
				switch(errCode)
				{
						case 0:break;
						default:
            {
				//�����Ƿ�ɹ���ȡ��������һ������
                cnt=0;
                g_state++;
            }
            break;
				}
			}
			break;
			case 4: // ��������
			{
					errCode = TCP_Connect();
					switch(errCode)
					{
						case 0:break;
						case 1:
						{
								cnt= 0;
								esp8266_conn_flag = 1;// ���ӳɹ�
								esp8266_reg_flag = 0;
								g_state+=10; 
						}
						break;
						default:
						{
								if(cnt >= 3){
									cnt = 0;
									TCP_Restart();
								} else {
									cnt++;
									g_state++;
								}
						}
						break;
					}
			}
			break;
			case 5: // �ر�����
			{
				if(ESP8266_tcp_off())g_state--;
			}
			break;
			default:  //TCP���ӳɹ�����ʼ�����շ�
			{
				if(esp8266_send_return&&g_send_return_tim<=0)
				{
					esp8266_send_return=0;
					esp8266_conn_flag=0;
					esp8266_reg_flag=0;
				}
				if(!esp8266_conn_flag)g_state=5;
			}
			break;
		}
}

BOOL TCP_Send_Data(char *data,  uint16_t len)
{
		if(!esp8266_conn_flag) return FALSE;
		u2_printf("AT+CIPSEND=%d\r\n",len);
		delay_ms(10);
		USART2_Send(data,len);
		esp8266_send_return=1;
		g_send_return_tim = 30000;
		return TRUE;
}
