#include "gdflash.h"

u32 GDFLASH_ReadWord(u32 faddr)	  	//读出字 
{
	return *(vu32*)faddr;
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
u16 GDFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

void GDFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)		//从指定地址开始写入指定长度的数据
{
	FLASH_Status status = FMC_READY;
	u32 addrx=0;
	u32 endaddr=0;	
	if(WriteAddr<GD32_FLASH_BASE||WriteAddr%4)return;
	fmc_unlock();									//解锁 
  
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
	{
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if(GDFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				status=fmc_sector_erase(GDFLASH_GetFlashSector(addrx));//VCC=2.7~3.6V之间!!
				if(status!=FMC_READY)break;	//发生错误了
			}else addrx+=4;
		} 
	}
	if(status==FMC_READY)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(fmc_word_program(WriteAddr,*pBuffer)!=FMC_READY)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
	fmc_lock();//上锁
}

void GDFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=GDFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}
