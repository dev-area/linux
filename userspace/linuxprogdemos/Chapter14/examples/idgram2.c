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

  if ( argc < 2 )
  {
    fprintf (stderr, "You must supply an address\n");
    exit (1);
  }

  /* The address structure contains the server address & port */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2000);
  
  /* Assume the user gave us an address as the argument */
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  /* We did not bind to a local address, a valid one will be
     automatically added to the datagram when it is sent. */
  n = sendto(sockfd, msg, strlen(msg), 0,
            (struct sockaddr*)&addr, sizeof(addr));

  return 0;
}
