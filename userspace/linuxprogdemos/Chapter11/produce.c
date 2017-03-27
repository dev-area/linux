#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
 * Producer program to exercise semaphore functions.
 * Creates a semaphore, with an initial value representing an initial
 * amount of "resources".
 * Then repeatedly "creates" more instances of the resource by calling
 * "signal" the correct number of times on the semaphore.
 */
#include "sem.h"

int main ( int argc, char *argv[] )
{
    int sema4;
    int numItems = 0;
    char buf[16];

    /*
     * Command line argument is the initial number of resources in the pool
     */
    if ( argc < 2 ) {
	fprintf(stderr, "Usage: %s initial-value\n", argv[0]);
	exit(1);
    }
    if ( (numItems = atoi(argv[1])) < 0 ) {
	fprintf(stderr, "Sorry, invalid initial value: %s\n", argv[1]);
	exit(1);
    }
    
    /*
     * Create the semaphore and set its value to the initial count.
     */
    if ( ( sema4 = sem_create(ftok(getenv("HOME"), 1), numItems) ) < 0 ) {

	perror("Unable to create the semaphore");

    } else {

	/*
	 * Ask the user for a new number of resources to add into the pool
	 * If user enters -1, then quit.
	 */
	while ( numItems >=0 ) {

	    printf("Enter number to produce[%d]: ", sem_getval(sema4)); 
	    fflush(stdout);

	    if ( fgets(buf, 16, stdin) == NULL ) {
		numItems = -1;
		continue;
	    }
	    if ( (numItems = atoi(buf)) < 0 ) {
		continue;
	    }

	    /*
	     * Create the resources by calling sem_signal the right number
	     * of times.
	     */
	    for ( ; numItems; numItems-- ) 
		sem_signal(sema4);
	}
    }

    sem_remove(sema4);
    exit(0);
}
