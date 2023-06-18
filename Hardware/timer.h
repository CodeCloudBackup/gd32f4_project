#ifndef GD32_TIMER_H
#define GD32_TIMER_H

#include "gd32f4xx_timer.h"

void TIMER1_Init(void);
void TIMER2_Init(void);

BOOL TIMER1_5ms(void);
BOOL TIMER1_10ms(void);
BOOL TIMER1_50ms(void);
BOOL TIMER1_100ms(void);
BOOL TIMER1_200ms(void);
BOOL TIMER1_1000ms(void);
#endif

