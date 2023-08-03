/***************************************************
* Description      : LT738�Ĳ��Գ���
****************************************************/

#ifndef _LT738_Demo_h
#define _LT738_Demo_h
#include "usart.h"
#include "delay.h"
#include "LT738_Lib.h"
#include "LT738.h"
#include "if_port.h"


#define Picture_1_Addr	0                                //ͼƬ1��FLASH�ĵ�ַ
#define Picture_2_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*3)  //ͼƬ2��FLASH�ĵ�ַ
#define Picture_3_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*6)  //ͼƬ3��FLASH�ĵ�ַ
#define Picture_4_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*9)  //ͼƬ4��FLASH�ĵ�ַ
#define Picture_5_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*12)  //ͼƬ5��FLASH�ĵ�ַ


#define layer1_start_addr 0                                //  ͼ�����Դ��λ��
#define layer2_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3    //  
#define layer3_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*2  //  
#define layer4_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*3  //  
#define layer5_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*4  //  
#define layer6_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*5  //  
#define layer7_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*6  //  

#define Resolution	(LCD_XSIZE_TFT*LCD_YSIZE_TFT)   //�ֱ���

void Display_picture(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *bmp);
void GRAY_32(void);//�ҽ�
void WB_block(void);//���̸�

#endif
