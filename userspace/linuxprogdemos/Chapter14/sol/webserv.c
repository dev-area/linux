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
 * Simple web server
 */
#include "web.h"

/*
 * Strings used in the limited HTTP conversations we support
 */
char *httpOK = "HTTP/1.0 200 OK\n";
char *httpNotFound = "HTTP/1.0 404 Not Found\n";
char *contentHTML = "Content-Type: text/html\n\n";

/*
 * Files requested should be relative to this directory
 */
char *serverDocBase = ".";

int sockfd;

/*
 * Signal handlers
 */
void cleanup(int sig)
{
    close( sockfd );
    exit(1);
}

void CHLDhandler(int sig)
{
    pid_t pid;

    while ((pid=waitpid(0,NULL,WNOHANG)) > 0)
        {}
}


int main( int argc, char *argv[] )
{
    char buffer[MAXBUF+1];
    char *fName;
    char hostname[MAXHOSTNAMELEN+1];
    struct sockaddr_in addr;
    struct sockaddr_in client;
    size_t addrlen, clientlen;
    pid_t pid;
    int len;
    int fd, commfd;
    int fds[2];
    struct hostent *host;
    struct sigaction act;
    char *getFileName ( char * );

    /*
     * Set up signal handlers
     */
    sigemptyset(&act.sa_mask);
    act.sa_handler = CHLDhandler;
    if (sigaction(SIGCHLD,&act,NULL) < 0)
    {
        perror("install CHLD handler failed");
        exit(1);
    }

    act.sa_handler = cleanup;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("install INT handler failed");
        exit(1);
    }
    if (sigaction(SIGQUIT, &act, NULL) < 0) {
        perror("install QUIT handler failed");
        exit(1);
    }
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("install TERM handler failed");
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
     * Set up and bind to address.
     * Because we are not running as root, we can't use port 80.
     * 8000 (from the header file) is a good second bet.
     */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if ( bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { 
        perror("bind"); 
        exit(1); 
    }

    /*
     * Enable as a server
     */
    if (listen(sockfd,5) < 0) { 
        perror("listen"); 
        exit(1); 
    }
 
    /*
     * Main loop, process requests from client...
     */
    for (;;) {

	/*
	 * accept() will return client address
	 */
	CLEAR(client);
        clientlen = sizeof(client);
	while ( (commfd = accept(sockfd,(struct sockaddr*)&client,&clientlen)) < 0 ) {
            if (errno != EINTR) { 
                perror("accept failed"); 
                exit(1); 
            }
        }

	/*
	 * Handle each client in a child process
	 */
	switch ( pid=fork() ) {
	case -1:
	    perror("Fork failed");
	    exit(1);

	case 0:

	    /* 
	     * Child - dont need main socket any more.
	     */
	    close (sockfd);

	    /*
	     * Log details of who has connected
	     */
	    host = gethostbyaddr ((char*)&client.sin_addr,
				  sizeof(struct in_addr), AF_INET);
            if ( host ) 
		printf("Connection from: %s, Port %d\n", host->h_name,
							ntohs(client.sin_port) );
            else {
		struct in_addr tmp;

		tmp.s_addr = ntohl(client.sin_addr.s_addr);
	    	printf("Connection from: %s, Port: %d\n", 
		   		inet_ntoa(tmp), ntohs(client.sin_port) );
            }

	    if ( (len=read(commfd, buffer, MAXBUF) ) < 0) {
		perror("read request");
		break;
	    }
	    buffer[len] = '\0';
	    
	    /*
	     * Find out which file they want by extracting the name from the
	     * "GET..." string.
	     */
	    fName = getFileName(buffer);
	    printf("Request for %s...", fName);

	    /*
	     * Try and open it
	     */
	    fd = open(fName, O_RDONLY);
	    if (fd >= 0)  {

		/*
		 * Successful open, send back HTTP header and then the file
		 * itself.
		 */
		if ( sendHTMLFile(fd, commfd) < 0 ) 
		    printf("Error sending file\n");
		else
		    printf("OK\n");
		close (fd);

	    }  else {

		/*
		 * Can't open the file, send back a failure notice that
		 * a browser can understand
		 */
		printf("unable to open file...");
		if ( sendNotFound(fName, commfd) < 0 ) 
		    printf("Error sending message\n");
		else
		    printf("message sent OK\n");
		close (fd);
		
	    }

	    close (commfd);
	    exit(0);

	    default:
		close (commfd);
	    }
    }

    exit(0);
}

/*
 * Extract the name portion of an HTTP "GET" request.
 * Return path to the file relative to the server's document base directory
 */
char *getFileName ( char *request ) 
{
    static char fileNameBuf[MAXBUF+1], *fn = fileNameBuf;
    char *cp1 = request, *cp2;

    while ( *cp1 != '/' ) cp1++;
    cp2 = cp1; while ( *cp2 != ' ' ) cp2++;
    *cp2 = '\0';
    strcpy(fileNameBuf, serverDocBase);
    strcat(fileNameBuf, cp1);

    return ( fn );
}

/*
 * Send back contents of the the file <fileFd> through the socket <socket>.
 * Precede the file stuff with the HTTP goodies necessary to keep a web
 * browser happy
 */
int sendHTMLFile ( int fileFd, int socket ) 
{
    char *httpOK = "HTTP/1.0 200 OK\n";
    char *contentHTML = "Content-Type: text/html\n\n";
    int OKlen = strlen(httpOK);
    int contentlen = strlen(contentHTML);
    char dataBuf[MAXBUF];
    int len;

    /*
     * HTTP preamble
     */
    if ( write(socket, httpOK, OKlen) < OKlen ) {
	perror("write");
	return(-1);
    }
    if ( write(socket, contentHTML, contentlen) < contentlen ) {
	perror("write");
	return(-1);
    }

    /*
     * Now the contents of the file
     */
    while ( (len=read(fileFd, dataBuf, MAXBUF) ) > 0) {
	if (write(socket, dataBuf, len) < 0) {
	    perror("write");
	    return(-1);
	}
    }
    return (0);

}


/*
 * Send back an HTTP "Not found" message 
 * for filename <fn>, through socket <socket>.
 */
int sendNotFound ( char *fn, int socket ) 
{
    char *httpNotFound = "HTTP/1.0 404 Not Found\n";
    char *contentHTML = "Content-Type: text/html\n\n";
    int nfLen = strlen(httpNotFound);
    int contentlen = strlen(contentHTML);
    char dataBuf[MAXBUF];

    /*
     * The HTTP preamble
     */
    if ( write(socket, httpNotFound, nfLen) < nfLen ) {
	perror("write");
	return(-1);
    }
    if ( write(socket, contentHTML, contentlen) < contentlen ) {
	perror("write");
	return(-1);
    }

    /*
     * An HTTP message saying that the filename cannot be found.
     * Makes it easier for browsers.
     */
    sprintf(dataBuf, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD><BODY>%s was not found</BODY></HTML>\n", fn);
    if ( write(socket, dataBuf, strlen(dataBuf)) < strlen(dataBuf)) {
	perror("write");
	return(-1);
    }

    return(0);
}
