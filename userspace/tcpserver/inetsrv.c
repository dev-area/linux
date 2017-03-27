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

#define MAXBUF 256


int main()
{
  /* Note error handling not shown! */
  char buffer[MAXBUF];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  unsigned int addrlen, n;
  int sockfd, commfd;

  /* create and bind socket, set it up as a server */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd,(struct sockaddr*)&addr ,sizeof(addr));
  listen (sockfd, 5);

  for (;;)
  {
    /* Accept connection and deal with request */
    memset(&client, 0, sizeof (client));
    addrlen = sizeof(client);
    commfd = accept(sockfd,(struct sockaddr*)&client, &addrlen);
    if ( commfd < 0 )
    {
      if ( errno == EINTR )         /* Probably a child dying */
         continue;
      perror("accept");
      exit(1);                      /* A real problem */
    }


    n=read(commfd, buffer, MAXBUF); buffer[n] = '\0';
    printf ("Message; %s\n", buffer);
    write(commfd,"bye",4);
    close(commfd);
  }

  return 0;

}
