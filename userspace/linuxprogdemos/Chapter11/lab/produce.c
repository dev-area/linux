#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "sem.h"

int main ( int argc, char *argv[] )
{
    int sema4;
    int numItems = 0;
    char buf[16];

    if ( argc < 2 ) {
	fprintf(stderr, "Usage: %s initial-value\n", argv[0]);
	exit(1);
    }
    if ( (numItems = atoi(argv[1])) < 0 ) {
	fprintf(stderr, "Sorry, invalid initial value: %s\n", argv[1]);
	exit(1);
    }
    
    if ( ( sema4 = sem_create(ftok(getenv("HOME"), 1), numItems) ) < 0 ) {

	perror("Unable to create the semaphore");

    } else {

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
	    for ( ; numItems; numItems-- ) 
		sem_signal(sema4);
	}
    }

    sem_remove(sema4);
    exit(0);
}
