#include "http_app.h"
#include "usart.h"
#include "hm609a.h"
#include "malloc.h"
#include <stdlib.h>

Byte8 HttpFlag;

u16 g_httpReturnTim=0;

void HTTP_TIM_1ms(void)
{
	if(hm609a_http_return_flag)
	{
		if(g_httpReturnTim)g_httpReturnTim--;
	}
}

u16 Http_Get_Package(char *buff_get, char *url_tail,u8 *host, u16 port)
{
	u16 len;
	len = sprintf(buff_get, "GET %s HTTP/1.1\r\n"
			"Connection:close\r\n"
			"Host:%s:%d\r\n"
			"Connection:Keep-Alive\r\n"
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
	


u8 HM609A_Http_Program(u8 sockid, char* addr, u32 port, u32 upload_port)
{
	u8 err = 0;
	u32 tmpPort=0;
	static u8 count=0;
	static u8 state=0;
	if(!hm609a_connect_flag)return 0;
	// 确认连接端口
	if(HTTP_FLAG_UPLOAD_PHOTO||HTTP_FLAG_UPLOAD_LOGFILE) 
	{
		tmpPort=upload_port;
	} else if(HTTP_FLAG_EQUIP_CERT||HTTP_FLAG_DOWNLOAD_BIN)
	{
		tmpPort=port;
	} else {
		return 0;
	}
	// http的tcp连接
	if(!hm609a_http_conn_flag){
		switch(state)
		{
				case 0:
				{
					err = HM609A_Connect(sockid, addr, tmpPort);
					switch(err)
					{
						 case 0:break; 		//正常流程,直接跳出
						 case 1:
						 {
							 //连接成功,跳到下一个流程
							 count=0;
							 hm609a_http_conn_flag=1;//标记连接成功
							 state+=10;
						 }
						 break;
						 default:
						 {
								//进入失败
								if(count>=3)
								{
									//超过重试次数，重启模块
									count=0;
									state=0;
									HM609A_Restart();
								}
								else
								{      
									count++;//重试次数+1
									state++;
								}
							}
							break;
					}
				}
				break;
				case 1:
				{
					if(HM609A_Tcp_Off(sockid))
						state--;//关闭连接后重新连接
				}
				break;
				default://TCP连接成功，开始数据收发
				{
						if(hm609a_http_return_flag&&g_httpReturnTim<=0)
						{
							hm609a_http_conn_flag=0;
							hm609a_http_return_flag=0;
						}
						if(!hm609a_http_conn_flag)
							state=0;
				}
				break;
			}
	}else{
		;
	}
	return 0;
}
