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
//	PROGRAM_RESERT_FLAG=0;
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
		StructInit();
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

extern u8 *g_rPublishbuf;
 
u8* g_httpContent=NULL;
u32 g_contLen=0;
extern u8* g_netData;
extern char* g_cookie;
void HTTP_Data_Program(void)
{
	u8 i=0;
	if (g_sHttpCmdSta.sta_equip_ident == 2)
	{
		char *ptr1=NULL, *ptr2=NULL;
		u8 len=0;
		printf("\r\nHttp Response: equip ident.\r\n");
		ptr1=strstr((const char *)g_netData, "Set-Cookie: session=");
		if(ptr1)
		ptr2=strstr((const char *)ptr1, ";");
		if(ptr2)
		{	
			len = ptr2 - ptr1 - 20;
			if(g_cookie == NULL)
				g_cookie=mymalloc(SRAMIN, len+1);
			memcpy(g_cookie, ptr1+20, len);
			g_cookie[len]='\0';
			printf("Cookie:%s\r\n",g_cookie);
		}
		
		g_sHttpCmdSta.sta_equip_ident = 0;
	}
	else if(g_sHttpCmdSta.sta_download_bin == 2)
	{
		printf("\r\nHttp Response: download bin.\r\n");
		char *ptr1=NULL, *ptr2=NULL, *ptr3=NULL;
		u8 *content;
		u32 cont_len=0;
		ptr1 = strstr((char*)g_netData, "Content-Length: ");	
		if(ptr1 != NULL)
		{
			ptr2 = strstr(ptr1, "\r\n");
			if(ptr2 != NULL)
			{
				char str_len[8] = {0,};
				memcpy(str_len, ptr1 + strlen("Content-Length: "), ptr2 - ptr1 - strlen("Content-Length: "));
				cont_len = atoi(str_len);
//				if(content == NULL)
//					content=mymalloc(SRAMIN, cont_len+1);
//				ptr3 = strstr(ptr2, "\r\n\r\n");
//				memcpy(content, ptr3+4, cont_len);
//				content[cont_len]='\0';
			}	
		}
		printf("conent_len[%d]",cont_len);
		//printf("first[%c],end[%c],conent_len[%d]",content[0],content[cont_len-1],cont_len);
		if(content!=NULL)
		{
			myfree(SRAMIN,content);
			content=NULL;
		}
		PROGRAM_APPFLASH_FLAG=1;
		g_sHttpCmdSta.sta_download_bin = 0;
	}
	else if (g_sHttpCmdSta.sta_upload_photo==2)
	{
		printf("\r\nHttp Response: upload photo.\r\n");
		g_sHttpCmdSta.sta_upload_photo = 0;
		
	}
	else if (g_sHttpCmdSta.sta_upload_logfile == 2)
	{
		printf("\r\nHttp Response: upload logfile.\r\n");
		g_sHttpCmdSta.sta_upload_logfile = 0;
	}
	
	i+=g_sHttpCmdSta.sta_equip_ident;
	i+=g_sHttpCmdSta.sta_download_bin;
	i+=g_sHttpCmdSta.sta_upload_photo;
	i+=g_sHttpCmdSta.sta_upload_logfile;
	if(!i)
	{
		if(g_netData != NULL)
		{
			myfree(SRAMIN, g_netData);
			g_netData=NULL;
		}
		g_sHttpCmdSta.sta_cmd=0;
	}
}

static int g_resetType = -1;

void MQTT_Data_Program(void)
{
	cJSON *json=NULL;
	if(!hm609a_mqtt_conn_flag)return;
	if(MQTT_FLAG_RECEIVE)
	{
		json=cJSON_Parse((char*)g_rPublishbuf);
		if(!json)
		{
			printf("Error before: []\n");
			return;
		}
	}		
	
	if(MQTT_FLAG_PHOTO){
		MQTT_FLAG_PHOTO=0;
		printf("\r\nServer command: take a photo.\r\n");
		PhotoJsonParse(json);
		//PROGRAM_TAKE_PHOTO_FLAG=1;
	}
	if(MQTT_FLAG_PHOTO_THIRD){
		MQTT_FLAG_PHOTO_THIRD=0;
		printf("\r\nServer command: third take a photo.\r\n");
	}
	if(MQTT_FLAG_DELY_OPEN){
		MQTT_FLAG_DELY_OPEN=0;
		printf("\r\nServer command: open deliver.\r\n");
		DelyJsonParse(json);
		PROGRAM_OPEN_DELY_FLAG=1;
	}
	if(MQTT_FLAG_DELY_CLOSE){
		MQTT_FLAG_DELY_CLOSE=0;
		printf("\r\nServer command: open deliver.\r\n");
		DelyJsonParse(json);
		if(g_sDelyInfo.type == 0)
		{
			PROGRAM_CLOSE_DELY_FLAG=1;
		}
		
	}
	if(MQTT_FLAG_LOCK_OPEN){
		u8 type = 0;
		type = LockJsonParse(json);
		printf("\r\nServer command: open lock..type:%d\r\n",type);
		MQTT_FLAG_LOCK_OPEN=0;
	}
	if(MQTT_FLAG_LOCK_CLOSE){
		u8 type = 0;
		type = LockJsonParse(json);
		printf("\r\nServer command: close lock.type:%d\r\n",type);
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
		MQTT_FLAG_CFG_UPLOAD=0;
		printf("\r\nServer command: upload config.\r\n");
		MQTT_FLAG_UP_PARAM_RES=1;
	}
	if(MQTT_FLAG_RESTART){
		MQTT_FLAG_RESTART=0;
		g_resetType = ResetJsonParse(json);
		printf("\r\nServer command: restart device type[%d].\r\n",g_resetType);
		
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
	
	g_sDeviceSta->power_voltage=Get_InVolt_Adc_Val();
	g_sDeviceSta->area_vacancy=g_sDeviceSta->power_voltage/BATTERY_VLOT;
	g_sDeviceSta->power_remain=g_sDeviceSta->area_vacancy;
	g_sDeviceSta->csq=0;
	g_sDeviceSta->lng=0.0;
	g_sDeviceSta->lat=0.0;
	memcpy(g_sDeviceSta->cameSerialCode,camCode,sizeof(camCode));
	memcpy(g_sDeviceSta->barCode,g_barCode,strlen(g_barCode));
	
	memcpy(g_sDeviceSta->simCode,simCode,sizeof(simCode));
	g_sDeviceSta->temp=Get_TempSensor_Adc_Val();
	printf("volt:%d, temp:%.2f",g_sDeviceSta->power_voltage, (float)g_sDeviceSta->temp);
	memcpy(g_sDeviceSta->acc, accel_g, sizeof(accel_g));
	memcpy(g_sDeviceSta->gyro, gyro_dps, sizeof(gyro_dps));
	g_sDeviceSta->temp=temp_degc;
	printf("\r\n temp:%.2f.\r\nacl:%.2f,%.2f,%.2f\r\ngyo:%.2f,%.2f,%.2f\r\n",\
					g_sDeviceSta->temp,g_sDeviceSta->acc[0],g_sDeviceSta->acc[1],g_sDeviceSta->acc[2],\
					g_sDeviceSta->gyro[0],g_sDeviceSta->gyro[1],g_sDeviceSta->gyro[2]);
}


void Device_Program(void)
{
	if(PROGRAM_TAKE_PHOTO_FLAG)
	{
		printf("Get jpg photo\r\n");
		if(!OV2640_Jpg_Photo())
		{
			g_sHttpCmdSta.sta_upload_photo=2;
			PROGRAM_TAKE_PHOTO_FLAG=0;
		}
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
	// 系统重启
	if(g_resetType == 1)
	{
		__set_FAULTMASK(1);
		NVIC_SystemReset();
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
	DAC1_Test( 36,4096 );
	// usart2_test();
}
