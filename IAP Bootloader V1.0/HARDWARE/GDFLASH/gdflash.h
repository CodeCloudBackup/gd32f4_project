#ifndef __GDFLASH_H__
#define __GDFLASH_H__
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6核心板
//GD32内部FLASH读写 驱动代码	   
//版本：V1.0									  
////////////////////////////////////////////////////////////////////////////////// 



//FLASH起始地址
#define GD32_FLASH_BASE 0x08000000 	//GD32 FLASH的起始地址
//FLASH解锁键值
#define FLASH_KEY1               0X45670123
#define FLASH_KEY2               0XCDEF89AB

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  


void GDFLASH_Unlock(void);					//FLASH解锁
void GDFLASH_Lock(void);				 	//FLASH上锁
u8 GDFLASH_GetStatus(void);				//获得状态
u8 GDFLASH_WaitDone(u32 time);				//等待操作结束
u8 GDFLASH_EraseSector(u32 sectoraddr);	 	//擦除扇区
u8 GDFLASH_WriteWord(u32 faddr, u32 dat);	//写入字
u32 GDFLASH_ReadWord(u32 faddr);		  	//读出字  
u8 GDFLASH_GetFlashSector(u32 addr);	//获取某个地址所在的flash扇区 
void GDFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
void GDFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u32 WriteData);								   
#endif

















