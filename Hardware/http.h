#ifndef __HTTP_H
#define __HTTP_H 

#include "tcp.h"
#include "usart.h"
#include "malloc.h"
#include "nand_flash.h"
#include "gdflash.h"

BOOL Package_Http_Get_Download_Header(char* buff, u16 size);
u8 Analysis_Http_Download_Header(u8* buff, u16 len_buf, u8 *is_stream, u32 *cont_len);
BOOL Get_Http_Download_File(u8* buff, u32 size, u32* head_len);
BOOL Http_Program(void);

#endif
