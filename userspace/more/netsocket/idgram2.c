/*
   idgram2.c  Chapter 14

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char *argv[])
{
  struct sockaddr_in addr = {0};
  int n;
  char *msg = "Test message from process 2";
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  n = sendto(sockfd, msg, strlen(msg), 0,
            (struct sockaddr*)&addr, sizeof(addr));

  return 0;
}
