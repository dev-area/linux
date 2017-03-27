/*
   unixclt.c  Chapter 13

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>   
#include <sys/un.h>

#define MAXBUF 2048

int main()
{
  char msg[MAXBUF];
  struct sockaddr_un addr;
  int n, sockfd;

  /* Create socket and connect to server */
  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  
  memset(&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  
  strcpy(addr.sun_path, "server");
  connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));

  sprintf (msg, "Test message from client %d", getpid());
  
  n=write(sockfd, msg, strlen(msg));	/* Send message */
  n=read(sockfd, msg, MAXBUF);	/* Read reply */
  
  msg[n] = '\0';
  printf ("Answer: %s\n", msg);

  return 0;

}
