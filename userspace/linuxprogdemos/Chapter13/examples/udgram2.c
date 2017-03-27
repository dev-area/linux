/*
   udgram2.c  Chapter 13

*/

#include <sys/types.h>
#include <sys/socket.h>   
#include <sys/un.h>

#define MAXBUF 2048

int main()
{
  char *msg="Test message from process 2";
  struct sockaddr_un addr = {0};
  int n;
  int sockfd;
  
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  
  addr.sun_family = AF_UNIX;
  
  strcpy(addr.sun_path, "sock2");
  if ( bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) == -1 )
  {
     perror ("bind");
     exit (1);
  }
  
  memset(&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "sock1");
  
  n = sendto(sockfd, msg, strlen(msg), 0,
            (struct sockaddr*)&addr, sizeof(addr));
  unlink ("sock2");		/* delete socket */

  printf ( "%d bytes sent\n", n);
  return 0;

}
