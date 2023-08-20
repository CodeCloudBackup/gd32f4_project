#ifndef __F35SQA512M_H
#define __F35SQA512M_H
#include "systick.h"

#define F35SQA512M	0XEF17
#define F35SQA_CS PBout(12) 

void F35SQA_Init(void);
u16  F35SQA_ReadID(void);  	    		//∂¡»°FLASH ID
#endif

