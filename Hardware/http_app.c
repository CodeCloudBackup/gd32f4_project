#include "http_app.h"
#include "usart.h"
#include "hm609a.h"
#include "malloc.h"
#include <stdlib.h>

Byte8 HttpFlag;

u16 g_httpReturnTim=0;


u16 Http_Get_Package(char *buff_get, char *url_tail,const char *host, u16 port)
{
	u16 len;
	len = sprintf(buff_get, "GET %s HTTP/1.1\r\n"
			"Connection:close\r\n"
			"Host:%s:%d\r\n"
			"Connection:close\r\n"
			"User-Agent:GD32F427\r\n"
			"\r\n",
			url_tail,host,port
		);
	return len;
}

u8 Http_Get_Analysis_Header(u8* buf, u16 buf_len, u8 *resp_code,u32 *cont_len)
{	
	*resp_code=0;
	*cont_len=0;
	if(strstr((char*)buf, "\r\n\r\n") != NULL)//response header ok
	{
		char *p1, *p2;
		p1 = strstr((char*)buf, "HTTP/1.1 200");	
		if(p1 != NULL)// io stream
		{
			*resp_code = 200;
		}		
		p1 = strstr((char*)buf, "Content-Length: ");	
		if(p1 != NULL)
		{
			p2 = strstr(p1, "\r\n");
			if(p2 != NULL)
			{
					char str_len[8] = {0,};
					memcpy(str_len, p1 + strlen("Content-Length: "), p2 - p1 - strlen("Content-Length: "));
					*cont_len = atoi(str_len);
					return 0;
			}
			
		}
	}	
	return 1;
}

u16 Http_Post_Head_Package(char *buff_post,char *url_tail,u8 *host, u16 port)
{
	u16 len;
	len = sprintf(buff_post, "POST %s HTTP/1.1\r\n"
			"Host:%s:%d\r\n"
			"User-Agent:GD32F427\r\n"
			"Accept: */*\r\n"
			"Connection:close\r\n"
			"Content-Type: multipart/form-data;boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
			"\r\n",
			url_tail,host,port
		);
	return len;
}

u16 Http_Post_Body_Package(char *buff_post,char *data, u16 data_len)
{
	u16 len;
	len = sprintf(buff_post, \
			"----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
			"Content-Disposition: form-data; name=\"image\"; filename=\"OIP-C.jpg\"\r\n"
			"Content-Type: image/jpeg\r\n"
			"\r\n"
				);
	memcpy(buff_post+len, data, data_len);
	sprintf(buff_post+len+data_len, "\r\n----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
			"\r\n"
			);
	return len+data_len;
}

u8 Http_Post_Analysis_Header(u8* buf, u16 buf_len, u8 *resp_code)
{
	char *body_cont;
	*resp_code=0;
	body_cont = strstr((char*)buf, "HTTP/1.1 200");	
	if(body_cont == NULL)
	{
		*resp_code = 200;
		return 0;
	}
	return 1;
}

static u32 g_httpWaitTim=0;
u8* g_httpResposeBuf=NULL;
static u32 g_http1STim=0;

void HTTP_TIM_10ms(void)
{
	if(hm609a_http_conn_flag&&g_httpWaitTim)g_httpWaitTim--;
	g_http1STim++;
	if(g_http1STim==500)
	{
		HTTP_FLAG_TASK=1;
		HTTP_FLAG_DOWNLOAD_BIN=1;
	}
}


void HTTP_Init(void)
{
	if(g_httpResposeBuf==NULL)
		g_httpResposeBuf=mymalloc(SRAMIN,1024);
}

void Http_Send_Resquest(const u8 sockid, const char *host,const u32 port)
{
	u16 len=0;
	char resquestBuf[400]={0};
	if(HTTP_FLAG_EQUIP_CERT)
	{
	}
	else if(HTTP_FLAG_DOWNLOAD_BIN)
	{
		printf("Send http get resquest\r\n");
		len=Http_Get_Package(resquestBuf, "iob/download/test.txt",host, port);
		HM609A_Send_Data( sockid,(const u8*)resquestBuf,len,1,HTTP_PROT);
		HTTP_FLAG_DOWNLOAD_BIN=0;
	}
	else if(HTTP_FLAG_UPLOAD_PHOTO)
	{
	}
	else if(HTTP_FLAG_UPLOAD_LOGFILE)
	{
	
	}
}

u8 HM609A_Http_Program(const u8 sockid, const char *host, const u32 port)
{
	u8 ret = 0;
	u16 i=0;
	if (hm609a_http_conn_flag)
	{
		i = USART1_Revice(g_httpResposeBuf);
		if(i){
			;
		}
		Http_Send_Resquest(sockid, host, port);
	}
	return 0;
}
