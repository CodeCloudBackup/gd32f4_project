#ifndef __HTTP_H
#define __HTTP_H 

#include "tcp.h"

BOOL GetIAPBin(void);
BOOL Handle_Http_Download_Header(u8* buff, u16 len_buf,u8 *resp_code, u8 *is_stream, u32 *cont_len);
#endif
