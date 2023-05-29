/*****************************************************************************
  文件: mpu6050.c
  作者: Zhengyu https://gzwelink.taobao.com
  版本: V1.0.0
  时间: 20200401
	平台:MINI-F030C8T6

*******************************************************************************/

/* 包含头文件 ----------------------------------------------------------------*/
#include "mpu6050.h"
#include "main.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 写数据到MPU6050寄存器
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	/* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, MPU6050_SLAVE_ADDRESS, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get(I2C0, I2C_FLAG_TBE));
    
    /* send the EEPROM's internal address to write to : only one byte address */
    i2c_data_transmit(I2C0, reg_add);
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
    
    /* send the byte to be written */
    i2c_data_transmit(I2C0, reg_dat); 
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
}

/**
  * 函数功能: 从MPU6050寄存器读取数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_ReadData(uint8_t reg_add,unsigned char*Read,uint8_t num)
{
	 /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    if(2 == num){
        i2c_ackpos_config(I2C0,I2C_ACKPOS_NEXT);
    }
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, MPU6050_SLAVE_ADDRESS, I2C_TRANSMITTER);
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
    
    /* wait until the transmit data buffer is empty */
    while(SET != i2c_flag_get( I2C0 , I2C_FLAG_TBE));

    /* enable I2C0*/
    i2c_enable(I2C0);
    
    /* send the EEPROM's internal address to write to */
    i2c_data_transmit(I2C0, reg_add);  
    
    /* wait until BTC bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
    
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, MPU6050_SLAVE_ADDRESS, I2C_RECEIVER);

    if(num < 3){
        /* disable acknowledge */
        i2c_ack_config(I2C0,I2C_ACK_DISABLE);
    }
    
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    
    /* clear the ADDSEND bit */
    i2c_flag_clear(I2C0,I2C_FLAG_ADDSEND);
    
    if(1 == num){
        /* send a stop condition to I2C bus */
        i2c_stop_on_bus(I2C0);
    }
    
    /* while there is data to be read */
    while(num){
        if(3 == num){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));

            /* disable acknowledge */
            i2c_ack_config(I2C0,I2C_ACK_DISABLE);
        }
        if(2 == num){
            /* wait until BTC bit is set */
            while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
            
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2C0);
        }
        
        /* wait until the RBNE bit is set and clear it */
        if(i2c_flag_get(I2C0, I2C_FLAG_RBNE)){
            /* read a byte from the EEPROM */
            *Read = i2c_data_receive(I2C0);
            
            /* point to the next location where the byte read will be saved */
            Read++; 
            
            /* decrement the read bytes counter */
            num--;
        } 
    }
    
    /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
    
    /* enable acknowledge */
    i2c_ack_config(I2C0,I2C_ACK_ENABLE);

    i2c_ackpos_config(I2C0,I2C_ACKPOS_CURRENT);
}


void Motion_Interrupt(void)             //运动中断
{
    MPU6050_WriteReg(MPU6050_RA_MOT_THR,30);              //运动阈值，如果要增加灵敏度，可以把值改到10以下
    MPU6050_WriteReg(MPU6050_RA_MOT_DUR,0x14);            //检测时间20ms 单位1ms 
}

/**
  * 函数功能: 中断初始化设置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU_INT_Init(void)
{

 
//自由落体、运动、静止中断 三选一  
//    Free_Fall_Interrupt();                          //自由落体中断
    Motion_Interrupt();                               //运动中断
//    Zero_Motion_Interrupt();                        //静止中断
    MPU6050_WriteReg(MPU6050_RA_CONFIG,0x04);           //配置外部引脚采样和DLPF数字低通滤波器
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,0x04);     //加速度传感器量程和高通滤波器配置
    MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG,0X1C);      //INT引脚低电平平时
    MPU6050_WriteReg(MPU6050_RA_INT_ENABLE,0x40);       //中断使能寄存器
}
/**
  * 函数功能: 初始化MPU6050芯片
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_Init(void)
{
  int i=0,j=0;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率，1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //低通滤波器的设置，截止频率是1K，带宽是5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	  //配置加速度传感器工作在2G模式，不自检
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	MPU_INT_Init();
	

}

/**
  * 函数功能: 读取MPU6050的ID
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	if(Re != 0x68)
	{
	//	printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
	//	printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}

/**
  * 函数功能: 读取MPU6050的加速度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
		MPU6050_ReadData(MPU6050_ACC_OUT, &buf[0], 6);

	
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

void MPU6050ReadAcc2Real(long *accData)
{
    uint8_t buf[6];
		short acc[3];
		MPU6050_ReadData(MPU6050_ACC_OUT, &buf[0], 6);
		
    acc[0] = (buf[0] << 8) | buf[1];
    acc[1] = (buf[2] << 8) | buf[3];
    acc[2] = (buf[4] << 8) | buf[5];
	
		for(uint32_t i=0;i<3;i++)
		{
			if(acc[i]>=0)
			{
				accData[i]=acc[i]*2000/32768;//转换成单位mg
			}else
			{
				accData[i]=-(-acc[i]+1)*2000/32768;
			}		
		}
}

/**
  * 函数功能: 读取MPU6050的角速度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * 函数功能: 读取MPU6050的角速度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadGyro2Real(long *gyroData)
{
    uint8_t buf[6];
		short gyro[3];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyro[0] = (buf[0] << 8) | buf[1];
    gyro[1] = (buf[2] << 8) | buf[3];
    gyro[2] = (buf[4] << 8) | buf[5];
		
		for(uint32_t i=0;i<3;i++)
		{
			if(gyro[i]>=0)
			{
				gyroData[i]=gyro[i]*2000/32768;//转换成单位mdps
			}else
			{
				gyroData[i]=-(-gyro[i]+1)*2000/32768;
			}
		}		
}

/**
  * 函数功能: 读取MPU6050的原始温度数据
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
	*tempData = (buf[0] << 8) | buf[1];
}

/**
  * 函数功能: 读取MPU6050的温度数据，转化成摄氏度
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */ 
void MPU6050_ReturnTemp(short*Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
	temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}

 


