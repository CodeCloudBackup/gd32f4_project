#include "dac.h"

#define DAC_OUT_VAL0 0x7FF0//0x7ff*3.3v/4096=1.65V
#define DAC_OUT_VAL1 0x7FF0

void DAC_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCU->AHB1EN|=1<<0;//GPIOA时钟使能
		RCU->APB1EN|=1<<29;//DAC时钟使能

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
		dac_deinit();
    /* configure the DAC0 */
    dac_trigger_disable(DAC0);
    dac_wave_mode_config(DAC0, DAC_WAVE_DISABLE);
    dac_output_buffer_enable(DAC0);
    
    /* configure the DAC1 */
    dac_trigger_disable(DAC1);
    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE);
    dac_output_buffer_enable(DAC1);
    
    /* enable DAC concurrent mode and set data */
    dac_concurrent_enable();
    dac_concurrent_data_set(DAC_ALIGN_12B_L, DAC_OUT_VAL0, DAC_OUT_VAL1);
}
