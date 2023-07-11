#include "http.h"
#include <string.h>
char g_urlStr[70] = "GET /iob/download/test.txt HTTP/1.1\r\nHost:101.37.89.157\r\n\r\n";

BOOL GetIAPBin(void)
{
	u32 len = sizeof(g_urlStr);
	TCP_Send_Data(g_urlStr, strlen(g_urlStr));
	return TRUE;
}

BOOL Handle_Http_Download_Header(u8* buff, u16 len_buf,u8 *resp_code, u8 *is_stream, u32 *cont_len)
{
	*resp_code = 0;
	*is_stream = 0;
	*cont_len = 0;
	u8 offset_ContLen = strlen("Content-Length: ");
	if(strstr((char*)buff, "\r\n\r\n") != NULL)
	{
		char *p1 = NULL, *p2 = NULL;
		p1 = strstr((char*)buff, "HTTP/1.1 200");
		if(p1 != NULL)
		{
				*resp_code = 200;
		} else return FALSE;
		
		
		p1 = strstr((char*)buff, "Content-Length: ");
		if(p1 != NULL)
		{
				p2 = strstr(p1, "\r\n");
				if(p2 != NULL)
				{
					char str_len[8] ={0};
					memcpy(str_len, p1 + offset_ContLen, p2 - p1 - offset_ContLen);
					*cont_len= atoi(str_len);
					return TRUE;
				}
		}else return FALSE;
	}
	return FALSE;
}