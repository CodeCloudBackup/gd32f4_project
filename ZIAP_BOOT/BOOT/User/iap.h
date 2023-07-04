#ifndef _GD32_BOOT_H_
#define _GD32_BOOT_H_

#include "gd32f4xx.h"
#include "systick.h"
#include "gdflash.h"
#include "nand_flash.h"


typedef  void (*iapfun)(void);				//定义一个函数类型的参数. 
#define FLASH_APP1_ADDR 0x08010000
#define Flash_App_Info  0x0800C000
#define FLASH_APP2_ADDR 0x08020000

void IAP_Load_App(u32 appxaddr);
void IAP_Write_Appbin(u32 appxaddr, u8 *appbuf, u32 applen);	


#endif
