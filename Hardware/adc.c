#include "adc.h"
#include "systick.h"

void ADC_Init(u32 adc_periph)
{
	
		//配置PA4 ADC引脚
		rcu_periph_clock_enable(RCU_GPIOA);
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);//PA4配置成输入
		/* enable ADC1 clock */
		rcu_periph_clock_enable(RCU_ADC1);
		/* config ADC1 clock */
		adc_clock_config(ADC_ADCCK_PCLK2_DIV4);
	
	  /* reset ADC */
    adc_deinit();
//    /* ADC mode config */
	  adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);//独立模式
    /* ADC continous function enable */
    adc_special_function_config(adc_periph, ADC_CONTINUOUS_MODE, ENABLE); //连续转换
    /* ADC data alignment config */
    adc_data_alignment_config(adc_periph, ADC_DATAALIGN_RIGHT);
      /* ADC resolusion 6B */
    /* ADC channel length config */
    adc_channel_length_config(adc_periph, ADC_REGULAR_CHANNEL, 1);//规则通道
    /* ADC regular channel config */
    adc_regular_channel_config(adc_periph, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_144);
    /* ADC trigger config */
	  adc_external_trigger_config(adc_periph, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);//不采用外部触发
    /* enable ADC interface */
    adc_enable(adc_periph); //使能 
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(adc_periph);
		
		adc_software_trigger_enable(adc_periph,ADC_REGULAR_CHANNEL);//启动
}

u16 Get_ADC(u32 adc_periph, u8 ch)
{
			uint16_t Vol_Value,adc_value;
			adc_flag_clear(ADC1,ADC_FLAG_EOC);
      while(SET != adc_flag_get(ADC1,ADC_FLAG_EOC)){//查询转换完成
      }
		  adc_value = ADC_RDATA(ADC1);//读取ADC值
			Vol_Value=adc_value*3300/4095;//转换成电压值
			return adc_value;
}

u16 Get_Adc_Average(u32 adc_periph, u8 ch, u8 times);
