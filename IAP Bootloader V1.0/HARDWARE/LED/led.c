#include "led.h" 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6���İ�
//LED��������	   
//�汾��V1.0				  							  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{    	 
	RCC->AHB1ENR|=1<<4;//ʹ��PORTFʱ�� 
	GPIO_Set(GPIOF,PIN2|PIN3|PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF9,PF10����
}






