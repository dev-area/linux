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

/*
 * Client for remote "who" application.
 * Fairly generic client operation - make connection to server
 * and then display whatever the server sends back to us.
 */
#include "nswho.h"

int main(int argc, char *argv[])
{
    char buffer[MAXBUF+1];
    struct sockaddr_in serv;
    size_t addrlen;
    size_t servlen;
    struct hostent *host;
    int len;
    int sockfd;

    /*
     * Arg check, user should provide name of server host
     */
    if (argc != 2) {
        fprintf (stderr, "Usage %s host\n", argv[0]);
        exit(1);
    }

    /*
     * Create the socket, TCP requires a stream socket in the Internet domain
     */
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0 ) { 
        perror("socket");
        exit(1);
    }

    /*
     * Build server address structure for connection
     * Port number must be in network byte order.
     */
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);

    /*
     * Look up server address
     */
    host = gethostbyname(argv[1]);
    if ( (host = gethostbyname(argv[1])) == NULL ) { 
        perror("gethostbyname"); 
        exit(1); 
    }

    /*
     * gethostbyname() returns address in network byte order, to preserve
     * this use memcpy() rather than a simple assignment
     */
    memcpy((char *)&serv.sin_addr, host->h_addr, host->h_length);

    /*
     * Now make the connection. If this works we have a channel to
     * the server.
     */
    if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0) { 
        perror("connect"); 
        exit(1); 
    }

    /*
     * Server should immediately begin sending the output from the who command
     * back to us, we simply display it on stdout
     */
    while ((len=read(sockfd, buffer, MAXBUF)) > 0) 
        write(1,buffer,len);

    if (len < 0) { 
        perror("read"); 
        exit(1); 
    }
    close(sockfd);

    exit(0);
}
