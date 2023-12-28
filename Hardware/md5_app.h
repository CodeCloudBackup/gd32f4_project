#ifndef __MD5_APP_H
#define __MD5_APP_H

#include "systick.h"
#include "malloc.h"
#include "usart.h"

void md5_sign(unsigned char *input, unsigned int input_len, unsigned char *output);

#endif
