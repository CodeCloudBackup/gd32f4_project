/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/



#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "malloc.h"	 
#include "delay.h"
/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/

//发送数据
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
	int rc = 0;
	//sim7600_MQTTsend(buf,buflen);
	return rc;
}


/*
*   作用: 替代原来的transport_getdata函数，因为没有用到socket
*   说明：使用方式与原函数一致=recv();
*   参数说明：
*       USART2_RX_BUF[]是串口接收的缓存区
*       read_buf_len 串口是读到的实际字节数
*   返回值：非常重要！自行看源码吧
*/
//接收数据,recv函数仅仅是copy数据，真正的接收数据是协议来完成的）， recv函数返回其实际copy的字节数。
int transport_getdata(unsigned char* buf, int count)
{
	int rc = 0;
  u16 len = 0;
  u8 *data = 0;
	while(1)
	{ 
//		if(UART7_RX_STA&0X8000)		//接收到一次数据了
//		{
//			memcpy(buf, (void*)&UART7_RX_BUF[read_buf_len], count);
//			read_buf_len += count;			
//			break;
//		}
		delay_ms(10);
	}
	return count;
}


//打开网络并连接服务器
int transport_open(char* addr, int port)
{
	u8 *p;
	p=mymalloc(SRAMIN,10);//申请50个字节的内存
	
//	sim7600_LinkToNetWork();//打开网络	
//	itoa(port,sport,10);
	sprintf((char*)p,"%d",port);
	printf("%s,%d",(char*)p,port);
//	sim7600_LinkToSever(addr,(char*)p);//连接服务器	
	
	myfree(SRAMIN,p); 
	return 0;
}

//关闭网络
int transport_close(int sock)
{
//	sim7600_close_network();//关闭网络	
	return 0;
}
