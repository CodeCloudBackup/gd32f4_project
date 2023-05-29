/*****************************************************************************
  �ļ�: mpu6050.c
  ����: Zhengyu https://gzwelink.taobao.com
  �汾: V1.0.0
  ʱ��: 20200401
	ƽ̨:MINI-F030C8T6

*******************************************************************************/

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "mpu6050.h"
#include "main.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: д���ݵ�MPU6050�Ĵ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: ��MPU6050�Ĵ�����ȡ����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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


void Motion_Interrupt(void)             //�˶��ж�
{
    MPU6050_WriteReg(MPU6050_RA_MOT_THR,30);              //�˶���ֵ�����Ҫ���������ȣ����԰�ֵ�ĵ�10����
    MPU6050_WriteReg(MPU6050_RA_MOT_DUR,0x14);            //���ʱ��20ms ��λ1ms 
}

/**
  * ��������: �жϳ�ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */ 
void MPU_INT_Init(void)
{

 
//�������塢�˶�����ֹ�ж� ��ѡһ  
//    Free_Fall_Interrupt();                          //���������ж�
    Motion_Interrupt();                               //�˶��ж�
//    Zero_Motion_Interrupt();                        //��ֹ�ж�
    MPU6050_WriteReg(MPU6050_RA_CONFIG,0x04);           //�����ⲿ���Ų�����DLPF���ֵ�ͨ�˲���
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,0x04);     //���ٶȴ��������̺͸�ͨ�˲�������
    MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG,0X1C);      //INT���ŵ͵�ƽƽʱ
    MPU6050_WriteReg(MPU6050_RA_INT_ENABLE,0x40);       //�ж�ʹ�ܼĴ���
}
/**
  * ��������: ��ʼ��MPU6050оƬ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */ 
void MPU6050_Init(void)
{
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ����ʣ�1KHz
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	        //��ͨ�˲��������ã���ֹƵ����1K��������5K
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x00);	  //���ü��ٶȴ�����������2Gģʽ�����Լ�
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	MPU_INT_Init();
	

}

/**
  * ��������: ��ȡMPU6050��ID
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */ 
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
	//	printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
	//	printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}

/**
  * ��������: ��ȡMPU6050�ļ��ٶ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
				accData[i]=acc[i]*2000/32768;//ת���ɵ�λmg
			}else
			{
				accData[i]=-(-acc[i]+1)*2000/32768;
			}		
		}
}

/**
  * ��������: ��ȡMPU6050�Ľ��ٶ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: ��ȡMPU6050�Ľ��ٶ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
				gyroData[i]=gyro[i]*2000/32768;//ת���ɵ�λmdps
			}else
			{
				gyroData[i]=-(-gyro[i]+1)*2000/32768;
			}
		}		
}

/**
  * ��������: ��ȡMPU6050��ԭʼ�¶�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */ 
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
	*tempData = (buf[0] << 8) | buf[1];
}

/**
  * ��������: ��ȡMPU6050���¶����ݣ�ת�������϶�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */ 
void MPU6050_ReturnTemp(short*Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
	temp3= (buf[0] << 8) | buf[1];
	*Temperature=(((double) (temp3 + 13200)) / 280)-13;
}

 


