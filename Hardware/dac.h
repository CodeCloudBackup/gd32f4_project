#ifndef __DAC_H
#define __DAC_H	 
#include "systick.h"

#define SPEAKER_SW PEout(8)

void Dac1_Init(void);		//DACͨ��1��ʼ��	 	 
void Dac1_Set_Vol(u16 vol);	//����ͨ��1�����ѹ
void DAC1_Test( u16 point, u16 maxnum );
void Speaker_Init(void);
#endif
