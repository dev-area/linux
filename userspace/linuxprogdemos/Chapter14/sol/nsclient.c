#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "nethdr.h"

int main(int argc, char *argv[])
{
    char buffer[MAXBUF+1];
    struct sockaddr_in serv;
    size_t addrlen;
    size_t servlen;
    struct hostent *host;
    int len;
    int sockfd;

    if (argc != 4) {
        fprintf (stderr, "Usage %s host filename\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd == -1)
    { 
        perror("client socket failed");
        exit(1);
    }

    /* connect socket to server */

    serv.sin_family = AF_INET;
    serv.sin_port = atoi(argv[2]);
    host = gethostbyname(argv[1]);
    if (!host)
    { 
        fprintf(stderr,"gethostbyname failed\n"); 
        exit(1); 
    }
    serv.sin_addr = *(struct in_addr*)host->h_addr;

    if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0)
    { 
        perror("client connect socket failed"); 
        exit(1); 
    }

    /* send filename */

    if (write(sockfd, argv[3], strlen(argv[3])) < 0)
    { 
        perror("client write filename failed"); 
        exit(1); 
    }
 
    /* read file data */

    while ((len=read(sockfd, buffer, MAXBUF)) > 0)
    {
        write(1,buffer,len);
    }

    if (len < 0)
    { 
        perror("client read failed"); 
        exit(1); 
    }

    return 0;
}
