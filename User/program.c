#include "program.h"
#include <stdlib.h>

#define FLASH_CONFIG_ADDR  0x080F0000 	//设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
//jpeg数据接收回调函数
void jpeg_dcmi_rx_callback(void)
{ 
	
}

char mcuIdHex[30]={0};

DEVICE_STATUS device_sta;

u8 *g_appConf=NULL;
#define CONFIG_SIZE 2048
cJSON *g_appConfJson=NULL;

void AppConf_Set(cJSON* root)
{
	g_sDeviceConf.braCode=mcuIdHex;
	AppConfJsonParse(root);
}

char *pwd=NULL;

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
		sprintf(mcuIdHex,"%02x%02x%02x",mcuID[0],mcuID[1],mcuID[2]);
		mcuIdHex[24]='\0';
		printf("MCUID:%s",mcuIdHex);
		
		while(OV2640_Init())
		{
			printf("ov2640_init failed\r\n");
			delay_ms(400);
		}	
		
		MyDCMI_Init();			//DCMI配置
		// icm
		ICM_Init(&icm_serif);
		// hm609a
		HM609A_Init();
		// load config
		g_appConf=mymalloc(SRAMIN, CONFIG_SIZE);
		GDFLASH_Read(FLASH_CONFIG_ADDR,(u32*)g_appConf,CONFIG_SIZE);
		printf("App Conf:%x\r\n",g_appConf[0]);
		if(1){
				AppConf_Init(mcuIdHex,(char*)g_appConf);
				GDFLASH_Write(FLASH_CONFIG_ADDR, \
					(u32*)g_appConf,strlen((char*)g_appConf));
				delay_ms(1000);
				memset(g_appConf,0,CONFIG_SIZE);
				GDFLASH_Read(FLASH_CONFIG_ADDR,(u32*)g_appConf,CONFIG_SIZE);
		}
		// printf("App Conf:\r\n%s",g_appConf);
		g_appConfJson=cJSON_Parse((char*)g_appConf);
		if(!g_appConfJson){
			printf("APP CONF NULL\r\n");
		}
		AppConf_Set(g_appConfJson);
		
		MQTT_Init(mcuIdHex,pwd);
		HTTP_Init();
		delay_ms(2000);
}

void Get_Sensor_Data(void)
{
	get_imu_data();
}

extern u8 *publishbuf;
extern u8 msg_type;
extern u8 g_mqttSubscribeFlag;
extern vu8 g_usart1RevFinish;
extern vu32 g_usart1Cnt; 
char res[20]="+IPURC: \"recv\",2,";
u8 g_identFlag=0;

void Data_Program(void)
{
	u8 ret=0;
	int size=0;
	u32 len = g_usart1Cnt;
	char *http_ptr=NULL;
	char *mqtt_ptr=NULL;
	if(!hm609a_mqtt_conn_flag && !hm609a_http_conn_flag)
		return;
	if(g_usart1RevFinish)
	{
		g_usart1RevFinish = 0;
		USART1_RX_BUF[len]='\0';//?????
		mqtt_ptr= strstr((const char *)USART1_RX_BUF, "+IPURC: \"recv\",2,");
		http_ptr= strstr((const char *)USART1_RX_BUF, "+IPURC: \"recv\",1,");
		if(mqtt_ptr != NULL)
		{	
			mqtt_ptr += strlen(res);
			size = atoi(mqtt_ptr);
			mqtt_ptr = strstr((const char *)mqtt_ptr,"\n");
			memcpy(g_netData, mqtt_ptr+1, size);
			USART1_Clear();
		} 
		else if (http_ptr != NULL)
		{
			u16 resp_code=0;
			http_ptr += strlen(res);
			size = atoi(http_ptr);
			http_ptr = strstr((const char *)http_ptr,"\n");
			memcpy(g_netData, http_ptr+1, size);
			USART1_Clear();
			Http_Post_Analysis_Header(g_netData, size, &resp_code);
			if(resp_code==200)
				g_identFlag=1;
		}else {
			return ;
		}
		
	}
	if(size)
  {
		char *p1=NULL;
		p1 = strstr((char*)g_netData, "HTTP/1.1");	
		if(p1 != NULL)
		{
			printf("\r\nHTTP Recv:%s\r\n",g_netData);
			HTTP_FLAG_TASK=0;
			hm609a_http_wait_flag=0;
			hm609a_http_conn_flag=0;
		}else {
			printf("\r\nMqtt Recv:%s\r\n",g_netData);
			ret=Mqtt_Deserialize_Handle(&msg_type, g_netData, publishbuf);
			if(ret) 
			{
				printf("MQTT return handle successed\r\n");
				hm609a_mqtt_wait_flag=0;
			}
			if(hm609a_mqtt_reg_flag && msg_type == CONNACK)
				g_mqttSubscribeFlag=1;
		}
	}	
}

void MQTT_Data_Program(void)
{
	cJSON *json;
	if(!hm609a_mqtt_conn_flag)return;
	if(MQTT_FLAG_RECEIVE)
		MQTT_Publish_Analysis_Json(publishbuf,json);
	
	if(MQTT_FLAG_PHOTO){
		printf("\r\nServer command: take a photo.\r\n");
		MQTT_FLAG_PHOTO=0;
	}
	if(MQTT_FLAG_PHOTO_THIRD){
		printf("\r\nServer command: third take a photo.\r\n");
		MQTT_FLAG_PHOTO_THIRD=0;
	}
	if(MQTT_FLAG_DELY_OPEN){
		printf("\r\nServer command: open deliver.\r\n");
		MQTT_FLAG_DELY_OPEN=0;
	}
	if(MQTT_FLAG_LOCK_OPEN){
		printf("\r\nServer command: open lock.\r\n");
		MQTT_FLAG_LOCK_OPEN=0;
	}
	if(MQTT_FLAG_LOCK_CLOSE){
		printf("\r\nServer command: close lock.\r\n");
		MQTT_FLAG_LOCK_CLOSE=0;
	}
	if(MQTT_FLAG_CFG_DOWNLOAD){
		printf("\r\nServer command: download config.\r\n");
		MQTT_FLAG_CFG_DOWNLOAD=0;
	}
	if(MQTT_FLAG_CFG_UPLOAD){
		printf("\r\nServer command: upload config.\r\n");
		MQTT_FLAG_CFG_UPLOAD=0;
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

void Get_Device_Status()
{
	get_imu_data();
	device_sta.power_voltage=Get_InVolt_Adc_Val();
	device_sta.temp=Get_TempSensor_Adc_Val();
	printf("volt:%d, temp:%.2f",device_sta.power_voltage, (float)device_sta.temp);
	memcpy(device_sta.acc, accel_g, sizeof(accel_g));
	memcpy(device_sta.gyro, gyro_dps, sizeof(gyro_dps));
	device_sta.temp=temp_degc;
	printf("\r\n temp:%f.\r\nacl:%f,%f,%f\r\ngyo:%f,%f,%f\r\n",\
					device_sta.temp,device_sta.acc[0],device_sta.acc[1],device_sta.acc[2],\
					device_sta.gyro[0],device_sta.gyro[1],device_sta.gyro[2]);
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

