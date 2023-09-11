#ifndef __HTTP_APP_H
#define __HTTP_APP_H

#include "systick.h"
extern Byte8 HttpFlag;

#define HTTP_FLAG  								HttpFlag.Byte
#define HTTP_FLAG_EQUIP_IDENT 		HttpFlag.Bits.B0    // 身份验证
#define HTTP_FLAG_DOWNLOAD_BIN		HttpFlag.Bits.B1 		// 第三方拍照
#define HTTP_FLAG_UPLOAD_PHOTO		HttpFlag.Bits.B2	  // 打开投递口
#define HTTP_FLAG_UPLOAD_LOGFILE	HttpFlag.Bits.B3	  // 关闭投递口
#define HTTP_FLAG_TASK						HttpFlag.Bits.B4
#define HTTP_FLAG_IDENT_SUCCESS 	HttpFlag.Bits.B5	  // 设备认真成功标志

typedef enum 
{
	none=0,
	form_data,
	urlencoded,
	raw,
	binary
}CONTENT_TYPE;

void HTTP_Init(void);
u8 HM609A_Http_Program(const u8 sockid, const char *host,const u32 port);
u16 Http_Get_Package(char *buff_get, char *url_tail,const char *host, u16 port);
#endif
