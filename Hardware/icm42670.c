#include "icm42670.h"
#include "systick.h"
#include "delay.h"
#include "usart.h" 


//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
u8 ICM_Init(void)
{ 
	u8 res;
	IIC_Init();//初始化IIC总线
	
	ICM_Write_Byte(ICM_PWR_MGMT0,0X00);	//复位MPU6050
    delay_ms(100);
	res = ICM_Read_Byte(ICM_WHOAMI);
	if(res!=ICM_ADDR)//器件ID正确
	{
		return res;
	}
	ICM_Write_Byte(ICM_FIFO_CFG1,0X00);	//stream to FIFO
	res = ICM_Read_Byte(ICM_INT_SOURCE0);
	ICM_Write_Byte(ICM_INT_SOURCE0,0x00);	//关闭所有中断
	ICM_Write_Byte(ICM_FIFO_CFG2,0X00);
	ICM_Write_Byte(ICM_FIFO_CFG3,0X02);
	ICM_Write_Byte(ICM_INT_SOURCE0,res);
	ICM_Write_Byte(ICM_FIFO_CFG1,0X33);	//stream to FIFO
	
	ICM_Write_Byte(ICM_INT_CFG,0X36);	//stream to FIFO
	res = ICM_Read_Byte(ICM_INT_SOURCE0);
	res |= 1<<2;
	ICM_Write_Byte(ICM_INT_SOURCE0,res);	//关闭所有中断
	
	ICM_Set_Gyro_Fsr(3,10);					//陀螺仪传感器,±2000dps
	ICM_Set_Accel_Fsr(0,10);					//加速度传感器,±2g
	res = ICM_Read_Byte(ICM_PWR_MGMT0);
	res |= (3<<2);	//GYRO_MODE,00:off, 01:Standby, 10:Reserved, 11:Low Noise
	res |= (3);			//ACCEL_MODE,00:off,01:off,10,Low Power,11:Low Noise
	ICM_Write_Byte(ICM_PWR_MGMT0, res);
	delay_ms(1);
	return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 ICM_Set_Gyro_Fsr(u8 fsr,u8 odr)
{
	return ICM_Write_Byte(ICM_GYRO_CFG0,fsr<<4|odr);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 ICM_Set_Accel_Fsr(u8 fsr,u8 odr)
{
	return ICM_Write_Byte(ICM_ACCEL_CFG0,fsr<<4|odr);//设置加速度传感器满量程范围  
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
	return ICM_Write_Byte(ICM_GYRO_CFG1,data)|ICM_Write_Byte(ICM_ACCEL_CFG1,data);//设置数字低通滤波器  
}



u8 ICM_Get_AllSensorData(ICM42670_SENSOR_DATA *sensor_data)
{
	return ICM_Read_Len(ICM_ADDR,ICM_TEMP_OUTH_REG,14,(u8*)sensor_data);
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
		ICM_Read_Len(ICM_ADDR,ICM_TEMP_OUTH_REG,2,buf);
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
	res=ICM_Read_Len(ICM_ADDR,ICM_GYRO_XOUTH_REG,6,buf);
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
	res=ICM_Read_Len(ICM_ADDR,ICM_ACCEL_XOUTH_REG,6,buf);
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
u8 ICM_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
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
