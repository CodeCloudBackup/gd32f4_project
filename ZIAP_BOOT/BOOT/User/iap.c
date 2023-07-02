#include "iap.h"

iapfun jump2app;

uint32_t iapbuf[512];


void IAP_Load_App(u32 appxaddr)
{	
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		__set_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
		
}

void IAP_Write_Appbin(u32 appxaddr, u8 *appbuf, u32 applen)
{
	u32 t;
	u32 temp;
	u16 i=0;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;
	for(t=0;t<applen;t+=4)
	{
		temp=(u32)dfu[3]<<24;
		temp=(u32)dfu[2]<<16;
		temp=(u32)dfu[1]<<8;
		temp=(u32)dfu[0];
		dfu+=4;
		iapbuf[i++]=temp;
		if(i==512)
		{
			i=0;
			GDFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//偏移2048  512*4=2048
		}
	}
	if(i)GDFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}
