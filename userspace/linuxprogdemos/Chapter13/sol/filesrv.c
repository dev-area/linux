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

#include "unixhdr.h"

/*
 * Cleanup function for terminating
 */
void cleanup()
{
    unlink (SOCKSERV);
    exit(1);
}

/*
 * Cleanup function called as signal handler.
 */
void cleanup_sig(int sig)
{
    unlink (SOCKSERV);
    exit(1);
}

/*
 * Handler function called when a child process terminates
 */
void CHLDhandler(int sig)
{
    pid_t pid;

    while ((pid=waitpid(0,NULL,WNOHANG)) > 0) {

    }
}

int main()
{
    char buffer[MAXBUF+1];
    struct sockaddr_un addr;
    struct sockaddr_un client;
    size_t addrlen;
    size_t clientlen;
    pid_t pid;
    int fd;
    int len;
    int sockfd;
    int commfd;
    struct sigaction act;

    /*
     * Set up signal handling
     */
    sigemptyset(&act.sa_mask);
    act.sa_handler = cleanup_sig;
    if (sigaction(SIGTERM,&act,NULL) < 0) {
        perror("install TERM handler failed");
        exit(1);
    }
    act.sa_handler = cleanup_sig;
    if (sigaction(SIGINT,&act,NULL) < 0) {
        perror("install INT handler failed");
        exit(1);
    }
    act.sa_handler = cleanup_sig;
    if (sigaction(SIGQUIT,&act,NULL) < 0) {
        perror("install QUIT handler failed");
        exit(1);
    }

    act.sa_handler = CHLDhandler;
    if (sigaction(SIGCHLD,&act,NULL) < 0) {
        perror("install CHLD handler failed");
        exit(1);
    }

    /*
     * Create the socket
     */
    if ( (sockfd = socket(AF_UNIX,SOCK_STREAM,0)) == -1) { 
        perror("create server socket");
        exit(1);
    }

    /*
     * Set up address and bind it to the socket
     */
    CLEAR(addr);
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKSERV);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { 
        perror("bind"); 
        cleanup(); 
    }

    /*
     * Enable socket as server
     */
    if (listen(sockfd,5) < 0) { 
        perror("listen"); 
        cleanup(); 
    }

    /*
     * Main loop: wait for client to connect, then deal with it 
     */
    for (;;) {

	/*
	 * Initialise address structure to hold client's address. 
	 * accept() will block until a client appears.
	 */
        CLEAR(client);
        clientlen = sizeof(client);
        while( (commfd = accept(sockfd, (struct sockaddr*)&client, &clientlen)) < 0 ) {
	    if ( errno != EINTR ) {
		perror("accept");
		cleanup();
	    }
	}

	/*
	 * Handle the request in a child process
	 */
	switch (pid=fork()) {
	case -1: 
	    perror("Fork failed");
	    cleanup();

	case 0:

	    /*
	     * Child - doesn't need the main socket any more
	     */
	    close (sockfd);

	    /*
	     * Read the filename through the connected socket
	     */
	    if ( (len = read(commfd, buffer, MAXBUF) ) < 0) {
		perror ("filename read");
		exit(1);
	    }
	    buffer[len] = '\0';

	    printf ("Request for <%s>\n", buffer);

	    /*
	     * Dump file back through the connected socket, or send a message
	     * if there's a problem.
	     */
	    fd = open(buffer, O_RDONLY);
	    if (fd >= 0)  {
		while ((len=read(fd,buffer,MAXBUF)) > 0) {
		    if (write(commfd, buffer, len) < 0) { 
			perror("server write failed"); 
			exit(1); 
		    }
		}
		close (fd);
	    }  else  {
		strcat (buffer, ": cannot open: ");
		strcat (buffer, strerror(errno));
		strcat (buffer, "\n");
		if (write(commfd, buffer, strlen(buffer)) < 0) { 
		    perror("server write failed"); 
		    exit(1); 
		}
	    }

	    /*
	     * Finished, close socket so client knows there's no more data
	     */
	    close (commfd);
	    exit(0);
 
	default:

	    /*
	     * Parent - dont need the connected socket here.
	     * Close it and go back to wait for the next client.
	     */
	    close (commfd);
	}
    }

    exit(0);
    
}
