#include "dac.h"
#include "math.h"

void DAC1_Init(void)
{
    DAC_InitTypeDef DAC_InitStructure;

    RCU_APB1PeriphClockCmd(RCU_APB1Periph_DAC, ENABLE);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);

}
//输出电压 0---3300，代表0--3.3V
void Dac1_Set_Val(u16 vol)
{
	float temp = vol;
	temp /= 1000;
	temp = temp * 4096 / 3.3f;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);
}


u16 Sine12bit[32] = {2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                     3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                     599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
                    };

//生成正弦波数据
//point 一个周期内的点数
//maxnum 最大值 最大值不能超4096
void DAC1_Test( u16 point, u16 maxnum )
{
    u16 i = 0, j = 0, k = 0;
    float hd = 0.0;       //弧度
    float fz = 0.0;       //峰值
    u16 tem = 0;
    j = point / 2;
    hd = 3.14 / j;        // π/2 内每一个点对应的弧度值
    k = maxnum / 2;       //最大值一半
    for( i = 0; i < point; i++ )
    {
        fz = k * sin( hd * i ) + k;
        tem = ( u16 )fz;
        //delay_ms( 1 );
        DAC_SetChannel1Data(DAC_Align_12b_R, tem);
    }
}

