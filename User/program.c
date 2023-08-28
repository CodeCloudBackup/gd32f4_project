#include "program.h"

//jpeg数据接收回调函数
void jpeg_dcmi_rx_callback(void)
{ 
	
}

void Program_Init(void)
{
		struct inv_imu_serif icm_serif;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    systick_config();
		CpuIDGetId();
		printf("MCUID:%x,%x,%x",mcuID[0],mcuID[1],mcuID[2]);
		delay_init(200);    //初始化延时函数
		LED_Init();
		LED_PWM_Init();
		OutDirveInit();
	  InVolt_Adc_Init(); 
		TempSensor_Adc_Init();
		TIM1_Init(99,999); //定时器时钟100M，分频系数1000，所以100M/1000=100Khz的计数频率，计数100次为1ms 
		TIM3_PWM_Init(499,99);	//100M/100=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.  
		usart2_init(115200);
		usart1_init(115200);
		Speaker_Init();
		SPEAKER_SW=1;
		//初始化内部内存池 
		my_mem_init(SRAMIN);		//	while(OV2640_Init())//初始化OV2640
	 
		//F35SQA_Init();
		while(OV2640_Init())
		{
			printf("ov2640_init failed\r\n");
			delay_ms(400);
		}
	
		MyDCMI_Init();			//DCMI配置
		HM609A_Init();
		delay_ms(2000);
		//icm
		ICM_Init(&icm_serif);
		
}

void Program_Progress(void)
{
	
}
