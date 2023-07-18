#include "nand_flash.h"

__align(4) u8 g_DataTmpBuffer[4096] = {0};
#define SectorBuf  g_DataTmpBuffer

#define FLASH_CS_0()			{gpio_bit_write(GPIOD, GPIO_PIN_0,RESET);delay_ms(10);}
#define FLASH_CS_1() 			{gpio_bit_write(GPIOD, GPIO_PIN_0,SET);delay_ms(10);}

__align(4) u16 g_WriteReadcnt = 0;
__align(4) u8 g_WriteData[0x100] = {0};
__align(4) u16 g_Count = 0;
__align(4) u8 g_ucPos = 0;


void Flash_RD_Test(u8 *ReadBuff)
{
	static u8 state = 0;
	u8 WriteBuff[10]={0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};//��д�洢
	u32 FlashJedecid,FlashDeviceid;//FLASH ID����
	switch(state)
	{
		case 0:
			FlashJedecid = spi_flash_read_id();//��ȡJedecid
			FlashDeviceid=SFLASH_ReadID();//��ȡDevice ID
			Flash_ReadSomeBytes(ReadBuff,0,8);//��FLASH 0��ַ��ȡ8�ֽ����ݷ���ReadBuff����
			Flash_WriteSR(0x42);//�������
			state++;
			break;
		case 1:
			Flash_ReadSR();//��״̬�Ĵ���
			Flash_WriteSomeBytes(WriteBuff,0,8);//��WriteBuff�����е�����д��FLASH 0��ַ
			state++;
			break;
		case 2:
			Flash_ReadSomeBytes(ReadBuff,0,8);//��FLASH 0��ַ��ȡ8�ֽ����ݷ���ReadBuff����
			state++;
			break;
		default:
			state = 0;
			break;
	}
	
}

void Flash_TestDataInit(void)
{
	u8 usMinData = 0;
	u8 usMaxData = 0xFF;
	u16 usCnt = 0;

    for(usCnt = 0; usCnt < 0x100 ; usCnt++)
	{
		if(g_WriteReadcnt % 2)
		{
			g_WriteData[usCnt] = usMinData;
			usMinData++;		
		}
		else
		{
			g_WriteData[usCnt] = usMaxData;
			usMaxData--;
		}	
		
	}
}
u8 spi_master_send_recv_byte(u8 spi_byte)
{		
	u8 ByteSend,ByteRecv;
	ByteSend=spi_byte;

 while(RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_TBE));
	spi_i2s_data_transmit(SPI2,ByteSend);
	while(RESET == spi_i2s_flag_get(SPI2, SPI_FLAG_RBNE));
  ByteRecv=spi_i2s_data_receive(SPI2);
	return ByteRecv;

}

void spi_master_recv_some_bytes( u8 *pbdata, u16 recv_length)
{
	u8 *temp_data = pbdata;

	while (recv_length--)
	{
		*temp_data++ = spi_master_send_recv_byte(0xFF);	//���� 0xff Ϊ���豸�ṩʱ��
	}
	
}


u32 spi_flash_read_id(void)
{
	u32 ulJedId = 0;
	u8 recv_buff[5] = {0};
	
	 FLASH_CS_0();

	spi_master_send_recv_byte(FLASH_READ_JEDEC_ID);	//9fh
	 
	spi_master_recv_some_bytes(recv_buff, sizeof(recv_buff));
	
	ulJedId = (recv_buff[0] <<16) | (recv_buff[1] <<8) | (recv_buff[2]);

	 FLASH_CS_1();
	
	return ulJedId;
}

u16 SFLASH_ReadID(void)
{
  u16 ID = 0;
   FLASH_CS_0();                              //ʹ������

	spi_master_send_recv_byte(0x90);//���Ͷ�ȡID����	    
	spi_master_send_recv_byte(0x00); 	    
	spi_master_send_recv_byte(0x00); 	    
	spi_master_send_recv_byte(0x00); 

	ID |= spi_master_send_recv_byte(0xFF)<<8;              //��ȡID
	ID |= spi_master_send_recv_byte(0xFF);
  
   FLASH_CS_1();                             //ʧ������
	
  return ID;
}

void Flash_WriteDisable(void)
{
	u8 command = FLASH_WRITE_DISABLE_CMD;
	FLASH_CS_0();  
	spi_master_send_recv_byte(command);
	FLASH_CS_1(); 
}


void Flash_WriteEnable(void)
{
	u8 command = FLASH_WRITE_ENABLE_CMD;

	FLASH_CS_0();	
	spi_master_send_recv_byte(command);//����дʹ��
	FLASH_CS_1();
}


u8 Flash_ReadSR(void)
{
	u8 ucTmpVal = 0;
	u8 command = FLASH_READ_SR_CMD;

	FLASH_CS_0();	
	
	spi_master_send_recv_byte(command);	//05h
	spi_master_recv_some_bytes(&ucTmpVal,1);
	
	FLASH_CS_1();
	
	return ucTmpVal;
}
void Flash_WaitNobusy(void)
{
	
	while(((Flash_ReadSR()) & 0x01)==0x01);	//�ȴ�BUSYλ���
}
void Flash_WriteSR(u8 _ucByte)
{
	u8 command = FLASH_WRITE_SR_CMD;

	Flash_WriteEnable();	
	Flash_WaitNobusy();

	FLASH_CS_0();	
	spi_master_send_recv_byte(command);	//01h
	spi_master_send_recv_byte(_ucByte);	//д��һ���ֽ�
  FLASH_CS_1();
}

void Flash_ErasePage(u32 _ulPageAddr)
{
	_ulPageAddr *= 256;
	
	Flash_WriteEnable();
	Flash_WaitNobusy();
	
	FLASH_CS_0();
	spi_master_send_recv_byte(FLASH_ERASE_PAGE);//ҳ����ָ��
	spi_master_send_recv_byte((u8)(_ulPageAddr>>16));	//д��24λ��ַ
	spi_master_send_recv_byte((u8)(_ulPageAddr>>8));
	spi_master_send_recv_byte((u8)(_ulPageAddr>>0));
	FLASH_CS_1();
	Flash_WaitNobusy();	//�ȴ�д�����
}


void Flash_EraseSector(u32 _ulSectorAddr)
{
	u8 command = FLASH_ERASE_SECTOR;
	_ulSectorAddr *= 4096;	//1������ 4 KBytes
	Flash_WriteEnable();
	Flash_WaitNobusy();
	FLASH_CS_0();
	
	spi_master_send_recv_byte(command);
	spi_master_send_recv_byte((u8)(_ulSectorAddr >> 16));
	spi_master_send_recv_byte((u8)(_ulSectorAddr >> 8));
	spi_master_send_recv_byte((u8)(_ulSectorAddr));
	FLASH_CS_1();
	
	Flash_WaitNobusy();	//�ȴ�д�����
}


void Flash_EraseBlock(u32 _ulBlockAddr)
{
	u8 command = FLASH_ERASE_BLOCK;
	_ulBlockAddr *= 65536;	//���ַ,һ��64K
	
	Flash_WriteEnable();
	Flash_WaitNobusy();

	FLASH_CS_0();
	spi_master_send_recv_byte(command);
	spi_master_send_recv_byte(_ulBlockAddr>>16);
	spi_master_send_recv_byte(_ulBlockAddr>>8);
	spi_master_send_recv_byte(_ulBlockAddr>>0);
	FLASH_CS_1();

	Flash_WaitNobusy();	//�ȴ�д�����
}


void Flash_EraseChip(void)
{
	u8 command = FLASH_ERASE_CHIP;

	Flash_WriteEnable();	//flashоƬдʹ��
	Flash_WaitNobusy();	//�ȴ�д�������
	
	FLASH_CS_0();
	spi_master_send_recv_byte(command);
	FLASH_CS_1();
	Flash_WaitNobusy();	//�ȴ�д�����
}
void Flash_ReadSomeBytes(u8 *ucpBuffer, u32 _ulReadAddr, u16 _usNByte)
{
	u8 command = FLASH_READ_DATA;
	u8 temp_buff[3] = {0};

	temp_buff[0] = (u8)(_ulReadAddr >> 16);
	temp_buff[1] = (u8)(_ulReadAddr >> 8);
	temp_buff[2] = (u8)(_ulReadAddr >> 0);

	FLASH_CS_0();
	
	spi_master_send_recv_byte(command);
	spi_master_send_recv_byte(temp_buff[0]);
	spi_master_send_recv_byte(temp_buff[1]);
	spi_master_send_recv_byte(temp_buff[2]);

	spi_master_recv_some_bytes(ucpBuffer, _usNByte);
	
		FLASH_CS_1();
}
void Flash_WritePage(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte)
{
	

	Flash_WriteEnable();	//дʹ��
	Flash_WaitNobusy();	//�ȴ�д�����
	
  FLASH_CS_0();
	
	spi_master_send_recv_byte(FLASH_WRITE_PAGE);	//02h
	spi_master_send_recv_byte(_ulWriteAddr>>16);	//д��24λ��ַ
	spi_master_send_recv_byte(_ulWriteAddr>>8);
	spi_master_send_recv_byte(_ulWriteAddr>>0);
	while(_usNByte--)
	{
		spi_master_send_recv_byte(*ucpBuffer);	//SPI д�뵥���ֽ�
		ucpBuffer++;
	}
	FLASH_CS_1();
	Flash_WaitNobusy();	//�ȴ�д�����
}

//����У��д�룬�ñ�֤FLASH�����Ѿ���������0xFF
void Flash_WriteNoCheck(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte)
{
	u16 PageByte = 256 - _ulWriteAddr % 256;//��ҳʣ���д�ֽ���

	if(_usNByte <= PageByte)	//������256�ֽ�
	{
		PageByte = _usNByte;
	}
	
	while(1)
	{
		Flash_WritePage(ucpBuffer, _ulWriteAddr, PageByte);
		if(_usNByte == PageByte)	//д�����
			break;
		else
		{
			ucpBuffer += PageByte;	//��һҳд�������
			_ulWriteAddr += PageByte;	//��һҳд��ĵ�ַ
			_usNByte -= PageByte;	//��д����ֽ����ݼ�
			if(_usNByte > 256)
			{
				PageByte = 256;
			}
			else
			{
				PageByte = _usNByte;
			}
		}
	}
}


void Flash_WriteSomeBytes(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte)
{
	u32 ulSecPos = 0;				//�õ�����λ��
	u16 usSecOff = 0;				//����ƫ��
	u16 usSecRemain = 0;		//ʣ������
	u32 i = 0;

	ulSecPos = _ulWriteAddr / 4096;//��ַ��������(0--511)
	usSecOff = _ulWriteAddr % 4096;//�����ڵ�ַƫ��
	usSecRemain = 4096 - usSecOff;//������ȥƫ�ƣ���ʣ�����ֽ�

	if(_usNByte <= usSecRemain)	//д�����ݴ�С < ʣ�������ռ��С
	{
		usSecRemain = _usNByte;
	}

	while(1)
	{
		Flash_ReadSomeBytes(SectorBuf, ulSecPos*4096, 4096);//������������������
		for (i = 0; i < usSecRemain; i++)	//У������
		{
			if (SectorBuf[usSecOff + i] != 0xFF)//�������ݲ�Ϊ0xFF����Ҫ����
				break;
		}
		
		if(i < usSecRemain)	//��Ҫ����
		{
			Flash_EraseSector(ulSecPos);	//�����������
			for(i = 0; i < usSecRemain; i++)	//����д�������
			{
				SectorBuf[usSecOff + i] = ucpBuffer[i];
			}
			Flash_WriteNoCheck(SectorBuf, ulSecPos*4096, 4096);	//д����������(����=������+��д������)
		Flash_ReadSomeBytes(SectorBuf, 0*4096, 4096);//������������������
		}
		else
		{
			Flash_WriteNoCheck(ucpBuffer, _ulWriteAddr, usSecRemain);//����Ҫ����,ֱ��д������
		}
		if(_usNByte == usSecRemain)	//д�����
		{
			Flash_WriteDisable();
			break;
		}
		else
		{
			ulSecPos++;		//������ַ����1
			usSecOff = 0;		//����ƫ�ƹ���
			ucpBuffer += usSecRemain;	//ָ��ƫ��
			_ulWriteAddr += usSecRemain;	//д��ַƫ��
			_usNByte -= usSecRemain;	//��д����ֽڵݼ�

			if(_usNByte > 4096)
			{
				usSecRemain = 4096;	//��д��һ����(4096�ֽڴ�С)
			}
			else
			{
				usSecRemain = _usNByte;		//��д������һ����������
			}
		}
		
	}
	
}
