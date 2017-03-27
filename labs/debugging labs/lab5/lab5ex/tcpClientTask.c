/*
 * tcpClientTask.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
/*
   inetclt.c  Chapter 14

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#include "app.h"

#define MAXBUF 			(256)
#define SERVER_IP		("127.0.0.1")
#define SERVER_PORT		(2000)
#define DUMP_MSG		(1000)

/* tcp client task:
 * 		1. connect to server using ip address and port number
 * 		2. get messages
 * 		3. check data and dump each 500 messages
 */

void tcpClientTask()
{
  struct sockaddr_in addr = {0};
  int n,msgCount=0, sockfd;
  net_buffer_t netBuffer;

  puts("tcpClientTask started");
  prctl(PR_SET_NAME,"TcpClient");
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(SERVER_PORT);
  addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  sleep(3);

  connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  while(1)
  {
	  msgCount++;
	  n = read(sockfd, &netBuffer, sizeof(net_buffer_t));	/* Read reply */
	  if(n<=0) // error or server closed socket
	  {
		  perror("tcp:");
		  exit(0);
	  }
	  if(DUMP_MSG == msgCount)
	  {
		  printf("msg num:%d\n===========\n",netBuffer.msgNum);
		  puts((char *)netBuffer.buf);
		  if(checksum(netBuffer.buf,netBuffer.len) == netBuffer.checksum)
		  {
			  puts("message ok");
		  }
		  else
		  {
			  puts("error in checksum");
		  }
		  msgCount=0;
	  }
  }
}


