#include "led.h"

void LED_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);//??GPIOB??
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);//PB4?????
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);//PB4???????,50M??
}

void LED_ON(void)
{
	gpio_bit_set(GPIOB, GPIO_PIN_4);//PB4?????
}
void LED_OFF(void)
{
	gpio_bit_reset(GPIOB, GPIO_PIN_4);//PB4?????
}



