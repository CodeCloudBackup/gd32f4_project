#include "program.h"
#include <stdlib.h>

//jpeg数据接收回调函数
void jpeg_dcmi_rx_callback(void)
{ 
	
}
#define CONFIG_SIZE 2048
char mcuIdHex[30]={0};



u8 *g_appConf=NULL;
cJSON *g_appConfJson=NULL;
Byte8 ProgramFlag;

u32 g_programTim=0;
void Program_TIM_100ms(void)
{
	g_programTim++;
	if(g_programTim > 500)
	{
		PROGRAM_ICM_DATA_FLAG=1;
		g_programTim=0;
	}
}

void AppConf_Set(cJSON* root)
{
	g_sDeviceConf.braCode=mcuIdHex;
	AppConfJsonParse(root);
}

void Program_Flag_Init(void)
{
	PROGRAM_TAKE_PHOTO_FLAG=1;  
	PROGRAM_ICM_DATA_FLAG=1; 	
	PROGRAM_OPEN_DELY_FLAG=0;
	PROGRAM_RESERT_FLAG=0;
	PROGRAM_APPFLASH_FLAG=0;
	PROGRAM_SPEAKER_FLAG=0;
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
		GDFLASH_Read(FLASH_CUR_CONFIG_ADDR,(u32*)g_appConf,CONFIG_SIZE);
		printf("App Conf:%x\r\n",g_appConf[0]);
		if(1){
			AppConf_Init(mcuIdHex,(char*)g_appConf);
			GDFLASH_Write(FLASH_CUR_CONFIG_ADDR, \
				(u32*)g_appConf,strlen((char*)g_appConf));
			delay_ms(1000);
			memset(g_appConf,0,CONFIG_SIZE);
			GDFLASH_Read(FLASH_CUR_CONFIG_ADDR,(u32*)g_appConf,CONFIG_SIZE);
		}
		// printf("App Conf:\r\n%s",g_appConf);
		g_appConfJson=cJSON_Parse((char*)g_appConf);
		if(!g_appConfJson){
			printf("APP CONF NULL\r\n");
		}
		AppConf_Set(g_appConfJson);
		if(g_appConf != NULL)
		{
			myfree(SRAMIN, g_appConf);
			g_appConf=NULL;
		}
		MQTT_Init(mcuIdHex,pwd);
		HTTP_Init();
		Program_Flag_Init();
		delay_ms(2000);
}

extern u8 *publishbuf;
 
u8* g_httpContent=NULL;
u32 g_contLen=0;

void HTTP_Data_Program(void)
{
	if (g_sHttpCmdSta.sta_equip_ident == 2)
	{
		printf("\r\nHttp Response: equip ident.\r\n");
		g_sHttpCmdSta.sta_equip_ident = 0;
		g_sHttpCmdSta.sta_cmd=0;
	}
	else if(g_sHttpCmdSta.sta_download_bin == 2)
	{
		printf("\r\nHttp Response: download bin.\r\n");
		PROGRAM_APPFLASH_FLAG=1;
		g_sHttpCmdSta.sta_download_bin = 0;
	}
	else if (g_sHttpCmdSta.sta_upload_photo == 2)
	{
		printf("\r\nHttp Response: upload photo.\r\n");
		g_sHttpCmdSta.sta_upload_photo = 0;
	}
	else if (g_sHttpCmdSta.sta_upload_logfile == 2)
	{
		printf("\r\nHttp Response: upload logfile.\r\n");
		g_sHttpCmdSta.sta_upload_logfile = 0;
	}
}

void MQTT_Data_Program(void)
{
	cJSON *json=NULL;
	if(!hm609a_mqtt_conn_flag)return;
	if(MQTT_FLAG_RECEIVE)
	{
		json=cJSON_Parse((char*)publishbuf);
		if(!json)
		{
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
			return;
		}
	}		
	
	if(MQTT_FLAG_PHOTO){
		printf("\r\nServer command: take a photo.\r\n");
		g_sHttpCmdSta.sta_upload_photo=1;
		MQTT_FLAG_PHOTO=0;
	}
	if(MQTT_FLAG_PHOTO_THIRD){
		printf("\r\nServer command: third take a photo.\r\n");
		MQTT_FLAG_PHOTO_THIRD=0;
	}
	if(MQTT_FLAG_DELY_OPEN){
		printf("\r\nServer command: open deliver.\r\n");
		DelyJsonParse(json);
		if(g_sDelyInfo.braCode)
		{
			PROGRAM_OPEN_DELY_FLAG=1;
		}
		MQTT_FLAG_DELY_OPEN=0;
	}
	if(MQTT_FLAG_DELY_CLOSE){
		printf("\r\nServer command: open deliver.\r\n");
		DelyJsonParse(json);
		if(g_sDelyInfo.type == 0)
		{
			PROGRAM_CLOSE_DELY_FLAG=1;
		}
		MQTT_FLAG_DELY_CLOSE=0;
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
		char *str=NULL;
		printf("\r\nServer command: download config.\r\n");
		AppConfJsonParse(json);
		str=cJSON_Print(json);
		GDFLASH_Read(FLASH_CUR_CONFIG_ADDR,(u32*)str,strlen(str));
		MQTT_FLAG_CFG_DOWNLOAD=0;
	}
	if(MQTT_FLAG_CFG_UPLOAD){
		printf("\r\nServer command: upload config.\r\n");
		MQTT_FLAG_CFG_UPLOAD=0;
	}
	if(MQTT_FLAG_RESTART){
		printf("\r\nServer command: restart device.\r\n");
		if(ResetJsonParse(json))
			PROGRAM_RESERT_FLAG=1;
		MQTT_FLAG_RESTART=0;
	}
	if(MQTT_FLAG_UPGRADE){
		char *str=NULL;
		printf("\r\nServer command: app upgrade.\r\n");
		UpgradeJsonParse(json);
		str=cJSON_Print(json);
		GDFLASH_Read(FLASH_BIN_CONFIG_ADDR,(u32*)str,strlen(str));
		g_sHttpCmdSta.sta_download_bin=1;
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
	if(json	!= NULL)
		cJSON_Delete(json);
}
extern char *g_barCode;
void GetDeviceStatus()
{
	char camCode[] = "/dev/video0";
	char simCode[] = "866390060039821";
	get_imu_data();
	
	g_sDeviceSta.power_voltage=Get_InVolt_Adc_Val();
	g_sDeviceSta.area_vacancy=g_sDeviceSta.power_voltage/BATTERY_VLOT;
	g_sDeviceSta.power_remain=g_sDeviceSta.area_vacancy;
	
	memcpy(g_sDeviceSta.cameSerialCode,camCode,sizeof(camCode));
	g_sDeviceSta.barCode=g_barCode;
	memcpy(g_sDeviceSta.simCode,simCode,sizeof(simCode));
	g_sDeviceSta.temp=Get_TempSensor_Adc_Val();
	printf("volt:%d, temp:%.2f",g_sDeviceSta.power_voltage, (float)g_sDeviceSta.temp);
	memcpy(g_sDeviceSta.acc, accel_g, sizeof(accel_g));
	memcpy(g_sDeviceSta.gyro, gyro_dps, sizeof(gyro_dps));
	g_sDeviceSta.temp=temp_degc;
	printf("\r\n temp:%.2f.\r\nacl:%.2f,%.2f,%.2f\r\ngyo:%.2f,%.2f,%.2f\r\n",\
					g_sDeviceSta.temp,g_sDeviceSta.acc[0],g_sDeviceSta.acc[1],g_sDeviceSta.acc[2],\
					g_sDeviceSta.gyro[0],g_sDeviceSta.gyro[1],g_sDeviceSta.gyro[2]);
}


void Device_Program(void)
{
	if(PROGRAM_TAKE_PHOTO_FLAG)
	{
		OV2640_Jpg_Photo();
		g_sHttpCmdSta.sta_upload_photo=0;
		PROGRAM_TAKE_PHOTO_FLAG=0;
	} 
	if(PROGRAM_APPFLASH_FLAG)
	{
		if(g_httpContent != NULL)
		{	
			GDFLASH_Write(FLASH_APPBIN_ADDR, \
				(u32*)g_httpContent, g_contLen);
			myfree(SRAMIN,g_httpContent);
			g_httpContent=NULL;
		}	
		
		PROGRAM_APPFLASH_FLAG=0;
	}
	if(PROGRAM_ICM_DATA_FLAG)
	{
		
		GetDeviceStatus();
		PROGRAM_ICM_DATA_FLAG=0;
	}
	if(PROGRAM_OPEN_DELY_FLAG)
	{
		PROGRAM_OPEN_DELY_FLAG=0;
	}
	if(PROGRAM_CLOSE_DELY_FLAG)
	{
		PROGRAM_CLOSE_DELY_FLAG=0;
	}
	if(PROGRAM_RESERT_FLAG)
	{
		PROGRAM_RESERT_FLAG=0;
	}
	
	if(PROGRAM_SPEAKER_FLAG)
	{
		PROGRAM_SPEAKER_FLAG=0;
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
