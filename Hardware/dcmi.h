#ifndef _DCMI_H
#define _DCMI_H
#include "systick.h"    									
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DCMI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.1 20140516
//�޸�DCMI_DMA_Init������֧��˫����ģʽ
//////////////////////////////////////////////////////////////////////////////////	 

extern void (*dcmi_rx_callback)(void);//DCMI DMA���ջص�����
 

void MyDCMI_Init(void);
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u32 DMA_Memory1BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);
void DCMI_Start(void);
void DCMI_Stop(void);
void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync);
#endif
