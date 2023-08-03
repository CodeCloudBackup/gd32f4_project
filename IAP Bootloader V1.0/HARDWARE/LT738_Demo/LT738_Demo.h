/***************************************************
* Description      : LT738的测试程序
****************************************************/

#ifndef _LT738_Demo_h
#define _LT738_Demo_h
#include "usart.h"
#include "delay.h"
#include "LT738_Lib.h"
#include "LT738.h"
#include "if_port.h"


#define Picture_1_Addr	0                                //图片1在FLASH的地址
#define Picture_2_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*3)  //图片2在FLASH的地址
#define Picture_3_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*6)  //图片3在FLASH的地址
#define Picture_4_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*9)  //图片4在FLASH的地址
#define Picture_5_Addr	(LCD_XSIZE_TFT*LCD_YSIZE_TFT*12)  //图片5在FLASH的地址


#define layer1_start_addr 0                                //  图层在显存的位置
#define layer2_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3    //  
#define layer3_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*2  //  
#define layer4_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*3  //  
#define layer5_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*4  //  
#define layer6_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*5  //  
#define layer7_start_addr LCD_XSIZE_TFT*LCD_YSIZE_TFT*3*6  //  

#define Resolution	(LCD_XSIZE_TFT*LCD_YSIZE_TFT)   //分辨率

void Display_picture(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *bmp);
void GRAY_32(void);//灰阶
void WB_block(void);//棋盘格

#endif
