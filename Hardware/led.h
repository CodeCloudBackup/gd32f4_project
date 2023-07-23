#ifndef GD32_LED_H
#define GD32_LED_H

#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"
#include "systick.h"

#define LED PBout(4) //LED

void LED_Init(void);

#endif

