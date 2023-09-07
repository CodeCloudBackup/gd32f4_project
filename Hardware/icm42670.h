#ifndef __MPU6050_H
#define __MPU6050_H
#include "myiic.h" 
#include "inv_imu_regmap.h"
#include "inv_imu_transport.h"
#include "inv_imu_driver.h"
#include "inv_imu_defs.h"
/** @brief define all supported board revisions types*/
#define SM_REVB_DB 0
#define SM_REVB_OB 1
#define SM_REVG    2
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define ICM_ADDR				0X68

/* 
 * Select communication link between SmartMotion and IMU 
 */
//#define SERIF_TYPE UI_SPI4
#define SERIF_TYPE   UI_I2C

/*
 * Set power mode flag
 * Set this flag to run example in low-noise mode.
 * Reset this flag to run example in low-power mode.
 * Note: low-noise mode is not available with sensor data frequencies less than 12.5Hz.
 */
#define USE_LOW_NOISE_MODE 0

/*
 * Select Fifo resolution Mode (default is low resolution mode)
 * Low resolution mode: 16 bits data format
 * High resolution mode: 20 bits data format
 * Warning: Enabling High Res mode will force FSR to 16g and 2000dps
 */
#define USE_HIGH_RES_MODE 0

/*
 * Select to use FIFO or to read data from registers
 */
#define USE_FIFO 0


typedef struct
{
	u16 x;
	u16 y;
	u16 z;
}ICM42670_ACCEL_DATA;

typedef struct
{
	u16 x;
	u16 y;
	u16 z;
}ICM42670_GYRO_DATA;

typedef struct 
{
	u16 temp;
	ICM42670_ACCEL_DATA sAccelData;
	ICM42670_GYRO_DATA sGyroData;
	
}ICM42670_SENSOR_DATA;


extern float accel_g[3];
extern float gyro_dps[3];
extern float temp_degc;

u8 ICM_Init(struct inv_imu_serif *icm_serif); 	//初始化MPU6050
u8 ICM_Write_Len(u8 addr,u8 reg,u8 len,const u8 *buf);//IIC连续写
u8 ICM_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf); //IIC连续读 
u8 ICM_Write_Byte(u8 reg,u8 data);				//IIC写一个字节
u8 ICM_Read_Byte(u8 reg);						//IIC读一个字节

/**
 * \brief This function is the custom handling packet function.
 * \param[in] event structure containing sensor data from one packet
 */
void imu_callback(inv_imu_sensor_event_t *event);
int ICM_Init_Configure(struct inv_imu_serif *icm_serif);
int ICM_IO_Init(struct inv_imu_serif *serif);
int ICM_IO_ReadData(struct inv_imu_serif *serif, u8 reg, u8 *rbuffer, u32 rlen);
int ICM_IO_WriteData(struct inv_imu_serif *serif, u8 reg, const u8 *wbuffer, u32 wlen);
u8 ICM_Init(struct inv_imu_serif *icm_serif);
int get_imu_data(void);
#endif
