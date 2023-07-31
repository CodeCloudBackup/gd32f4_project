#ifndef _FLASH_F35SQA512M_H
#define _FLASH_F35SQA512M_H

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"
#include "delay.h"
#include "spi.h"
#include "systick.h"

#define	F35SQA512M_CS 	PBout(12)  		//W25QXX��Ƭѡ�ź�

///////////////////////////////////////////////////////////////////////////////// 
//ָ���
#define F35S_WriteEnable			0x06 
#define F35S_WriteDisable			0x04 
//#define F35S_ReadStatusReg		0x05 
//#define F35S_WriteStatusReg		0x01 
#define F35S_GetFeature         0x0F
#define F35S_SetFeature         0x1F
#define F35S_ReadData						0x03 
#define F35S_ReadPageData    	  0x13
#define F35S_FastReadData				0x0B 
#define F35S_FastReadDual				0x3B 
#define F35S_PageProgram				0x02 
#define F35S_BlockErase					0xD8 
//#define F35S_SectorErase			0x20 
//#define F35S_ChipErase				0xC7 
//#define F35S_PowerDown				0xB9 
//#define F35S_ReleasePowerDown	0xAB 
//#define F35S_DeviceID					0xAB 
//#define F35S_ManufactDeviceID	0x90 
#define F35S_JedecDeviceID		0x9F 

void FLASH_Init(void);
u16  FLASH_ReadID(void);  	    		//��ȡFLASH ID
u8	 FLASH_ReadSR(void);        		//��ȡ״̬�Ĵ��� 
void FLASH_Write_SR(u8 sr);  			//д״̬�Ĵ���
void FLASH_Write_Enable(void);  		//дʹ�� 
void FLASH_Write_Disable(void);		//д����
void FLASH_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void FLASH_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void FLASH_Erase_Chip(void);    	  	//��Ƭ����
void FLASH_Erase_Sector(u32 Dst_Addr);	//��������
void FLASH_Wait_Busy(void);           	//�ȴ�����
void FLASH_PowerDown(void);        	//�������ģʽ
void FLASH_WAKEUP(void);				//����

#endif
