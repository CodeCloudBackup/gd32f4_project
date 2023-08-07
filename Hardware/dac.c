#include "dac.h"
#include "math.h"
#include "delay.h"
u16 Sine12bit[32] = {2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                     3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                     599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647
                    };

//�������Ҳ�����
//point һ�������ڵĵ���
//maxnum ���ֵ ���ֵ���ܳ�4096
void DAC1_Test( u16 point, u16 maxnum )
{
    u16 i = 0, j = 0, k = 0;
    float hd = 0.0;       //����
    float fz = 0.0;       //��ֵ
    u16 tem = 0;
    j = point / 2;
    hd = 3.14 / j;        // ��/2 ��ÿһ�����Ӧ�Ļ���ֵ
    k = maxnum / 2;       //���ֵһ��
    for( i = 0; i < point; i++ )
    {
        fz = k * sin( hd * i ) + k;
        tem = ( u16 )fz;
      //  delay_ms( 1 );
        DAC_SetChannel1Data(DAC_Align_12b_R, tem);
    }
}


//DACͨ��1�����ʼ��
void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOAʱ��
  RCU_APB1PeriphClockCmd(RCU_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DACͨ��1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

void Speaker_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOAʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	Dac1_Init();
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
}



