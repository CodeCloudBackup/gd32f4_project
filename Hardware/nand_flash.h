#ifndef __NAND_FLASH_H
#define __NAND_FLASH_H 	

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"
#include "systick.h"
#include "delay.h"

#define	W25QXX_CS 		PBout(14)  		//W25QXX的片选信号

#define  FLASH_WRITE_ENABLE_CMD 		0x06
#define  FLASH_WRITE_DISABLE_CMD		0x04
#define  FLASH_READ_SR_CMD				   0x05
#define  FLASH_WRITE_SR_CMD				   0x01
#define  FLASH_READ_DATA				     0x03
#define  FLASH_FASTREAD_DATA			   0x0b
#define  FLASH_WRITE_PAGE			        0x02
#define  FLASH_ERASE_PAGE      			  0x81
#define  FLASH_ERASE_SECTOR       		0x20
#define	 FLASH_ERASE_BLOCK				    0xd8
#define	 FLASH_ERASE_CHIP				      0xc7
#define  FLASH_POWER_DOWN				      0xb9
#define  FLASH_RELEASE_POWER_DOWN       0xab
#define  FLASH_READ_DEVICE_ID      		0x90
#define  FLASH_READ_JEDEC_ID      		0x9f

#define 	W25Q_FLASH_SIZE	 (1*1024*1024)	// 1M字节
#define		PAGE_SIZE			8192	// 256 bytes
#define 	SECTOR_SIZE		512	 // 4-Kbyte
#define		BLOCK_SIZE		32	// 64-Kbyte	

#define PAGE_LEN		255	 //一页256字节
#define FALSH_TEST_ADDR		(0x100)

//定义FLASH相关属性定义
struct Flash_Attr  {
	u16 flash_id;
	u16 page_size;
	u16 sector_size;
	u8 block_size;
};	

void Flash_RD_Test(u8 *ReadBuff);

void Flash_TestDataInit(void);
u8 spi_master_send_recv_byte(u8 spi_byte);
void spi_master_recv_some_bytes( u8 *pbdata, u16 recv_length);
u32 spi_flash_read_id(void);
u16 SFLASH_ReadID(void);
void Flash_WriteDisable(void);
void Flash_WriteEnable(void);
u8 Flash_ReadSR(void);
void Flash_WaitNobusy(void);
void Flash_WriteSR(u8 _ucByte);
void Flash_ErasePage(u32 _ulPageAddr);
void Flash_EraseSector(u32 _ulSectorAddr);
void Flash_EraseBlock(u32 _ulBlockAddr);
void Flash_EraseChip(void);
void Flash_ReadSomeBytes(u8 *ucpBuffer, u32 _ulReadAddr, u16 _usNByte);
void Flash_WritePage(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte);
void Flash_WriteNoCheck(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte);
void Flash_WriteSomeBytes(u8 *ucpBuffer, u32 _ulWriteAddr, u16 _usNByte);
#endif
