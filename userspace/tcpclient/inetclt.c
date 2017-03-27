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

#define MAXBUF 256

int main(int argc, char *argv[])
{
  char msg[MAXBUF];
  struct sockaddr_in addr = {0};
  int n, sockfd;

  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = inet_addr("192.168.0.1");

  connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  sprintf (msg, "Test message from client %d", getpid());
  n = write(sockfd, msg, strlen(msg));	/* Send message */
  n = read(sockfd, msg, MAXBUF);	/* Read reply */
  msg[n] = '\0';
  printf ("Answer: %s\n", msg);

  return 0;
}
