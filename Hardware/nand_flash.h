#ifndef __NAND_FLASH_H
#define __NAND_FLASH_H 	

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"
#include "systick.h"
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
	uint16_t flash_id;
	uint16_t page_size;
	uint16_t sector_size;
	uint8_t block_size;
};	

void Flash_RD_Test(uint8_t *ReadBuff);

void Flash_TestDataInit(void);
uint8_t spi_master_send_recv_byte(uint8_t spi_byte);
void spi_master_recv_some_bytes( uint8_t *pbdata, uint16_t recv_length);
uint32_t spi_flash_read_id(void);
uint16_t SFLASH_ReadID(void);
void Flash_WriteDisable(void);
void Flash_WriteEnable(void);
uint8_t Flash_ReadSR(void);
void Flash_WaitNobusy(void);
void Flash_WriteSR(uint8_t _ucByte);
void Flash_ErasePage(uint32_t _ulPageAddr);
void Flash_EraseSector(uint32_t _ulSectorAddr);
void Flash_EraseBlock(uint32_t _ulBlockAddr);
void Flash_EraseChip(void);
void Flash_ReadSomeBytes(uint8_t *ucpBuffer, uint32_t _ulReadAddr, uint16_t _usNByte);
void Flash_WritePage(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
void Flash_WriteNoCheck(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
void Flash_WriteSomeBytes(uint8_t *ucpBuffer, uint32_t _ulWriteAddr, uint16_t _usNByte);
#endif