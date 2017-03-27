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
 * Server for the remote "who" service.
 * Accept connections from clients and then send back the output from
 * running the who command.
 */
#include "nswho.h"

int sockfd;

/*
 * Handler to clean up on termination
 */
void cleanup(int sig)
{
    close( sockfd );
    exit(1);
}

/*
 * Handler to clean up after child processes
 */
void CHLDhandler(int sig)
{
    pid_t pid;

    while ((pid=waitpid(0,NULL,WNOHANG)) > 0)
        {}
}

int main( int argc, char *argv[] )
{
    char buffer[MAXBUF+1];
    struct sockaddr_in addr;
    struct sockaddr_in client;
    size_t addrlen;
    size_t clientlen;
    pid_t pid;
    int fd;
    int len;
    int commfd;
    int fds[2];
    struct hostent *host;
    struct sigaction act;

    /*
     * Set up signal handlers
     */
    sigemptyset(&act.sa_mask);
    act.sa_handler = CHLDhandler;
    if (sigaction(SIGCHLD, &act, NULL) < 0) {
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
     * Open a TCP socket
     */
    if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) { 
        perror("socket");
        exit(1);
    }

    /*
     * Set up and bind address for socket.
     * Port number must be in network byte order.
     * INADDR_ANY means listen on any valid IP address for this host.
     */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { 
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
     * Main loop, accept requests and process them
     */
    for (;;)  {

	/*
	 * accept() will return details of client address when a connection 
	 * is made.
	 */
	CLEAR(client);
        clientlen = sizeof(client);
	while ( (commfd = accept(sockfd, (struct sockaddr*)&client, &clientlen)) < 0 ) {
            if (errno != EINTR) { 
		/*
		 * This means a child process has finished, we just need to
		 * tidy up and go back to wait for another client.
		 */
                perror("accept"); 
                exit(1); 
            }
        }

	/*
	 * Each request is to be handled in a child process.
	 */
	switch (pid=fork()) {
	case -1:
	    perror("Fork failed");
	    exit(1);

	case 0:

	    /*
	     * Child, close the main socket
	     */
	    close (sockfd);

	    /*
	     * Report who the client is.
	     */
	    host = gethostbyaddr ((char*)&client.sin_addr,
                               sizeof(struct in_addr), AF_INET);
	    printf("Request from host: %s, port: %d\n", 
		   ( host ? host->h_name : inet_ntoa(client.sin_addr) ),
		   ntohs(client.sin_port) );

	    /*
	     * Do the I/O redirection and then exec() the who command
	     */
	    dup2(commfd,2);
	    dup2(commfd,1);
	    close(commfd);

	    execlp ("who", "who", NULL);
	    perror ("exec who failed");
	    exit (1);	

	default:

	    /*
	     * Parent, close the connected socket and go back to 
	     * wait for another connection
	     */
	    close(commfd);
	}

    }
    return 0;
}
