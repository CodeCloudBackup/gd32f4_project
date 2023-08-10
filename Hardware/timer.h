#ifndef _TIMER_H
#define _TIMER_H

#include "systick.h"

void TIM1_Init(u16 arr,u16 psc);

void TIM3_PWM_Init(u16 arr,u16 psc);
#endif

