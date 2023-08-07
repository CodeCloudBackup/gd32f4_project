#ifndef __DAC_H
#define __DAC_H	 
#include "systick.h"

#define SPEAKER_SW PEout(8)

void Dac1_Init(void);		//DAC通道1初始化	 	 
void Dac1_Set_Vol(u16 vol);	//设置通道1输出电压
void DAC1_Test( u16 point, u16 maxnum );
void Speaker_Init(void);
#endif
