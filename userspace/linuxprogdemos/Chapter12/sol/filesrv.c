#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * File service implemented using shared memory region for IPC.
 * Needs 2 semaphores for controlling access to the shared memory.
 */
#include "sem.h"

int clntSem, /* Says client has finished with the shared memory */
    srvSem;  /* Says server has finished with the shared memory */

int shMem;   /* The id for the shared memory */

void cleanUp();

int main ( int argc, char *argv[] )
{
    int ipcKey1 = ftok(getenv("HOME"), 1);
    int ipcKey2 = ftok(getenv("HOME"), 2);
    char *shData;
    int pageSize, fd, nread;
    struct sigaction act;
    sigset_t mask;

    /*
     * Set up signal handling for tidying up when we finish
     */
    sigemptyset(&mask);
    act.sa_handler = cleanUp;
    act.sa_mask = mask;
    act.sa_flags = 0;
    if ( sigaction(SIGINT, &act, NULL) < 0 ) {
	perror("sigaction for INT");
	exit(1);
    }
    if ( sigaction(SIGQUIT, &act, NULL) < 0 ) {
	perror("sigaction for INT");
	exit(1);
    }
    if ( sigaction(SIGTERM, &act, NULL) < 0 ) {
	perror("sigaction for INT");
	exit(1);
    }

    /*
     * Most efficient if the mapped region is a multiple of page size,
     * so we make it 1 page. Find out what the page size is...
     */
    if ( (pageSize = sysconf(_SC_PAGESIZE)) < 0 ) {
	printf("Unable to discover page size, using 4K\n");
	pageSize = 4096;
    }

    /*
     * Open the semaphores and shared memory. Create them if necessary.
     */
    if ( (clntSem = sem_create(ipcKey1, 0)) < 0 ) {
	perror("Create request semaphore");
	exit(1);
    }
    if ( (srvSem = sem_create(ipcKey2, 0)) < 0 ) {
	perror("Create request semaphore");
	exit(1);
    }

    if ( (shMem = shmget(ipcKey1, pageSize, IPC_CREAT|S_IRWXU)) < 0 ) {
	perror("Creating shared memory segment");
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
     * Main loop
     */
    while ( 1 ) {
	
	/*
	 * Tell client it's OK to put something in the shared memory 
	 */
	sem_signal(srvSem);

	/*
	 * Now wait until client puts something there - it should be the
	 * name of the file we have to list.
	 */
	sem_wait(clntSem);
	printf("Message requesting file %s\n", shData); 

	/*
	 * Now open the file and send it back...
	 */
	if ( ( fd = open(shData, O_RDONLY)) < 0 ) {

	    /*
	     * Problem - put error message in the shared memory and signal
	     * client to read it...
	     */
	    strcat(shData, ": ");
	    strcat(shData, strerror(errno));
	    strcat(shData, "\n");

	    /*
	     * Tell client we've put something in the shared memory
	     */
	    sem_signal(srvSem);

	    /*
	     * Wait for client to tell us they have read it
	     */
	    sem_wait(clntSem);

	} else {

	    /*
	     * Read the file directly into the shared segment, a page at a time
	     */
	    while ( (nread = read(fd, shData, pageSize-1)) > 0 ) {

		/*
		 * Make it look like a string for the client's sake...
		 */
		*(shData+nread) = '\0';

		/*
		 * Tell client the data is there...
		 */
		sem_signal(srvSem);

		/*
		 * Wait for the client to tell us they have read it
		 */
		sem_wait(clntSem);
	    }
	    close(fd);

	}

	/*
	 * Empty "message" to signal end of conversation
	 */
	*shData = '\0';

	/*
	 * Tell client the data is there...
	 */
	sem_signal(srvSem);

	/*
	 * Wait for the client to tell us they have read it
	 */
	sem_wait(clntSem);
    }
}

/*
 * Tidy up - just remove the IPC bits...
 */
void cleanUp( int signo )
{
    shmctl(shMem, IPC_RMID, 0);
    sem_remove(clntSem);
    sem_remove(srvSem);
    exit(0);
}
