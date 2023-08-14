#include "icm42670.h"
#include "systick.h"
#include "delay.h"
#include "usart.h" 


//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
u8 ICM_Init(void)
{ 
	u8 res;
	IIC_Init();//��ʼ��IIC����
	
	ICM_Write_Byte(ICM_PWR_MGMT0,0X00);	//��λMPU6050
    delay_ms(100);
	res = ICM_Read_Byte(ICM_WHOAMI);
	if(res!=ICM_ADDR)//����ID��ȷ
	{
		return res;
	}
	ICM_Write_Byte(ICM_FIFO_CFG1,0X00);	//stream to FIFO
	res = ICM_Read_Byte(ICM_INT_SOURCE0);
	ICM_Write_Byte(ICM_INT_SOURCE0,0x00);	//�ر������ж�
	ICM_Write_Byte(ICM_FIFO_CFG2,0X00);
	ICM_Write_Byte(ICM_FIFO_CFG3,0X02);
	ICM_Write_Byte(ICM_INT_SOURCE0,res);
	ICM_Write_Byte(ICM_FIFO_CFG1,0X33);	//stream to FIFO
	
	ICM_Write_Byte(ICM_INT_CFG,0X36);	//stream to FIFO
	res = ICM_Read_Byte(ICM_INT_SOURCE0);
	res |= 1<<2;
	ICM_Write_Byte(ICM_INT_SOURCE0,res);	//�ر������ж�
	
	ICM_Set_Gyro_Fsr(3,10);					//�����Ǵ�����,��2000dps
	ICM_Set_Accel_Fsr(0,10);					//���ٶȴ�����,��2g
	res = ICM_Read_Byte(ICM_PWR_MGMT0);
	res |= (3<<2);	//GYRO_MODE,00:off, 01:Standby, 10:Reserved, 11:Low Noise
	res |= (3);			//ACCEL_MODE,00:off,01:off,10,Low Power,11:Low Noise
	ICM_Write_Byte(ICM_PWR_MGMT0, res);
	delay_ms(1);
	return 0;
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 ICM_Set_Gyro_Fsr(u8 fsr,u8 odr)
{
	return ICM_Write_Byte(ICM_GYRO_CFG0,fsr<<4|odr);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 ICM_Set_Accel_Fsr(u8 fsr,u8 odr)
{
	return ICM_Write_Byte(ICM_ACCEL_CFG0,fsr<<4|odr);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
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
	return ICM_Write_Byte(ICM_GYRO_CFG1,data)|ICM_Write_Byte(ICM_ACCEL_CFG1,data);//�������ֵ�ͨ�˲���  
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
//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
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
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
//IIC����д
//addr:������ַ 
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
u8 ICM_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//��������
		if(IIC_Wait_Ack())		//�ȴ�ACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
u8 ICM_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//������,����nACK 
		else *buf=IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//����һ��ֹͣ���� 
	return 0;	
}
//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
u8 ICM_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((ICM_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	IIC_Send_Byte(data);//��������
	if(IIC_Wait_Ack())	//�ȴ�ACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 ICM_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((ICM_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack();		//�ȴ�Ӧ��
    IIC_Start();
	IIC_Send_Byte((ICM_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(0);//��ȡ����,����nACK 
    IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}
