#include "program.h"

//jpeg数据接收回调函数
void jpeg_dcmi_rx_callback(void)
{ 
	
}

char mcuIdHex[30]={0};

void Program_Init(void)
{
		struct inv_imu_serif icm_serif;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    systick_config();
		CpuIDGetId();
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
		sprintf(mcuIdHex,"%02x%02x%02x",mcuID[0],mcuID[1],mcuID[2]);
		mcuIdHex[24]='\0';
		printf("MCUID:%s",mcuIdHex);
		MyDCMI_Init();			//DCMI配置
		HM609A_Init();
	  MQTT_Init(mcuIdHex);
		delay_ms(2000);
		//icm
		ICM_Init(&icm_serif);
		
}

extern u8 *publishbuf;

void MQTT_Data_Program(void)
{
	cJSON *json;
	if(!hm609a_mqtt_conn_flag)return;
	if(MQTT_FLAG_REVICE)
		MQTT_Publish_Analysis_Json(publishbuf,json);
	
	if(MQTT_FLAG_PHOTO){
		printf("\r\nServer command: take a photo.\r\n");
		MQTT_FLAG_PHOTO=0;
	}
	if(MQTT_FLAG_PHOTO_THIRD){
		printf("\r\nServer command: third take a photo.\r\n");
		MQTT_FLAG_PHOTO_THIRD=0;
	}
	if(MQTT_FLAG_DELIVER_OPEN){
		printf("\r\nServer command: open deliver.\r\n");
		MQTT_FLAG_DELIVER_OPEN=0;
	}
	if(MQTT_FLAG_LOCK_OPEN){
		printf("\r\nServer command: open lock.\r\n");
		MQTT_FLAG_LOCK_OPEN=0;
	}
	if(MQTT_FLAG_LOCK_CLOSE){
		printf("\r\nServer command: close lock.\r\n");
		MQTT_FLAG_LOCK_CLOSE=0;
	}
	if(MQTT_FLAG_CONFIG_DOWNLOAD){
		printf("\r\nServer command: download config.\r\n");
		MQTT_FLAG_CONFIG_DOWNLOAD=0;
	}
	if(MQTT_FLAG_CONFIG_UPLOAD){
		printf("\r\nServer command: upload config.\r\n");
		MQTT_FLAG_CONFIG_UPLOAD=0;
	}
	if(MQTT_FLAG_RESTART){
		printf("\r\nServer command: restart device.\r\n");
		MQTT_FLAG_RESTART=0;
	}
	if(MQTT_FLAG_UPGRADE){
		printf("\r\nServer command: app upgrade.\r\n");
		MQTT_FLAG_UPGRADE=0;
	}
	if(MQTT_FLAG_LOG_LIST){
		printf("\r\nServer command: upload log list.\r\n");
		MQTT_FLAG_LOG_LIST=0;
	}
	if(MQTT_FLAG_LOG_INFO){
		printf("\r\nServer command: upload log info.\r\n");
		MQTT_FLAG_UPGRADE=0;
	}
}

void Program_Test(void)
{
		//	LED_PWM_Test();
		//	flash_id=F35SQA_ReadID();	//读取FLASH ID.
		//	printf("F35SQA_ID:%x",flash_id);
		//	delay_ms(1000);
		//	get_imu_data();
			//Sensor_Adc_Test();
			//Open_Lock_Test();
			// LED_Test();
			//DAC1_Test( 36,4096 );
      // usart2_test();
}

void Program_Progress(void)
{
	
}
