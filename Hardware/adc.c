#include "adc.h"
#include "delay.h"
#include <math.h>
#include "usart.h"
//初始化ADC															   
void  InVolt_Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCU_APB2PeriphClockCmd(RCU_APB2Periph_ADC0, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA7 通道7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCU_APB2PeriphResetCmd(RCU_APB2Periph_ADC0,ENABLE);	  //ADC1复位
	RCU_APB2PeriphResetCmd(RCU_APB2Periph_ADC0,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC0, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC0, ENABLE);//开启AD转换器	

}		

//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_InVolt_Adc_Val(void)   
{
		u16 adcx=0;
		//设置指定ADC的规则组通道，一个序列，采样时间
		adcx=Get_Adc(ADC_Channel_7);
		adcx=(float)adcx*(330/4096.0);
		return adcx;
}

//初始化ADC															   
void  TempSensor_Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
  RCU_APB2PeriphClockCmd(RCU_APB2Periph_ADC0, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PC4 通道1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
 
	RCU_APB2PeriphResetCmd(RCU_APB2Periph_ADC0,ENABLE);	  //ADC1复位
	RCU_APB2PeriphResetCmd(RCU_APB2Periph_ADC0,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC0, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC0, ENABLE);//开启AD转换器	
}		

const float Rp = 10000.0;
const float T2 = 298.15; //273.15+25.0
const float Bx = 3435;
const float Ka = 273.15;
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_TempSensor_Adc_Val(void)   
{
		u16 adcx=0;
		u16 Rt=0;
		u16 temp=0;
		//设置指定ADC的规则组通道，一个序列，采样时间
		adcx=Get_Adc(ADC_Channel_14);
		adcx=(float)adcx*(330/4096.0);
	  Rt=Rp*(adcx/(float)(330-adcx));
	
		if(Rt==0)
			Rt+=5;
		temp=1/(1/T2+1/Bx*log(Rt/Rp))-Ka;
		return temp;
}

//初始化ADC															   
void  Micro_Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
  RCU_APB2PeriphClockCmd(RCU_APB2Periph_ADC0, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
 
	RCU_APB2PeriphResetCmd(RCU_APB2Periph_ADC0,ENABLE);	  //ADC1复位
	RCU_APB2PeriphResetCmd(RCU_APB2Periph_ADC0,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC0, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC0, ENABLE);//开启AD转换器	
}	

//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Micro_Adc_Val(void)   
{
		u16 adcx=0;
		//设置指定ADC的规则组通道，一个序列，采样时间
		adcx=Get_Adc(ADC_Channel_15);
		adcx=adcx*(33000/4096);
		return adcx;
}

//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC0, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC0);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC0, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC0);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 

void Sensor_Adc_Test(void)
{
	u16 inVolt=0; 
	u16 temp=0;	
	delay_ms(1000);
	inVolt=Get_InVolt_Adc_Val();
	printf("InVolt:%d\n",inVolt);
	temp=Get_TempSensor_Adc_Val();
	printf("temp:%d\n",temp);
}
