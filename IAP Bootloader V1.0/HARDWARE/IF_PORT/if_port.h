/****************************************************
* Description      : ѡ��ͬ��������ʽ
****************************************************/

#ifndef _if_port_h
#define _if_port_h
#include "sys.h"     

//ѡ��������ʽ

#define GD32_FSMC_16	1		// ʹ��FSMC������ʾģ��
#define GD32_SPI		  0		// ʹ��4��SPI������ʾģ��


#define LCD_RST PGout(6)

//ѡ��FSMC��������
#if GD32_FSMC_16
#define LCD_BASE0        ((u32)(0x6C000000 | 0x0000007E))
#define LCD_BASE1        ((u32)(0x6C000000 | 0x00000080))
#endif

//ѡ��SPI����
#if GD32_SPI							
#define SPI_CS 	PCout(1)
#endif




void Parallel_Init(void);

void FSMC_Init_16(void);
void FMSC_16_CmdWrite(u8 cmd);
void FMSC_16_DataWrite(u8 data);
void FMSC_16_DataWrite_Pixel(u16 data);
u8 FMSC_16_StatusRead(void);
u16 FMSC_16_DataRead(void);

void SPI2_Init(void);
u8 SPI2_ReadWriteByte(u8 TxData);
void SPI_CmdWrite(u8 cmd);
void SPI_DataWrite(u8 data);
void SPI_DataWrite_Pixel(u16 data);
u8 SPI_StatusRead(void);
u16 SPI_DataRead(void);

void LCD_CmdWrite(u8 cmd);
void LCD_DataWrite(u8 data);
void LCD_DataWrite_Pixel(u16 data);
u8 LCD_StatusRead(void);
u16 LCD_DataRead(void);
	 
void Delay_us(u16 time); //��ʱ����us��
void Delay_ms(u16 time); //��ʱ����ms��

void test_LCDIO(void);


#endif
