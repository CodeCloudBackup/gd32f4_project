#include "bh170.h"

#define I2C1_SLAVE_ADDRESS7    0x47

//д��1�ֽ����ݵ�BH170,�豸��ַ0x46
void BH170_WriteReg(uint8_t reg_add)
{

	  /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus*/
    i2c_master_addressing(I2C0, I2C1_SLAVE_ADDRESS7, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set*/
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
 

		/* send a data byte */
		i2c_data_transmit(I2C0,reg_add);
		/* wait until the transmission data register is empty*/
		while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
		 i2c_stop_on_bus(I2C0);
		 /* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0)&0x0200);
  
}

//��ȡBH170����,�豸��ַ0x46
void BH170_ReadData(unsigned char*Read)
{

	
	 /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, I2C1_SLAVE_ADDRESS7, I2C_RECEIVER);
    /* disable ACK before clearing ADDSEND bit */
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    /* Wait until the last data byte is received into the shift register */
    /* wait until the RBNE bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));

    /* read a data from I2C_DATA */
    Read[0] = i2c_data_receive(I2C0);

    /* read a data from I2C_DATA */
    Read[1] = i2c_data_receive(I2C0);
    /* send a stop condition */
    i2c_stop_on_bus(I2C0);

}

void BH170_Init(void)
{
	BH170_WriteReg(0x01);// power on
	BH170_WriteReg(0x10);//H- resolution mode
}

float Get_LightData()
{
		u8 DataBuff[2];//�洢����
		float LightData;//����ǿ�ȣ���С��λ����λlx��
		//u32 LightData_Hex;//����ǿ�ȣ���������λlx��
		BH170_ReadData(DataBuff);//��ȡ����
		LightData=((DataBuff[0]<<8)+DataBuff[1])/1.2f;//����ת���ɹ�ǿ�ȣ���λlx
   // LightData_Hex=LightData;//floatת��������
		return LightData;
}
