#ifndef __ADC_H
#define __ADC_H	
#include "systick.h"

 							   
void InVolt_Adc_Init(void); 				//ADC通道初始化
u16 Get_InVolt_Adc_Val(void);
void  TempSensor_Adc_Init(void);
u16 Get_TempSensor_Adc_Val(void) ;
void  Micro_Adc_Init(void);
u16 Get_Micro_Adc_Val(void);
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
void Sensor_Adc_Test(void);
#endif 
