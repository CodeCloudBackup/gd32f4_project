#ifndef __GDFLASH_H__
#define __GDFLASH_H__
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6���İ�
//GD32�ڲ�FLASH��д ��������	   
//�汾��V1.0									  
////////////////////////////////////////////////////////////////////////////////// 



//FLASH��ʼ��ַ
#define GD32_FLASH_BASE 0x08000000 	//GD32 FLASH����ʼ��ַ
//FLASH������ֵ
#define FLASH_KEY1               0X45670123
#define FLASH_KEY2               0XCDEF89AB

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  


void GDFLASH_Unlock(void);					//FLASH����
void GDFLASH_Lock(void);				 	//FLASH����
u8 GDFLASH_GetStatus(void);				//���״̬
u8 GDFLASH_WaitDone(u32 time);				//�ȴ���������
u8 GDFLASH_EraseSector(u32 sectoraddr);	 	//��������
u8 GDFLASH_WriteWord(u32 faddr, u32 dat);	//д����
u32 GDFLASH_ReadWord(u32 faddr);		  	//������  
u8 GDFLASH_GetFlashSector(u32 addr);	//��ȡĳ����ַ���ڵ�flash���� 
void GDFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void GDFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr,u32 WriteData);								   
#endif

















