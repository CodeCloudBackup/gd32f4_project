#include "http_app.h"
#include "usart.h"
#include "hm609a.h"
#include "malloc.h"
#include <stdlib.h>

Byte8 HttpReqFlag;
HTTP_REQ_ENUM g_nHttpReq;
HTTP_CMD_STA g_sHttpCmdSta;
u16 g_httpReturnTim=0;


u16 Http_Get_Package(char *buff_get, char *url_tail,const char *host, u16 port)
{
	u16 len;
	len = sprintf(buff_get, "GET %s HTTP/1.1\r\n"
			"Connection:close\r\n"
			"Host:101.37.89.157:80\r\n"
			"Connection:close\r\n"
			"Accept: */*\r\n"
			"User-Agent:GD32F427\r\n"
			"\r\n",url_tail
			//host,port,url_tail,host,port
		);
	return len;
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
			"Connection: close\r\n",
			url_tail,host,port
		);
	if (content_type == NONE)
	{
		;
	}
	else if (content_type == URLENCODED)
	{
		strcat(buff_post, "Content-Type: application/x-www-form-urlencoded\r\n");
	}
	else if (content_type == FORM_DATA)
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
	if(content_type == FORM_DATA)
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

char cookie[50]={0};
u16 Http_Response_Analysis(u8* buf, u16 buf_len, u8* content,u32 *cont_len)
{	
	
	u16 resp_code=0;
	*cont_len=0;
	if(strstr((char*)buf, "\r\n\r\n") != NULL)//response header ok
	{
		char *p, *p1, *p2,*p3;
		char *session=NULL;
		char *end=NULL;
		p1 = strstr((char*)buf, "HTTP/1.1 200");
		p  = strstr((char*)buf, "HTTP/1.1 ");	
		if(p1 != NULL)// io stream
		{
			resp_code = 200;
		}else {
			resp_code=atoi(p);
		}		

		{
			session = strstr((char*)buf, "Set-Cookie: session=");
			if(session != NULL)
			{	
				end = strstr(session, ";");
				memcpy(cookie, session, end-session+1);
				printf("cookie:%s\r\n",cookie);
			}
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
				if(content == NULL)
					content=mymalloc(SRAMIN, *cont_len+1);
				p3 = strstr(p2, "\r\n\r\n");
				memcpy(content, p3+4, *cont_len);
				content[*cont_len]='\0';
			}
			
		}
	}	
	return resp_code;
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
		if(g_sHttpCmdSta.sta_download_bin==1 || g_sHttpCmdSta.sta_equip_ident==1)
			g_sHttpCmdSta.sta_cmd=1;
		if(g_sHttpCmdSta.sta_upload_photo==1 || g_sHttpCmdSta.sta_upload_logfile==1)
			g_sHttpCmdSta.sta_cmd=1;
		g_http1STim=0;
	}
}


void HTTP_Init(void)
{
	if(g_httpResposeBuf==NULL)
		g_httpResposeBuf=mymalloc(SRAMIN,1024);
	memset(&g_sHttpCmdSta, 0 ,sizeof(g_sHttpCmdSta));
	g_sHttpCmdSta.sta_cmd=0;
	g_sHttpCmdSta.sta_equip_ident=0;
}

char *g_braCode="20230824900001";
char *g_chipId="20230824900001test";
char *md5="205e0dde2f9ffcb40594ba3276614378";
char *g_cookie=NULL;
extern u8* g_netData;
void Http_Send_Resquest(const u8 sockid, const char *host,const u32 port)
{
	u16 len=0;
	char resquestBuf[400]={0};
	
	if(g_sHttpCmdSta.sta_equip_ident == 1)
	{
		char body[100]={0};
		u32 body_len=0;
		if(g_netData==NULL)
			g_netData=mymalloc(SRAMIN, 600);
		body_len=sprintf(body, "barCode=%s&chipId=%s&md5=%s",g_braCode,g_chipId,md5);
		printf("1.Send http post resquest:Equipment ident\r\n");
		len = Http_Post_Head_Package(resquestBuf, "/auth/20230908150000/",\
						host,port,(u8*)body,body_len,URLENCODED);
		printf("\r\n%s\r\n",resquestBuf);
		resquestBuf[len]='\0';
		HM609A_Send_Data( sockid,(const u8*)resquestBuf,len+1,1,HTTP_PROT);
		hm609a_http_wait_flag=1;
	}
	else if(g_sHttpCmdSta.sta_download_bin == 1)
	{
		printf("Send http get resquest\r\n");
		if(g_netData==NULL)
			g_netData=mymalloc(SRAMIN, 60*1024);
		len=Http_Get_Package(resquestBuf, "iob/download/test.txt",host, port);
		printf("\r\n%s\r\n",resquestBuf);
		len = strlen(resquestBuf);
		printf("\r\nquest len%d\r\n",len);
		HM609A_Send_Data( sockid,(const u8*)resquestBuf,len,0,HTTP_PROT);
		hm609a_http_wait_flag=1;
	}
	else if(g_sHttpCmdSta.sta_upload_photo == 1)
	{
		//hm609a_http_wait_flag=1;
	}
	else if(g_sHttpCmdSta.sta_upload_logfile == 1)
	{
		hm609a_http_wait_flag=1;
	}
}
extern char *g_httpRes;
extern char *g_mqttRes;
u32 HTTP_Recvice(u8* buf, u32 buflen)
{
	u32 len=g_usart1Cnt; 
	u32 size=0;
	char *ptr=NULL;
	if(!hm609a_http_conn_flag) return 0;
	if(g_usart1RevFinish)
	{
		if(len > 0)
		{	
			ptr = strstr((const char *)USART1_RX_BUF, g_mqttRes);
			if(ptr)
				return 0;
			ptr = strstr((const char *)USART1_RX_BUF, g_httpRes);
			if(ptr != NULL)
			{	
				printf("Http Recv:%s\r\n",USART1_RX_BUF);
				memcpy(buf, USART1_RX_BUF, len);
				
				USART1_Clear();
				return len;	
			}
		}
		else
		{
			USART1_Clear();
			return 0;
		}	
	}
	return 0;
}
