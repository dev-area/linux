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
#include <string.h>
#include <sys/prctl.h>

#define MAXBUF 			256
#define SERVERPORT 		2000
#define QUEUE_SIZE 		5



void tcpServerTask() {
	/* Note error handling not shown! */
	char buffer[MAXBUF];
	struct sockaddr_in addr;
	struct sockaddr_in client;
	unsigned int addrlen, n;
	int sockfd, commfd;

	puts("tcpServerTask started");
	prctl(PR_SET_NAME,"TcpServer");

	/* create and bind socket, set it up as a server */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVERPORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
	listen(sockfd, QUEUE_SIZE);

	/* Accept connection and deal with request */
	memset(&client, 0, sizeof(client));
	addrlen = sizeof(client);
	commfd = accept(sockfd, (struct sockaddr*) &client, &addrlen);
	if (commfd < 0) {
		perror("accept");
		exit(1); /* A real problem */
	}

	while (1) {
		sleep(10);
		n = read(commfd, buffer, MAXBUF);
		buffer[n] = '\0';
		printf("Message; %s\n", buffer);
		write(commfd, "server: bye", 12);
	}

	close(commfd);

}

