#include "program.h"

//jpeg���ݽ��ջص�����
void jpeg_dcmi_rx_callback(void)
{ 
	
}

void Program_Init(void)
{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    systick_config();
		delay_init(200);    //��ʼ����ʱ����
		LED_Init();
		LED_PWM_Init();
	  InVolt_Adc_Init(); 
		TIM1_Init(99,999); //��ʱ��ʱ��100M����Ƶϵ��1000������100M/1000=100Khz�ļ���Ƶ�ʣ�����100��Ϊ1ms 
		TIM3_PWM_Init(499,99);	//100M/100=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz.  
		usart2_init(115200);
		usart1_init(115200);
		Speaker_Init();
		my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
		SPEAKER_SW=1;
	
		while(OV2640_Init())//��ʼ��OV2640
		{
			printf("ov2640_init failed\r\n");
			delay_ms(400);
		}
		delay_ms(2000);
		MyDCMI_Init();			//DCMI����
		HM609A_Init();
		OutDirveInit();
}

void Program_Progress(void)
{
	
}
