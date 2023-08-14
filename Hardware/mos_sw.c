#include "mos_sw.h"
#include "delay.h"
void OutDirveInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟
	RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOG, ENABLE);//使能GPIOF时钟
  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
}

void Open_Lock_Test(void)
{
	delay_ms(2000);
	OUT_DRIVE_CTR2=1;
	delay_ms(500);
	OUT_DRIVE_CTR2=0;
}
