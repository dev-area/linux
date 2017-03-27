#include <stdio.h>
#include <time.h>

/*
 * Measure the relative overhead of making a function call as
 * compared with making a system call.
 * Use the time() function, so all measurements will be in
 * seconds.
 */

int main ( int argc, char * argv[] )
{
    time_t before, after;
    int numIterations;
    int dummy();
    int i;
    double tfunc, tsyscall;

    /*
     * Sanity check that we have been called with an argument
     * that represents a meaningful number of iterations for
     * measurement
     */
    if ( argc < 2 ) {
	fprintf(stderr, "Usage %s numInterations\n", argv[0]);
	exit(1);
    }
    numIterations = atoi(argv[1]);
    if ( numIterations < 10000 ) {
	printf("%d is probably too small to give a meaningful answer\n",
	       numIterations);
	exit(1);
    }

    printf("Going to measure function call..."); fflush(stdout);

    if ( (before = time(&before)) < 0 ) {
	perror("time before");
	exit(2);
    }

    /* 
     * dummy() doesn't do anything of interest, just a simple
     * assignment and return, so it is functionally comparable to
     * the system call we are comparing with 
     */
    for ( i=0; i<numIterations; i++)
	dummy();

    if ( (after = time(&after)) < 0 ) {
	perror("time after");
	exit(2);
    }

    tfunc = difftime(after, before);
    printf("done\n");

    printf("Going to measure system call...");  fflush(stdout);

    if ( (before = time(&before)) < 0 ) {
	perror("time before");
	exit(2);
    }

    /* 
     * getpid() is the simplest system call - it does virtually nothing in
     * the kernel and so it gives us an idea of the actual overhead of 
     * system calls 
     */
    for ( i=0; i<numIterations; i++)
	getpid();

    if ( (after = time(&after)) < 0 ) {
	perror("time after");
	exit(2);
    }

    tsyscall = difftime(after, before);
    printf("done\n");

    printf("Time for %d function calls: %8.6lf seconds\n", numIterations, tfunc);
    printf("Time for %d system calls:   %8.6lf seconds\n", numIterations, tsyscall);

    exit(0);
}

int dummy()
{
    int x = 1;
    return x;
}
    
