#include "dac.h"
#include "math.h"
#include "delay.h"
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
      //  delay_ms( 1 );
        DAC_SetChannel1Data(DAC_Align_12b_R, tem);
    }
}


//DAC通道1输出初始化
void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
  RCU_APB1PeriphClockCmd(RCU_APB1Periph_DAC, ENABLE);//使能DAC时钟
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}

void Speaker_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOE, ENABLE);//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//下拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	Dac1_Init();
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
}



