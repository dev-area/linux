/*
 * tcpServerTask.c
 *
 *  Created on: May 15, 2010
 *      Author: root
 */
/*
 inetsrv.c  Chapter 14

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wait.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/prctl.h>
#include "app.h"

#define MAXBUF 			256
#define PORT 		2000
#define QUEUE_SIZE 		5


// fill the buffer to send with some test data
void fillBuffer(net_buffer_t *p_buffer)
{
	memset(p_buffer->buf,'X',NET_BUFFER_SIZE);
	memcpy(p_buffer->buf,"abcdefghijklmnopqrstuvwxyz",26);
	p_buffer->checksum = checksum(p_buffer->buf,NET_BUFFER_SIZE);
	p_buffer->len = NET_BUFFER_SIZE;
}


void tcpServerTask() {

	struct sockaddr_in addr;
	struct sockaddr_in client;
	unsigned int addrlen;
	int sockfd, commfd,i=0;
	struct timespec waitTime;
	net_buffer_t buf1;
	waitTime.tv_nsec =  10 * MILI_SECOND;
	waitTime.tv_sec = 0;

	puts("tcpServerTask started");
	prctl(PR_SET_NAME,"TcpServer");

	fillBuffer(&buf1);


	/* create and bind socket, set it up as a server */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
	listen(sockfd, QUEUE_SIZE);

	/* Accept connection and deal with request */
	memset(&client, 0, sizeof(client));
	addrlen = sizeof(client);
	puts("server ready");
	commfd = accept(sockfd, (struct sockaddr*) &client, &addrlen);
	if (commfd < 0) {
		perror("accept");
		exit(1); /* A real problem */
	}

	while (1) {
		buf1.msgNum = i++;
		nanosleep(&waitTime,NULL);
		write(commfd, &buf1, sizeof(net_buffer_t));
	}

	close(commfd);

}

