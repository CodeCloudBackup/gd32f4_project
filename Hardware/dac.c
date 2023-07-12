#include "dac.h"

#define DAC_OUT_VAL0 0x7FF0//0x7ff*3.3v/4096=1.65V
#define DAC_OUT_VAL1 0x7FF0

void DAC_Init(void)
{
		rcu_periph_clock_enable(RCU_GPIOA);//GPIOA时钟使能
		rcu_periph_clock_enable(RCU_DAC);//DAC时钟使能
		gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4|GPIO_PIN_5);//PA4,PA5配置成AD
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