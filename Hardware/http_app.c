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
	len = sprintf(buff_get, "GET http://%s:%d/%s HTTP/1.1\r\n"
			"Connection:close\r\n"
			"Host:%s:%d\r\n"
			"Connection:close\r\n"
			"Accept: */*\r\n"
			"User-Agent:GD32F427\r\n"
			"\r\n",
			host,port,url_tail,host,port
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

u16 Http_Post_Head_Package(char *buff_post,char *url_tail,const char *host, u16 port,u8 *body, u32 body_len, CONTENT_TYPE content_type )
{
	u16 len;
	char contentLenStr[30];
	char contentDesc[100];
	char *boundaryStr = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
	len = sprintf(buff_post, "POST %s HTTP/1.1\r\n"
			"Host:%s:%d\r\n"
			"User-Agent:GD32F427\r\n"
			"Accept: */*\r\n"
			"Connection:close\r\n",
			url_tail,host,port
		);
	if (content_type == none)
	{
		;
	}
	else if (content_type == urlencoded)
	{
		strcat(buff_post, "Content-Type: application/x-www-form-urlencoded\r\n");
	}
	else if (content_type == form_data)
	{
		strcat(buff_post, "Content-Type: multipart/form-data; ");
		strcat(buff_post, "boundary=");
		strcat(buff_post, boundaryStr);
		strcat(buff_post, "\r\n");
	}
	sprintf(contentLenStr,"Content-Length: %d\r\n\r\n",body_len);
	strcat(buff_post, contentLenStr);
	
	// body
	len = strlen(buff_post);
	if(content_type == form_data)
	{
		sprintf(contentDesc,"%s\r\n "
			"Content-Disposition: form-data; name=\"image\"; filename=\"test.jpg\"\r\n"
			"Content-Type: image/jpeg\r\\n\r\n",
			boundaryStr
		);
		len = strlen(buff_post);
		memcpy(buff_post + len, body, body_len);
		strcat(buff_post, boundaryStr);
		strcat(buff_post, "\r\n\r\n");
		len = len + body_len + strlen(boundaryStr) + 4;
	}
	else
	{
		strcat(buff_post, (char *)body);
		strcat(buff_post, "\r\n\r\n");
		len = strlen(buff_post);
	}
	
	return len;
}

char cookie[100];

u8 Http_Post_Analysis_Header(u8* buf, u16 buf_len, u16 *resp_code)
{
	char *body_cont=NULL;
	char *session=NULL;
	char *end=NULL;
	char *body_content=NULL;
	*resp_code=0;
	body_cont = strstr((char*)buf, "HTTP/1.1 200");	
	if(body_cont == NULL)
	{
		*resp_code = 200;
		return 0;
	}
	
	session = strstr((char*)buf, "Set-Cookie: session=");
	if(session != NULL)
	{	
		end = strstr(session, ";");
		memcpy(cookie, session, end-session+1);
		printf("cookie:%s\r\n",cookie);
	}
	body_content = strstr((char*)buf, "\r\n\r\n");
	if(body_content != NULL){
		printf("body_content:%s",body_content);
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
	if(g_http1STim==1000)
	{
		HTTP_FLAG_TASK=1;
	//	HTTP_FLAG_DOWNLOAD_BIN=1;
		g_http1STim=0;
	}
}


void HTTP_Init(void)
{
	if(g_httpResposeBuf==NULL)
		g_httpResposeBuf=mymalloc(SRAMIN,1024);
	HTTP_FLAG_DOWNLOAD_BIN=0;
	HTTP_FLAG_UPLOAD_PHOTO=0;
	HTTP_FLAG_UPLOAD_LOGFILE=0;
	HTTP_FLAG_IDENT_SUCCESS=0;
	HTTP_FLAG_TASK=1;
	HTTP_FLAG_EQUIP_IDENT=1;
}

char *g_braCode="20230824900001";
char *g_chipId="20230824900001test";
char *md5="205e0dde2f9ffcb40594ba3276614378";
void Http_Send_Resquest(const u8 sockid, const char *host,const u32 port)
{
	u16 len=0;
	char resquestBuf[400]={0};
	if(!hm609a_http_conn_flag) return;
	
	if(HTTP_FLAG_EQUIP_IDENT)
	{
		char body[100]={0};
		u32 body_len=0;
		body_len=sprintf(body, "barCode=%s&chipId=%s&md5=%s",g_braCode,g_chipId,md5);
		printf("1.Send http post resquest:Equipment ident\r\n");
		len = Http_Post_Head_Package(resquestBuf, "/auth/20230908150000",\
						host,port,(u8*)body,body_len,urlencoded);
		printf("\r\n%s\r\n",resquestBuf);
		HM609A_Send_Data( sockid,(const u8*)resquestBuf,len,0,HTTP_PROT);
		HTTP_FLAG_EQUIP_IDENT=0;
		hm609a_http_wait_flag=1;
	}
	else if(HTTP_FLAG_DOWNLOAD_BIN)
	{
		printf("Send http get resquest\r\n");
		
		len=Http_Get_Package(resquestBuf, "iob/download/test.txt",host, port);
		printf("\r\n%s\r\n",resquestBuf);
		len = strlen(resquestBuf);
		printf("\r\nquest len%d\r\n",len);
		HM609A_Send_Data( sockid,(const u8*)resquestBuf,len,0,HTTP_PROT);
		HTTP_FLAG_DOWNLOAD_BIN=0;
		hm609a_http_wait_flag=1;
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
	if (hm609a_http_conn_flag && !hm609a_http_wait_flag)
	{
		Http_Send_Resquest(sockid, host, port);
	}
	return ret;
}
