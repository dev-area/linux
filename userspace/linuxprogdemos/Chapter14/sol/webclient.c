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
 * Client for (simplified) HTTP
 */

#include "web.h"

int main(int argc, char *argv[])
{
    char buffer[MAXBUF+1];
    struct sockaddr_in serv;
    size_t addrlen;
    size_t servlen;
    struct hostent *host;
    int len;
    int sockfd;
    int port;
    char *filename;

    /*
     * Args.
     * User must specify hostname. 
     * May specify port number (defaults to 80)
     * May specify filename (defaults to /index.html)
     */
    switch ( argc ) {
    case 2:
	port = SERVER_PORT;
	filename = "index.html";
	break;
    case 3:
	port = atoi(argv[2]);
	filename = "index.html";
	break;
    case 4:
	port = atoi(argv[2]);
	filename = argv[3];
	break;
    default:
	fprintf (stderr, "Usage %s host [port] [filename]\n", argv[0]);
	exit(1);
    }

    /*
     * Create a TCP socket
     */
    if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) { 
        perror("socket");
        exit(1);
    }

    /*
     * Make server address structure.
     */
    CLEAR(serv);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    if ( (host = gethostbyname(argv[1])) == NULL ) { 
        fprintf(stderr,"gethostbyname failed\n"); 
        exit(1); 
    }
    memcpy((char *)&serv.sin_addr, host->h_addr, host->h_length);

    /*
     * Connect to server
     */
    if (connect(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0) { 
        perror("client connect socket failed"); 
        exit(1); 
    }

    /*
     * Make HTTP request string and send it to the server
     */
    sprintf(buffer, "GET /%s HTTP/1.0\n\n", filename);
    if (write(sockfd, buffer, strlen(buffer)) < 0) { 
        perror("write"); 
        exit(1); 
    }
 
    /*
     * Dump whatever the server sends back to us to the screen
     */
    while ((len=read(sockfd, buffer, MAXBUF)) > 0)
        write(1,buffer,len);

    /*
     * Close down
     */
    close(sockfd);

    if (len < 0) { 
        perror(" read"); 
        exit(1); 
    }

    exit(0);
}
