#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
 * Consumer program to exercise semaphore functions, works in cooperation
 * with the producer program.
 * Assume that the semaphore has been created and initialised by the
 * producer program.
 * Repeatedly "consumes" instances of the resource by calling
 * "wait" the correct number of times on the semaphore.
 */
#include "sem.h"

int main ( int argc, char *argv[] )
{
    int sema4;
    int numItems = 0;
    char buf[16];

    /*
     * Open the semaphore - assume it has already been created
     */
    if ( ( sema4 = sem_open(ftok(getenv("HOME"), 1) ) ) < 0 ) {
	    perror("Unable to open the semaphore");

    } else {

	/*
	 * Ask the user to enter the number to consume, -1 to quit
	 */
	while ( numItems >=0 ) {
	    printf("Enter number to consume[%d]: ", sem_getval(sema4)); 
	    fflush(stdout);
	    if ( fgets(buf, 16, stdin) == NULL ) {
		numItems = -1;
		continue;
	    }
	    if ( (numItems = atoi(buf)) < 0 ) {
		continue;
	    }

	    /*
	     * Consume by calling wait the required number of times.
	     */
	    for ( ; numItems; numItems-- ) 
		sem_wait(sema4);
	}
    }

    exit(0);
}
