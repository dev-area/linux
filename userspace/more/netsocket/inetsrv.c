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

#define MAXBUF 256

/* --------------------------------------------------------------- */

void CHLDhandler (int sig)
{
  while (waitpid(0,NULL,WNOHANG) > 0)
    {}
}

/* --------------------------------------------------------------- */

void TERMhandler (int sig)
{
  printf ("Shutting down\n");
  exit(0);
}

/* --------------------------------------------------------------- */

int main()
{
  /* Note error handling not shown! */
  char buffer[MAXBUF];
  struct sockaddr_in addr;
  struct sockaddr_in client;
  int addrlen, n;
  int sockfd, commfd;
  struct sigaction act;
  pid_t pid;

  /* Set up signal handling for SIGCHLD,
     SIGINT and SIGTERM */
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  act.sa_handler = CHLDhandler;
  sigaction(SIGCHLD, &act, NULL);

  act.sa_handler = TERMhandler;
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);

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

    /* each accepted client connection is 
       handled in separate sub-process */

    switch ( pid = fork() ) 
    {
      case -1:  /* Error, not shown here */
         break;
      case 0:
        close (sockfd);	/* Don't need listening socket now */
        n=read(commfd, buffer, MAXBUF); buffer[n] = '\0';
        printf ("Message; %s\n", buffer);
        /* Could send a reply here if required */
        exit(0);
      default:
        close (commfd);	/* Don't need connected socket */
    }
  }

  return 0;

}
