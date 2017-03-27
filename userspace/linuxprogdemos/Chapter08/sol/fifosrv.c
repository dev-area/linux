#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

/*
 * Simple "file" server, receives name of a file through one FIFO,
 * opens the file and sends its contents back through the other.
 */
#define REQFIFO "./fifo.req"
#define RESPFIFO "./fifo.resp"

#define MAXPATHLEN 1024
#define MAXBUF 1024

int reqFd, respFd;
int main ( int argc, char *argv[] )
{
    int fd, nRead;
    char fnameBuf[MAXPATHLEN], *fName = fnameBuf;
    char dataBuf[MAXBUF], *data = dataBuf;
    struct sigaction act;
    sigset_t mask;
    void shutdown();
    
    /*
     * Open or create the two FIFOs.
     */
    if ( mkfifo(REQFIFO, 0777) < 0 && errno != EEXIST ) {
	perror("REQFIFO");
	exit(1);
    }

    if ( mkfifo(RESPFIFO, 0777) < 0 && errno != EEXIST ) {
	perror("RESPFIFO");
	exit(1);
    }

    /*
     * Set up signal handling for the three termination signals:
     * SIGINT, SIGQUIT and SIGTERM
     */
    sigemptyset(&mask);
    act.sa_handler = shutdown;
    act.sa_mask = mask;
    act.sa_flags = 0;
    if ( sigaction(SIGINT, &act, NULL) < 0 ) {
	perror("sigaction for INT");
	exit(1);
    }
    if ( sigaction(SIGQUIT, &act, NULL) < 0 ) {
	perror("sigaction for QUIT");
	exit(1);
    }
    if ( sigaction(SIGTERM, &act, NULL) < 0 ) {
	perror("sigaction for TERM");
	exit(1);
    }
   
    /*
     * Main loop...
     */
    while ( 1 ) {

	/*
	 * Open FIFO to read request. Will block until a client
	 * opens the other end for writing.
	 */
	if ( (reqFd = open(REQFIFO, O_RDONLY)) < 0 ) {
	    perror(REQFIFO);
	    exit(1);
	}
    
	/*
	 * Read filename and ensure it is NULL terminated
	 */
	if ( (nRead = read(reqFd, fnameBuf, MAXPATHLEN)) < 0 ) {
	    perror("read of filename");
	    exit(1);
	}
	fnameBuf[nRead] = '\0';

	/*
	 * Open the other FIFO for writing, will block until client
	 * opens the other end for reading.
	 */
	if ( (respFd = open(RESPFIFO, O_WRONLY)) < 0 ) {
	    perror(RESPFIFO);
	    exit(1);
	}

	/*
	 * Open the file
	 */
	if ( (fd = open(fName, O_RDONLY)) < 0 ) {

	    /*
	     * Unable to open the file, send an appropriate message
	     * back through the FIFO to the client.
	     */
	    sprintf(data, "Unable to open file %s: %s\n", 
		    fName, strerror(errno));
	    if ( write(respFd, data, strlen(data)) < 0 ) {
		perror("write to fifo");
		exit(1);
	    }

	} else {

	    /*
	     * Send the file contents through the FIFO to the client
	     */
	    while ( (nRead = read(fd, dataBuf, MAXBUF)) > 0 ) 
		write(respFd, dataBuf, nRead);
	}

	/*
	 * Close the file, and the FIFOs
	 */
	close(fd);
	close(reqFd);
	close(respFd);
    }

    exit(0);
}


/*
 * Signal handler, tidy up and remove the FIFOs
 * before exiting.
 */
void shutdown ( int signo ) 
{
    close(reqFd);
    close(respFd);
    unlink(REQFIFO);
    unlink(RESPFIFO);
    exit(0);
}
