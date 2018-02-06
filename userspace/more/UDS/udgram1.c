/*
   udgram1.c  Chapter 13

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>   
#include <sys/un.h>

#define MAXBUF 2048

int main()
{
  char buffer[MAXBUF+1];
  struct sockaddr_un addr = {0};
  size_t addrlen, n;
  int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "sock1");

  bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  memset(&addr, 0, sizeof (addr));
  addrlen = sizeof(addr);
  while(1){
  	n = recvfrom(sockfd,buffer,MAXBUF,0,
         	(struct sockaddr*)&addr, &addrlen);

  	buffer[n] = '\0';
  	printf ("Data from client %s\n%s\n", addr.sun_path, buffer);
  	sleep();
  	n = sendto(sockfd, "ok", 2, 0,
            (struct sockaddr*)&addr, sizeof(addr));

  }
  unlink ("sock1");		/* delete socket */

  return 0;
}








