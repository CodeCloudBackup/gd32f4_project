#ifndef __ADC_H
#define __ADC_H

#include "gd32f4xx.h"
#include "gd32f4xx_adc.h"

void ADC_Init(u32 adc_periph); //ADC通道初始化
u16 Get_ADC(u32 adc_periph, u8 ch);	 //获得某个通道值 
u16 Get_Adc_Average(u32 adc_periph, u8 ch, u8 times); //得到某个通道给定次数采样的平均值  
#endif

