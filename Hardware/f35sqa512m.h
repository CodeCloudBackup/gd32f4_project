#ifndef __F35SQA512M_H
#define __F35SQA512M_H
#include "systick.h"

#define F35SQA512M	0XCD70
#define F35SQA_CS PBout(12) 

#define F35SQA_ManufactDeviceID	0x90 
#define F35SQA_JedecDeviceID		0x90 

void F35SQA_Init(void);
u32  F35SQA_ReadID(void);  	    		//∂¡»°FLASH ID
#endif

