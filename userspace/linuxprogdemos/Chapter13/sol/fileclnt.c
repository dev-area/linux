#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * File service client implemented using Unix domain sockets.
 */

#include "unixhdr.h"

int main(int argc, char *argv[])
{
    char buffer[MAXBUF+1];
    struct sockaddr_un addr;
    struct sockaddr_un serv;
    int addrlen;
    int servlen;
    int len;
    int sockfd;

    /*
     * Arg check, make sure we have a filename to ask for
     */
    if (argc != 2) {
        fprintf (stderr, "Usage %s filename\n", argv[0]);
        exit(1);
    }

    /*
     * Create socket
     */
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        perror("create socket");
        exit(1);
    }

    /*
     * Set up address of server socket that we will be connecting to
     */
    CLEAR(addr);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKSERV);

    /*
     * Make the connection. If it works, we have a channel to the server
     */
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { 
        perror("client connect socket failed"); 
        exit(1); 
    }

    /*
     * Write the filename through the socket to the server.
     */
    if ( (len = write(sockfd, argv[1], strlen(argv[1])) )  < 0) { 
        perror("client write"); 
        exit(1); 
    }

    /*
     * Read everything the server send us back and dump it to stdout.
     */
    while ((len=read(sockfd, buffer, MAXBUF)) > 0) {
        write(1,buffer,len);
    }
    if (len < 0) { 
        perror("client read failed"); 
        exit(1); 
    }

    return(0);
}
