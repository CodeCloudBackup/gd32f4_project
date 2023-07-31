#ifndef __SPI_H
#define __SPI_H

#include "gd32f4xx_rcu.h"
#include "gd32f4xx_spi.h"

void SPI_Init(void);
u8 SPI1_ReadWriteByte(u8 TxData);
#endif
