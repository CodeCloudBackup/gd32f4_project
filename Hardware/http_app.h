#ifndef __HTTP_APP_H
#define __HTTP_APP_H

#include "systick.h"


typedef enum
{
	EQUIP_IDENT=0,
	DOWNLOAD_BIN,
	UPLOAD_PHOTO,
	UPLOAD_LOGFILE
}HTTP_REQ_ENUM;

typedef enum 
{
	NONE=0,
	FORM_DATA,
	URLENCODED,
	RAW,
	BINARY
}CONTENT_TYPE;

// http命令执行状态，0:未标志，1:发起请求标志，2:等待回复标志
typedef struct 
{
	u8 sta_cmd;
	u8 sta_equip_ident;
	u8 sta_download_bin;
	u8 sta_upload_photo;
	u8 sta_upload_logfile;
}HTTP_CMD_STA;

extern HTTP_CMD_STA g_sHttpCmdSta;
u32 HTTP_Recvice(u8* buf, u32 buflen);
void HTTP_Init(void);
u8 HM609A_Http_Program(const u8 sockid, const char *host,const u32 port);
u16 Http_Get_Package(char *buff_get, char *url_tail,const char *host, u16 port);
u16 Http_Response_Analysis( u8* buf, u16 buf_len,u8* content,u32 *cont_len);
void Http_Send_Resquest(const u8 sockid, const char *host,const u32 port);
#endif
