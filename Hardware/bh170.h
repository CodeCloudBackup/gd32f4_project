#ifndef _BH170_H
#define _BH170_H

#include "gd32f4xx.h"
#include "iic.h"


void BH170_WriteReg(uint8_t reg_add);
void BH170_ReadData(unsigned char*Read);

#endif
