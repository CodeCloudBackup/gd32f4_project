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

//��������
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
	int rc = 0;
	//sim7600_MQTTsend(buf,buflen);
	return rc;
}


/*
*   ����: ���ԭ����transport_getdata��������Ϊû���õ�socket
*   ˵����ʹ�÷�ʽ��ԭ����һ��=recv();
*   ����˵����
*       USART2_RX_BUF[]�Ǵ��ڽ��յĻ�����
*       read_buf_len �����Ƕ�����ʵ���ֽ���
*   ����ֵ���ǳ���Ҫ�����п�Դ���
*/
//��������,recv����������copy���ݣ������Ľ���������Э������ɵģ��� recv����������ʵ��copy���ֽ�����
int transport_getdata(unsigned char* buf, int count)
{
	int rc = 0;
  u16 len = 0;
  u8 *data = 0;
	while(1)
	{ 
//		if(UART7_RX_STA&0X8000)		//���յ�һ��������
//		{
//			memcpy(buf, (void*)&UART7_RX_BUF[read_buf_len], count);
//			read_buf_len += count;			
//			break;
//		}
		delay_ms(10);
	}
	return count;
}


//�����粢���ӷ�����
int transport_open(char* addr, int port)
{
	u8 *p;
	p=mymalloc(SRAMIN,10);//����50���ֽڵ��ڴ�
	
//	sim7600_LinkToNetWork();//������	
//	itoa(port,sport,10);
	sprintf((char*)p,"%d",port);
	printf("%s,%d",(char*)p,port);
//	sim7600_LinkToSever(addr,(char*)p);//���ӷ�����	
	
	myfree(SRAMIN,p); 
	return 0;
}

//�ر�����
int transport_close(int sock)
{
//	sim7600_close_network();//�ر�����	
	return 0;
}
