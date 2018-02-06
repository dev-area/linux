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
  char buffer[100];
  size_t addrlen;
  struct sockaddr_un addr = {0};
  int n;
  int sockfd;
  
  sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
  
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "sock2");

  bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

 // memset(&addr, 0, sizeof (addr));
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "sock1");
  
  n = sendto(sockfd, msg, strlen(msg), 0,
            (struct sockaddr*)&addr, sizeof(addr));

  printf ( "%d bytes sent\n", n);
  
  sleep(1);
  
  n = recvfrom(sockfd,buffer,100,0,
         	(struct sockaddr*)&addr, &addrlen);

  buffer[n] = '\0';
  printf ("Data from server %s\n%s\n", addr.sun_path, buffer);

  unlink ("sock2");
  return 0;


}






