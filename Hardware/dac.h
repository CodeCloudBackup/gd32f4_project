#ifndef __DAC_H
#define __DAC_H
#include "systick.h"

void DAC1_Init(void);
void Dac1_Set_Val(u16 vol);
void DAC1_Test( u16 point, u16 maxnum );

#endif
