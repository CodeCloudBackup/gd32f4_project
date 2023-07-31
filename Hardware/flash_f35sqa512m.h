#ifndef _FLASH_F35SQA512M_H
#define _FLASH_F35SQA512M_H

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"
#include "delay.h"
#include "spi.h"
#include "systick.h"

#define	F35SQA512M_CS 	PBout(12)  		//W25QXX的片选信号

///////////////////////////////////////////////////////////////////////////////// 
//指令表
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
u16  FLASH_ReadID(void);  	    		//读取FLASH ID
u8	 FLASH_ReadSR(void);        		//读取状态寄存器 
void FLASH_Write_SR(u8 sr);  			//写状态寄存器
void FLASH_Write_Enable(void);  		//写使能 
void FLASH_Write_Disable(void);		//写保护
void FLASH_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void FLASH_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void FLASH_Erase_Chip(void);    	  	//整片擦除
void FLASH_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void FLASH_Wait_Busy(void);           	//等待空闲
void FLASH_PowerDown(void);        	//进入掉电模式
void FLASH_WAKEUP(void);				//唤醒

#endif
