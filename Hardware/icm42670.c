#include "icm42670.h"
#include "systick.h"
#include "delay.h"
#include "usart.h" 
#include "Invn/EmbUtils/RingBuffer.h"
/*
 * Print raw data or scaled data
 * 0 : print raw accel, gyro and temp data
 * 1 : print scaled accel, gyro and temp data in g, dps and degree Celsius
 */
#define SCALED_DATA_G_DPS 1


/* I2C number and slave address for INV device */
#if (SM_BOARD_REV == SM_REVB_OB)
#define INV_SPI_AP   INV_SPI_ONBOARD_REVB
#define ICM_I2C_ADDR 0x69
#else
/* SM_REVB_DB and SM_REVG have same SPI/I2C configuration */
#define ICM_I2C_ADDR 0x68
#endif

/*
 * ICM mounting matrix
 * Coefficients are coded as Q30 integer
 */
#if (SM_BOARD_REV == SM_REVB_DB) /* when DB or EVB are used */
static int32_t icm_mounting_matrix[9] = { 0, -(1 << 30), 0, (1 << 30), 0, 0, 0, 0, (1 << 30) };
#else /* For SmartMotion */
static int32_t icm_mounting_matrix[9] = { (1 << 30), 0, 0, 0, (1 << 30), 0, 0, 0, (1 << 30) };
#endif

static struct inv_imu_device icm_driver;

int ICM_IO_Init(struct inv_imu_serif *serif)
{
	switch (serif->serif_type) {
		case UI_SPI4: {
			break;
		}
		case UI_I2C:
			/* Set I2C clock is 400kHz by default */
			IIC_Init();
			break;
		default:
			return -1;
	}

	return 0;
}

/* --------------------------------------------------------------------------------------
 *  Static functions definition
 * -------------------------------------------------------------------------------------- */

static void apply_mounting_matrix(const int32_t matrix[9], int32_t raw[3])
{
	unsigned i;
	int64_t  data_q30[3];

	for (i = 0; i < 3; i++) {
		data_q30[i] = ((int64_t)matrix[3 * i + 0] * raw[0]);
		data_q30[i] += ((int64_t)matrix[3 * i + 1] * raw[1]);
		data_q30[i] += ((int64_t)matrix[3 * i + 2] * raw[2]);
	}
	raw[0] = (int32_t)(data_q30[0] >> 30);
	raw[1] = (int32_t)(data_q30[1] >> 30);
	raw[2] = (int32_t)(data_q30[2] >> 30);
}

int get_imu_data(void)
{
#if USE_FIFO
	return inv_imu_get_data_from_fifo(&icm_driver);
#else
	return inv_imu_get_data_from_registers(&icm_driver);
#endif
}


static void get_accel_and_gyr_fsr(uint16_t *accel_fsr_g, uint16_t *gyro_fsr_dps)
{
	ACCEL_CONFIG0_FS_SEL_t accel_fsr_bitfield;
	GYRO_CONFIG0_FS_SEL_t  gyro_fsr_bitfield;

	inv_imu_get_accel_fsr(&icm_driver, &accel_fsr_bitfield);
	switch (accel_fsr_bitfield) {
	case ACCEL_CONFIG0_FS_SEL_2g:
		*accel_fsr_g = 2;
		break;
	case ACCEL_CONFIG0_FS_SEL_4g:
		*accel_fsr_g = 4;
		break;
	case ACCEL_CONFIG0_FS_SEL_8g:
		*accel_fsr_g = 8;
		break;
	case ACCEL_CONFIG0_FS_SEL_16g:
		*accel_fsr_g = 16;
		break;
	default:
		*accel_fsr_g = -1;
	}

	inv_imu_get_gyro_fsr(&icm_driver, &gyro_fsr_bitfield);
	switch (gyro_fsr_bitfield) {
	case GYRO_CONFIG0_FS_SEL_250dps:
		*gyro_fsr_dps = 250;
		break;
	case GYRO_CONFIG0_FS_SEL_500dps:
		*gyro_fsr_dps = 500;
		break;
	case GYRO_CONFIG0_FS_SEL_1000dps:
		*gyro_fsr_dps = 1000;
		break;
	case GYRO_CONFIG0_FS_SEL_2000dps:
		*gyro_fsr_dps = 2000;
		break;
	default:
		*gyro_fsr_dps = -1;
	}
}

void imu_callback(inv_imu_sensor_event_t *event)
{
	int32_t  accel[3], gyro[3];
#if SCALED_DATA_G_DPS
	float    accel_g[3];
	float    gyro_dps[3];
	float    temp_degc;
	uint16_t accel_fsr_g, gyro_fsr_dps;
#endif
	apply_mounting_matrix(icm_mounting_matrix, accel);
	apply_mounting_matrix(icm_mounting_matrix, gyro);
	
	accel[0] = event->accel[0];
	accel[1] = event->accel[1];
	accel[2] = event->accel[2];

	gyro[0] = event->gyro[0];
	gyro[1] = event->gyro[1];
	gyro[2] = event->gyro[2];

	// Force sensor_mask so it gets displayed below
	event->sensor_mask |= (1 << INV_SENSOR_TEMPERATURE);
	event->sensor_mask |= (1 << INV_SENSOR_ACCEL);
	event->sensor_mask |= (1 << INV_SENSOR_GYRO);
	
		/*
	 * Convert raw data into scaled data in g and dps
	*/
	get_accel_and_gyr_fsr(&accel_fsr_g, &gyro_fsr_dps);
	accel_g[0]  = (float)(accel[0] * accel_fsr_g) / INT16_MAX;
	accel_g[1]  = (float)(accel[1] * accel_fsr_g) / INT16_MAX;
	accel_g[2]  = (float)(accel[2] * accel_fsr_g) / INT16_MAX;
	gyro_dps[0] = (float)(gyro[0] * gyro_fsr_dps) / INT16_MAX;
	gyro_dps[1] = (float)(gyro[1] * gyro_fsr_dps) / INT16_MAX;
	gyro_dps[2] = (float)(gyro[2] * gyro_fsr_dps) / INT16_MAX;
	if (USE_HIGH_RES_MODE || !USE_FIFO)
		temp_degc = 25 + ((float)event->temperature / 128);
	else
		temp_degc = 25 + ((float)event->temperature / 2);
	
	printf("\r\n temp:%f.\r\nacl:%f,%f,%f\r\ngyo:%f,%f,%f\r\n",\
					temp_degc,accel_g[0],accel_g[1],accel_g[2],gyro_dps[0],gyro_dps[1],gyro_dps[2]);
	/*
	 * Output raw data on UART link
	
	if (event->sensor_mask & (1 << INV_SENSOR_ACCEL) && event->sensor_mask & (1 << INV_SENSOR_GYRO))
		printf("acl:%d, %d, %d,temp: %d, gyro:%d, %d, %d", accel[0],\
		        accel[1], accel[2], event->temperature, gyro[0], gyro[1], gyro[2]);
	else if (event->sensor_mask & (1 << INV_SENSOR_GYRO))
		printf("NA, NA, NA, %d, %d, %d, %d", \
		        event->temperature, gyro[0], gyro[1], gyro[2]);
	else if (event->sensor_mask & (1 << INV_SENSOR_ACCEL))
		printf("%d, %d, %d, %d, NA, NA, NA", accel[0],
		        accel[1], accel[2], event->temperature);
	 */
}


int ICM_Init_Configure(struct inv_imu_serif *icm_serif)
{
	int     rc = 0;
	uint8_t who_am_i;

	/* Init device */
	rc = inv_imu_init(&icm_driver, icm_serif, imu_callback);
	if (rc != INV_ERROR_SUCCESS) {
		//INV_MSG(INV_MSG_LEVEL_ERROR, "Failed to initialize IMU!");
		return rc;
	}

	/* Check WHOAMI */
	rc = inv_imu_get_who_am_i(&icm_driver, &who_am_i);
	if (rc != INV_ERROR_SUCCESS) {
		//INV_MSG(INV_MSG_LEVEL_ERROR, "Failed to read whoami!");
		return rc;
	}

	if (who_am_i != ICM_WHOAMI) {
		//INV_MSG(INV_MSG_LEVEL_ERROR, "Bad WHOAMI value!");
		//INV_MSG(INV_MSG_LEVEL_ERROR, "Read 0x%02x, expected 0x%02x", who_am_i, ICM_WHOAMI);
		return INV_ERROR;
	}

//#if !USE_FIFO
//	RINGBUFFER_CLEAR(&timestamp_buffer);
//#endif

	if (!USE_FIFO)
		rc |= inv_imu_configure_fifo(&icm_driver, INV_IMU_FIFO_DISABLED);

	if (USE_HIGH_RES_MODE) {
		rc |= inv_imu_enable_high_resolution_fifo(&icm_driver);
	} else {
		rc |= inv_imu_set_accel_fsr(&icm_driver, ACCEL_CONFIG0_FS_SEL_4g);
		rc |= inv_imu_set_gyro_fsr(&icm_driver, GYRO_CONFIG0_FS_SEL_2000dps);
	}

	if (USE_LOW_NOISE_MODE) {
		rc |= inv_imu_set_accel_frequency(&icm_driver, ACCEL_CONFIG0_ODR_25_HZ);
		rc |= inv_imu_set_gyro_frequency(&icm_driver, GYRO_CONFIG0_ODR_25_HZ);
		rc |= inv_imu_enable_accel_low_noise_mode(&icm_driver);
	} else {
		rc |= inv_imu_set_accel_lp_avg(&icm_driver,ACCEL_CONFIG1_ACCEL_FILT_AVG_2);
		rc |= inv_imu_set_accel_frequency(&icm_driver, ACCEL_CONFIG0_ODR_25_HZ);
//		rc |= inv_imu_set_gyro_frequency(&icm_driver, GYRO_CONFIG0_ODR_25_HZ);
		rc |= inv_imu_enable_accel_low_power_mode(&icm_driver);
	}

//	rc |= inv_imu_enable_gyro_low_noise_mode(&icm_driver);

	if (!USE_FIFO)
		delay_ms(GYR_STARTUP_TIME_US/1000);
	return rc;
}

int ICM_IO_ReadData(struct inv_imu_serif *serif, u8 reg, u8 *rbuffer, u32 rlen)
{
	switch (serif->serif_type) {
	case UI_SPI4:
	case UI_I2C:
		while (ICM_Read_Len(ICM_I2C_ADDR, reg, rlen, rbuffer)) {
			delay_ms(32); // Loop in case of I2C timeout
		}
		return 0;
	default:
		return -1;
	}
}

int ICM_IO_WriteData(struct inv_imu_serif *serif, u8 reg, const u8 *wbuffer, u32 wlen)
{
	switch (serif->serif_type) {
	case UI_SPI4:
	case UI_I2C:
		while (ICM_Write_Len(ICM_I2C_ADDR, reg, wlen, wbuffer)) {
			delay_ms(32); // Loop in case of I2C timeout
		}
		return 0;
	default:
		return -1;
	}
}
//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
u8 ICM_Init(struct inv_imu_serif *icm_serif)
{ 
	int rc=0;
	icm_serif->context    = 0; /* no need */
	icm_serif->read_reg   = ICM_IO_ReadData;
	icm_serif->write_reg  = ICM_IO_WriteData;
	icm_serif->max_read   = 1024 * 32; /* maximum number of bytes allowed per serial read */
	icm_serif->max_write  = 1024 * 32; /* maximum number of bytes allowed per serial write */
	icm_serif->serif_type = UI_I2C;
	
	rc = ICM_IO_Init(icm_serif);
	if (rc != INV_ERROR_SUCCESS) {
		//INV_MSG(INV_MSG_LEVEL_ERROR, "Failed to read whoami!");
		return rc;
	}
	rc = ICM_Init_Configure(icm_serif);
	if (rc != INV_ERROR_SUCCESS) {
		//INV_MSG(INV_MSG_LEVEL_ERROR, "Failed to read whoami!");
		return rc;
	}

	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 ICM_Set_Gyro_Fsr(u8 fsr,u8 odr)
{
	return 0;
	//return ICM_Write_Byte(ICM_GYRO_CFG0,fsr<<4|odr);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 ICM_Set_Accel_Fsr(u8 fsr,u8 odr)
{
	return 0;
	//return ICM_Write_Byte(ICM_ACCEL_CFG0,fsr<<4|odr);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 ICM_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=180)data=1;
	else if(lpf>=121)data=2;
	else if(lpf>=73)data=3;
	else if(lpf>=53)data=4;
	else if(lpf>=34)data=5;
	else if(lpf>=25)data=6;
	else data=7; 
	
	return 0;
	//return ICM_Write_Byte(ICM_GYRO_CFG1,data)|ICM_Write_Byte(ICM_ACCEL_CFG1,data);//设置数字低通滤波器  
}



u8 ICM_Get_AllSensorData(ICM42670_SENSOR_DATA *sensor_data)
{
	return 0;
//	return ICM_Read_Len(ICM_ADDR,ICM_TEMP_OUTH_REG,14,(u8*)sensor_data);
}

void ICM_Test(void)
{
	ICM42670_SENSOR_DATA icm_data;
	ICM_Get_AllSensorData(&icm_data);
//	printf("icm_data temp:%d",temp);
}
//得到温度值
//返回值:温度值(扩大了100倍)
short ICM_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
		float temp;
	//	ICM_Read_Len(ICM_ADDR,ICM_TEMP_OUTH_REG,2,buf);
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=(float)raw/128+25;  
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 ICM_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	//res=ICM_Read_Len(ICM_ADDR,ICM_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 ICM_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	//res=ICM_Read_Len(ICM_ADDR,ICM_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


//IIC连续写
//addr:器件地址 
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
u8 ICM_Write_Len(u8 addr,u8 reg,u8 len,const u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//发送数据
		if(IIC_Wait_Ack())		//等待ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 ICM_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//产生一个停止条件 
	return 0;	
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 ICM_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((ICM_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack())	//等待应答
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答 
	IIC_Send_Byte(data);//发送数据
	if(IIC_Wait_Ack())	//等待ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 ICM_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((ICM_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_Wait_Ack();		//等待应答 
    IIC_Send_Byte(reg);	//写寄存器地址
    IIC_Wait_Ack();		//等待应答
    IIC_Start();
	IIC_Send_Byte((ICM_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack();		//等待应答 
	res=IIC_Read_Byte(0);//读取数据,发送nACK 
    IIC_Stop();			//产生一个停止条件 
	return res;		
}
