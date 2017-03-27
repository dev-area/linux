#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "sem.h"

int main ( int argc, char *argv[] )
{
    int sema4;
    int numItems = 0;
    char buf[16];

    if ( ( sema4 = sem_open(ftok(getenv("HOME"), 1) ) ) < 0 ) {
	    perror("Unable to open the semaphore");
    } else {
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
	    for ( ; numItems; numItems-- ) 
		sem_wait(sema4);
	}
    }

    exit(0);
}
