#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

/*
 * Simple file service client.
 * Send filename to client, get contents back
 */
#define REQFIFO "./fifo.req"
#define RESPFIFO "./fifo.resp"

#define MAXBUF 256

int main ( int argc, char *argv[] )
{
    int reqFd, respFd, nRead;
    char dataBuf[MAXBUF];
    
    /*
     * Check arguments
     */
    if ( argc < 2 ) {
	fprintf(stderr, "Usage: %s filename\n", argv[0]);
	exit(1);
    }

    /*
     * Open FIFO for writing, will block until server opens the
     * other end for reading.
     */
    if ( (reqFd = open(REQFIFO, O_WRONLY)) < 0 ) {
	perror(REQFIFO);
	exit(1);
    }
    
    /*
     * Send filename through the FIFO, then close it as we dont need
     * it any more...
     */
    if ( write(reqFd, argv[1], strlen(argv[1])) < 0 ) {
	perror("write to FIFO");
	exit(1);
    }
    close(reqFd);

    /*
     * Open the other FIFO for reading, this will block until the 
     * server opens the other end for writing.
     */
    if ( (respFd = open(RESPFIFO, O_RDONLY)) < 0 ) {
	perror(RESPFIFO);
	exit(1);
    }

    /*
     * Dump everything the server sends to us to stdout
     */
    while ( (nRead = read(respFd, dataBuf, MAXBUF)) > 0 ) 
	write(1, dataBuf, nRead);

    close(respFd);

    exit(0);
}
	 
