#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6核心板
//按键输入驱动代码	   
//版本：V1.0								  
////////////////////////////////////////////////////////////////////////////////// 	 


#define KEY0 		  PEin(4)   //PE4
#define WK_UP 		PAin(0)		//PA0 

#define KEY0_PRES 	1	//KEY0按下
#define WKUP_PRES   2	//KEY_UP按下(即WK_UP)

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数					    
#endif
