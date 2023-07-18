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
	u8 WriteBuff[10]={0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};//读写存储
	u32 FlashJedecid,FlashDeviceid;//FLASH ID变量
	switch(state)
	{
		case 0:
			FlashJedecid = spi_flash_read_id();//读取Jedecid
			FlashDeviceid=SFLASH_ReadID();//读取Device ID
			Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
			Flash_WriteSR(0x42);//解除保护
			state++;
			break;
		case 1:
			Flash_ReadSR();//读状态寄存器
			Flash_WriteSomeBytes(WriteBuff,0,8);//把WriteBuff数组中的内容写入FLASH 0地址
			state++;
			break;
		case 2:
			Flash_ReadSomeBytes(ReadBuff,0,8);//从FLASH 0地址读取8字节内容放入ReadBuff数组
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
		*temp_data++ = spi_master_send_recv_byte(0xFF);	//发送 0xff 为从设备提供时钟
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
   FLASH_CS_0();                              //使能器件

	spi_master_send_recv_byte(0x90);//发送读取ID命令	    
	spi_master_send_recv_byte(0x00); 	    
	spi_master_send_recv_byte(0x00); 	    
	spi_master_send_recv_byte(0x00); 

	ID |= spi_master_send_recv_byte(0xFF)<<8;              //读取ID
	ID |= spi_master_send_recv_byte(0xFF);
  
   FLASH_CS_1();                             //失能器件
	
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
	spi_master_send_recv_byte(command);//开启写使能
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
	
	while(((Flash_ReadSR()) & 0x01)==0x01);	//等待BUSY位清空
}
void Flash_WriteSR(u8 _ucByte)
{
	u8 command = FLASH_WRITE_SR_CMD;

	Flash_WriteEnable();	
	Flash_WaitNobusy();

	FLASH_CS_0();	
	spi_master_send_recv_byte(command);	//01h
	spi_master_send_recv_byte(_ucByte);	//写入一个字节
  FLASH_CS_1();
}

void Flash_ErasePage(u32 _ulPageAddr)
{
	_ulPageAddr *= 256;
	
	Flash_WriteEnable();
	Flash_WaitNobusy();
	
	FLASH_CS_0();
	spi_master_send_recv_byte(FLASH_ERASE_PAGE);//页擦除指令
	spi_master_send_recv_byte((u8)(_ulPageAddr>>16));	//写入24位地址
	spi_master_send_recv_byte((u8)(_ulPageAddr>>8));
	spi_master_send_recv_byte((u8)(_ulPageAddr>>0));
	FLASH_CS_1();
	Flash_WaitNobusy();	//等待写入结束
}


void Flash_EraseSector(u32 _ulSectorAddr)
{
	u8 command = FLASH_ERASE_SECTOR;
	_ulSectorAddr *= 4096;	//1个扇区 4 KBytes
	Flash_WriteEnable();
	Flash_WaitNobusy();
	FLASH_CS_0();
	
	spi_master_send_recv_byte(command);
	spi_master_send_recv_byte((u8)(_ulSectorAddr >> 16));
	spi_master_send_recv_byte((u8)(_ulSectorAddr >> 8));
	spi_master_send_recv_byte((u8)(_ulSectorAddr));
	FLASH_CS_1();
	
	Flash_WaitNobusy();	//等待写入结束
}


void Flash_EraseBlock(u32 _ulBlockAddr)
{
	u8 command = FLASH_ERASE_BLOCK;
	_ulBlockAddr *= 65536;	//块地址,一块64K
	
	Flash_WriteEnable();
	Flash_WaitNobusy();

	FLASH_CS_0();
	spi_master_send_recv_byte(command);
	spi_master_send_recv_byte(_ulBlockAddr>>16);
	spi_master_send_recv_byte(_ulBlockAddr>>8);
	spi_master_send_recv_byte(_ulBlockAddr>>0);
	FLASH_CS_1();

	Flash_WaitNobusy();	//等待写入结束
}


void Flash_EraseChip(void)
{
	u8 command = FLASH_ERASE_CHIP;

	Flash_WriteEnable();	//flash芯片写使能
	Flash_WaitNobusy();	//等待写操作完成
	
	FLASH_CS_0();
	spi_master_send_recv_byte(command);
	FLASH_CS_1();
	Flash_WaitNobusy();	//等待写入结束
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
	

	Flash_WriteEnable();	//写使能
	Flash_WaitNobusy();	//等待写入结束
	
  FLASH_CS_0();
	
	spi_master_send_recv_byte(FLASH_WRITE_PAGE);	//02h
	spi_master_send_recv_byte(_ulWriteAddr>>16);	//写入24位地址
	spi_master_send_recv_byte(_ulWriteAddr>>8);
	spi_master_send_recv_byte(_ulWriteAddr>>0);
	while(_usNByte--)
	{
		spi_master_send_recv_byte(*ucpBuffer);	//SPI 写入单个字节
		ucpBuffer++;
	}
	FLASH_CS_1();
	Flash_WaitNobusy();	//等待写入结束
}

//不带校验写入，得保证FLASH数据已经被擦除成0xFF
void Flash_WriteNoCheck(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte)
{
	u16 PageByte = 256 - _ulWriteAddr % 256;//单页剩余可写字节数

	if(_usNByte <= PageByte)	//不大于256字节
	{
		PageByte = _usNByte;
	}
	
	while(1)
	{
		Flash_WritePage(ucpBuffer, _ulWriteAddr, PageByte);
		if(_usNByte == PageByte)	//写入结束
			break;
		else
		{
			ucpBuffer += PageByte;	//下一页写入的数据
			_ulWriteAddr += PageByte;	//下一页写入的地址
			_usNByte -= PageByte;	//待写入的字节数递减
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
	u32 ulSecPos = 0;				//得到扇区位置
	u16 usSecOff = 0;				//扇区偏移
	u16 usSecRemain = 0;		//剩余扇区
	u32 i = 0;

	ulSecPos = _ulWriteAddr / 4096;//地址所在扇区(0--511)
	usSecOff = _ulWriteAddr % 4096;//扇区内地址偏移
	usSecRemain = 4096 - usSecOff;//扇区除去偏移，还剩多少字节

	if(_usNByte <= usSecRemain)	//写入数据大小 < 剩余扇区空间大小
	{
		usSecRemain = _usNByte;
	}

	while(1)
	{
		Flash_ReadSomeBytes(SectorBuf, ulSecPos*4096, 4096);//读出整个扇区的内容
		for (i = 0; i < usSecRemain; i++)	//校验数据
		{
			if (SectorBuf[usSecOff + i] != 0xFF)//储存数据不为0xFF，需要擦除
				break;
		}
		
		if(i < usSecRemain)	//需要擦除
		{
			Flash_EraseSector(ulSecPos);	//擦除这个扇区
			for(i = 0; i < usSecRemain; i++)	//保存写入的数据
			{
				SectorBuf[usSecOff + i] = ucpBuffer[i];
			}
			Flash_WriteNoCheck(SectorBuf, ulSecPos*4096, 4096);	//写入整个扇区(扇区=老数据+新写入数据)
		Flash_ReadSomeBytes(SectorBuf, 0*4096, 4096);//读出整个扇区的内容
		}
		else
		{
			Flash_WriteNoCheck(ucpBuffer, _ulWriteAddr, usSecRemain);//不需要擦除,直接写入扇区
		}
		if(_usNByte == usSecRemain)	//写入结束
		{
			Flash_WriteDisable();
			break;
		}
		else
		{
			ulSecPos++;		//扇区地址增加1
			usSecOff = 0;		//扇区偏移归零
			ucpBuffer += usSecRemain;	//指针偏移
			_ulWriteAddr += usSecRemain;	//写地址偏移
			_usNByte -= usSecRemain;	//待写入的字节递减

			if(_usNByte > 4096)
			{
				usSecRemain = 4096;	//待写入一扇区(4096字节大小)
			}
			else
			{
				usSecRemain = _usNByte;		//待写入少于一扇区的数据
			}
		}
		
	}
	
}
