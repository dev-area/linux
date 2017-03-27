#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * Client of file service, implemented using a shared memory region 
 * controlled by 2 semaphores for IPC
 */
#include "sem.h"

int main ( int argc, char *argv[] )
{
    int ipcKey1 = ftok(getenv("HOME"), 1);
    int ipcKey2 = ftok(getenv("HOME"), 2);

    int clntSem, /* Says client has finished with the shared memory */
	srvSem;  /* Says server has finished with the shared memory */
    int shMem;   /* The id for the shared memory */
    char *shData;
    int pageSize, len;

    /*
     * Arg check, make sure we havebeen given a file to ask for
     */
    if ( argc < 2 ) {
	fprintf(stderr, "%s filename\n", argv[0]);
	exit(1);
    }

    /*
     * For efficiency make the shared memory region one page in size.
     * So find out what the page size is...
     */
    if ( (pageSize = sysconf(_SC_PAGESIZE)) < 0 ) {
	printf("Unable to discover page size, using 4K\n");
	pageSize = 4096;
    }

    /*
     * Open the shared memory and the semaphores.
     * Assume server has created them
     */
    if ( (clntSem = sem_open(ipcKey1)) < 0 ) {
	perror("Create request semaphore");
	exit(1);
    }
    if ( (srvSem = sem_open(ipcKey2)) < 0 ) {
	perror("Create request semaphore");
	exit(1);
    }

    if ( (shMem = shmget(ipcKey1, pageSize, 0)) < 0 ) {
	perror("Accessing shared memory segment");
	exit(1);
    }

    /*
     * Attach the shared memory segment. Make it appear as if it is
     * just a big buffer, so access it thru a char pointer.
     */    
    if ( (shData = (char *)shmat(shMem, 0, 0)) == (char *)-1 ) {
	perror("shmat");
	exit(1);
    }

    /*
     * Wait until the server lets us use the memory
     */
    sem_wait(srvSem);

    /*
     * Put the filename into the shared memory and tell the server it's there...
     */
    strcpy(shData, argv[1]);
    sem_signal(clntSem);
    
    /*
     * Take whatever the server sends us back and dump it to stdout
     */
    do {

	/*
	 * Wait until server tells us there's something there
	 */
	sem_wait(srvSem);
	len = strlen(shData);

	if ( len > 0 ) {

	    /* 
	     * If it's nonzero length, then print it
	     */
	    write(1, shData, len);

	    /*
	     * Tell server we have finished with the shared memory
	     */
	    sem_signal(clntSem);

	} else {

	    /*
	     * Zero length message means server has finished.
	     * We just have to "acknowledge" that we have read it.
	     */
	    sem_signal(clntSem);
	    break;
	}   

    } while ( len > 0 );

    exit(0);
}
