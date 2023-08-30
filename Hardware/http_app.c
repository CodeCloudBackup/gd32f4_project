#include "http_app.h"
#include "usart.h"
#include "hm609a.h"
#include "malloc.h"

u16 Http_Get_Package(char *buff_get, char *url_tail,u8 *host, u16 port)
{
	u16 len;
	len = sprintf(buff_get, "GET %s HTTP/1.1\r\n"
			"Connection:close\r\n"
			"Host:%s:%d\r\n"
			"Connection:Keep-Alive"
			"User-Agent:GD32F427\r\n"
			"\r\n",
			url_tail,host,port
		);
	return len;
}

u16 Http_Post_Head_Package(char *buff_post,char *url_tail,u8 *host, u16 port)
{
	u16 len;
	len = sprintf(buff_post, "POST %s HTTP/1.1\r\n"
			"Host:%s:%d\r\n"
			"User-Agent:GD32F427\r\n"
			"Accept: */*\r\n"
			"Connection:close\r\n"
			"Content-Type: multipart/form-data;boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n\r\n"
			"\r\n",
			url_tail,host,port
		);
	return len;
}

u16 Http_Post_Body_Package(char *buff_post,char *data)
{
	u16 len;
	len = sprintf(buff_post, \
			"----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
			"Content-Disposition: form-data; name=\"image\"; filename=\"OIP-C.jpg\"\r\n"
			"Content-Type: image/jpeg\r\n\r\n"
			"%s\r\n"
			"----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
			,data);
	return len;
}

u8 HM609A_Http_Program(char* addr, int port)
{

}