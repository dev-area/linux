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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/prctl.h>

#define MAXBUF 256

void tcpClientTask()
{
  char msg[MAXBUF];
  struct sockaddr_in addr = {0};
  int n, sockfd;

  puts("tcpClientTask started");
  prctl(PR_SET_NAME,"TcpClient");
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  sprintf (msg, "Test message from client %d", getpid());
  while(1)
  {
	  n = write(sockfd, msg, strlen(msg));	/* Send message */
	  n = read(sockfd, msg, MAXBUF);	/* Read reply */
	  msg[n] = '\0';
	  printf ("client:  %s\n", msg);
  }
}


