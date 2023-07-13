#include "http.h"
#include <string.h>

static char g_urlStr[100];
static char *g_interface = "/iob/download/app.bin";
static char *g_ip = "101.37.89.157";
static char *g_port = "80";

BOOL Package_Http_Get_Download_Header(char* buff, u16 size)
{
	u8 tmp[64];
	memset(buff, 0, size);
	memcpy(buff, "GET ", strlen("GET "));
	strcat(buff, g_interface);
	strcat(buff, " HTTP/1.1\r\n");
	memset(tmp, 0 , sizeof(tmp));
	sprintf((char*)tmp, "Host: %s:%s\r\n", g_ip, g_port);
	strcat(buff, (char*)tmp);
	// strcat((char *)buff, "Connection: Keep-Alive\r\n");
	// strcat((char *)buff, "User-Agent: W5500\r\n");
	// strcat((char *)buff, "Accept-Encoding: gzip,deflate\r\n");
	strcat(buff, "\r\n");
	return TRUE;
}

u8 Analysis_Http_Download_Header(u8* buff, u16 len_buf, u8 *is_stream, u32 *cont_len)
{
	u8 resp_code = 0;
	*is_stream = 0;
	*cont_len = 0;
	u8 offset_ContLen = strlen("Content-Length: ");
	if(strstr((char*)buff, "\r\n\r\n") != NULL)
	{
		char *p1 = NULL, *p2 = NULL;
		p1 = strstr((char*)buff, "HTTP/1.1 200");
		if(p1 != NULL)
		{
				resp_code = 200;
		} 
		
		p1 = strstr((char*)buff, "Content-Length: ");
		if(p1 != NULL)
		{
				p2 = strstr(p1, "\r\n");
				if(p2 != NULL)
				{
					char str_len[8] ={0};
					memcpy(str_len, p1 + offset_ContLen, p2 - p1 - offset_ContLen);
					*cont_len= atoi(str_len);
					
				}else resp_code = 0;
		}else resp_code = 0;
	}
	return resp_code;
}


BOOL Get_Http_Download_File(u8* buff, u32 size, u32* head_len)
{
	u8* posHeader = NULL;
	posHeader = (u8*)strstr((char*)buff, "\r\n\r\n");
	*head_len = posHeader + 4 - buff;
	return TRUE;
}

static u8 g_status = 0;
BOOL Http_Program(APP_INFO app_info)
{
	u8* http_buff = NULL;
	u8 resp_code = 0,is_stream = 0;
	u32 cont_len = 0,head_len = 0;
	u32 version = 0x1234567a;
	u8 appInfoBuff[8];
	Package_Http_Get_Download_Header(g_urlStr, sizeof(g_urlStr));
	switch(g_status)
	{
		case 0:
				if(TCP_Send_Data(g_urlStr, strlen(g_urlStr)))
				{
					g_status++;
				}
				break;
		case 1:
			http_buff = mymalloc(SRAMIN, 32*1024);
			if(USART5_Revice(COMMAND,http_buff)){
				resp_code = Analysis_Http_Download_Header(http_buff, sizeof(http_buff), &is_stream, &cont_len);
				if(resp_code == 200)
				{
					app_info.App_Version = 0x12345679;
					app_info.App_Size = cont_len;
					g_status++;
				}	
			}	
			break;
		case 2:
			if(Get_Http_Download_File(http_buff, cont_len, &head_len))
			{
				Flash_WriteSomeBytes((u8*)&app_info,0,8);
				Flash_WriteSomeBytes(http_buff+head_len,8,cont_len);//把WriteBuff数组中的内容写入FLASH 0地址
				myfree(SRAMIN, http_buff);
				g_status++;
			}
			break;
		default:
			break;
	}
	
	return TRUE;
}

