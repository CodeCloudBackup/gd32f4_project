#ifndef __MPU6050_H
#define __MPU6050_H
#include "myiic.h"   

//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
//#define MPU_PROD_ID_REG			0X0C	//prod id寄存器,在寄存器手册未提到
#define ICM_MCLK_READY				0x00  //inter clock ready
#define ICM_DEVICE_CONFIG     0x01  //SPI_WIRE,SPI_MODE
#define ICM_SIGNAL_PATH_RESET	0x02  //SOFT RESET,FIFO FLUSH
#define ICM_DRIVE_CFG1				0x03  //
#define ICM_DRIVE_CFG2				0x04  //I2C_SLEW_RATE,ALL_SLEW_RATE
#define ICM_DRIVE_CFG3				0x05	//SPI_SLEW_RATE
#define ICM_INT_CFG						0x06

#define ICM_TEMP_OUTH_REG			0X09	//温度值高八位寄存器
#define ICM_TEMP_OUTL_REG			0X0A	//温度值低8位寄存器
#define ICM_ACCEL_XOUTH_REG		0X0B	//加速度值,X轴高8位寄存器
#define ICM_ACCEL_XOUTL_REG		0X0C	//加速度值,X轴低8位寄存器
#define ICM_ACCEL_YOUTH_REG		0X0D	//加速度值,Y轴高8位寄存器
#define ICM_ACCEL_YOUTL_REG		0X0E	//加速度值,Y轴低8位寄存器
#define ICM_ACCEL_ZOUTH_REG		0X0F	//加速度值,Z轴高8位寄存器
#define ICM_ACCEL_ZOUTL_REG		0X10	//加速度值,Z轴低8位寄存器

#define ICM_GYRO_XOUTH_REG		0X11	//陀螺仪值,X轴高8位寄存器
#define ICM_GYRO_XOUTL_REG		0X12	//陀螺仪值,X轴低8位寄存器
#define ICM_GYRO_YOUTH_REG		0X13	//陀螺仪值,Y轴高8位寄存器
#define ICM_GYRO_YOUTL_REG		0X14	//陀螺仪值,Y轴低8位寄存器
#define ICM_GYRO_ZOUTH_REG		0X15	//陀螺仪值,Z轴高8位寄存器
#define ICM_GYRO_ZOUTL_REG		0X16	//陀螺仪值,Z轴低8位寄存器

#define ICM_TMST_FSYNCH				0x17
#define ICM_TMST_FSYNCL				0x18
#define ICM_APEX_DATA4				0x1D	//Lower byte of Freefall Duration
#define ICM_APEX_DATA5				0x1E	//Upper byte of Freefall Duration
#define ICM_PWR_MGMT0					0x1F	//LP CLOCK SELECT,GYRO_MODE,ACCEL_MODE
#define ICM_GYRO_CFG0					0X20	//陀螺仪配置寄存器,Full scale select,ODR select
#define ICM_ACCEL_CFG0				0X21	//加速度计配置寄存器,Full scale select,ODR select
#define ICM_TEMP_CFG0					0X22	//加速度计配置寄存器,Sets the bandwidth 
#define ICM_GYRO_CFG1					0X23	//陀螺仪配置寄存器,Selects GYRO UI low pass filter bandwidth
#define ICM_ACCEL_CFG1				0X24	//加速度计配置寄存器,Selects ACCEL UI low pass filter bandwidth

#define ICM_FIFO_CFG1					0x28  //FIFO mode control
#define ICM_FIFO_CFG2					0x29 	//FIFO_WM[7:0]
#define ICM_FIFO_CFG3					0x2A  //FIFO_WM[11:8]
#define ICM_INT_SOURCE0				0x2B
#define ICM_INT_SOURCE1				0x2C
#define ICM_INT_SOURCE3				0x2D
#define ICM_INT_SOURCE4				0x2E
#define ICM_FIFO_CNTH					0x3D	//FIFO计数寄存器高八位
#define ICM_FIFO_CNTL					0x3E	//FIFO计数寄存器低八位
#define ICM_FIFO_DATA					0x3F	//FIFO data port

#define	ICM_WHOAMI						0x75	//Reset value: 0x67,器件ID寄存器
#define ICM_BLK_SEL_W					0x79  //write bank mreg select 0x00 MREG1,0x28 MREG2,0x50 MREG3
#define ICM_MADDR_W  					0x7A	//write bank mreg addr
#define ICM_M_W								0x7B  //write bank mreg value
#define ICM_BLK_SEL_R					0x7C  //read bank mreg select 0x00 MREG1,0x28 MREG2,0x50 MREG3
#define ICM_MADDR_R           0x7D	//read bank mreg addr
#define ICM_M_R								0x7E	//read bank mreg value

// USER BANK MREG1
#define ICM_FIFO_CFG5					0x01 //FIFO ENABLE
#define ICM_FIFO_CFG6					0x02
#define ICM_FSYNC_CFG					0x03
#define ICM_ST_CONFIG					0x13 //self test config
#define ICM_SELFTEST_ENABLE		0x14
#define ICM_ACCEL_WOM_X_THR		0x4B
#define ICM_ACCEL_WOM_Y_THR		0x4C
#define ICM_ACCEL_WOM_Z_THR		0x4D
#define ICM_ACCEL_ST_STATUS   0x63
#define ICM_GYRO_ST_STATUS		0x64

// USER BANK MREG3
#define ICM_XA_ST_DATA				0x00
#define ICM_YA_ST_DATA				0x01
#define ICM_ZA_ST_DATA				0x02
#define ICM_XG_ST_DATA				0x03
#define ICM_YG_ST_DATA				0x04
#define ICM_ZG_ST_DATA				0x05
//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//如果接V3.3,则IIC地址为0X69(不包含最低位).
#define ICM_ADDR				0X68

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

u8 ICM_Init(void); 								//初始化MPU6050
u8 ICM_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);//IIC连续写
u8 ICM_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf); //IIC连续读 
u8 ICM_Write_Byte(u8 reg,u8 data);				//IIC写一个字节
u8 ICM_Read_Byte(u8 reg);						//IIC读一个字节

u8 ICM_Set_Gyro_Fsr(u8 fsr,u8 odr);
u8 ICM_Set_Accel_Fsr(u8 fsr,u8 odr);
u8 ICM_Set_LPF(u16 lpf);
u8 ICM_Set_Fifo(u8 sens);

u8 ICM_Get_AllSensorData(ICM42670_SENSOR_DATA *sensor_data);
short ICM_Get_Temperature(void);
u8 ICM_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 ICM_Get_Accelerometer(short *ax,short *ay,short *az);

#endif
