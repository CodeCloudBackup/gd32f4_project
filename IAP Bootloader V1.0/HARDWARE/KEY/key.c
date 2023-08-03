#include "key.h"
#include "delay.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6���İ�
//����������������	   
//�汾��V1.0							  
////////////////////////////////////////////////////////////////////////////////// 	 

//������ʼ������
void KEY_Init(void)
{
	RCC->AHB1ENR|=1<<0;     //ʹ��PORTAʱ�� 
	RCC->AHB1ENR|=1<<4;     //ʹ��PORTEʱ��
	GPIO_Set(GPIOE,PIN4,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);			//PE4������������
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_IN,0,0,GPIO_PUPD_PD); 		//PA0����Ϊ�������� 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//4��KEY_UP���� ��WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==1||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==1)return KEY0_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==0&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}




















