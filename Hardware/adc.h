#ifndef __ADC_H
#define __ADC_H

#include "gd32f4xx.h"
#include "gd32f4xx_adc.h"

void ADC_Init(u32 adc_periph); //ADCͨ����ʼ��
u16 Get_ADC(u32 adc_periph, u8 ch);	 //���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u32 adc_periph, u8 ch, u8 times); //�õ�ĳ��ͨ����������������ƽ��ֵ  
#endif

