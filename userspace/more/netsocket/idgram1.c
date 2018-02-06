/*
   udgram1.c  Chapter 14

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXBUF 256

int main()
{
  char buffer[MAXBUF];
  struct sockaddr_in addr = {0};
  size_t addrlen, n;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

  addrlen = sizeof(addr);
  n = recvfrom(sockfd, buffer, MAXBUF, 0,
                     (struct sockaddr*)&addr, &addrlen);
  buffer[n] = '\0';

  puts(buffer);
  
  printf("other port %d\n",addr.sin_port);
  
}
