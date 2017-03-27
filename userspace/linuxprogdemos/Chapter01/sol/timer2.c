#include <stdio.h>
#include <sys/time.h>

/*
 * Measure the relative overhead of making a function call as
 * compared with making a system call.
 * This version uses gettimeofday() so we may get a more accurate
 * measurement
 */

int main ( int argc, char * argv[] )
{
    struct timeval before, after;
    int numIterations;
    int i;
    double tfunc, tsyscall;
    int dummy();
    double subtime(struct timeval *, struct timeval *);

    /*
     * Sanity check that we have been called with an argument
     * that represents a meaningful number of iterations for
     * measurement
     */
    if ( argc < 2 ) {
	fprintf(stderr, "Usage: %s numInterations\n", argv[0]);
	exit(1);
    }
    numIterations = atoi(argv[1]);
    if ( numIterations < 10000 ) {
	printf("%d is probably too small to give a meaningful answer\n",
	       numIterations);
	exit(1);
    }

    printf("Going to measure function call..."); fflush(stdout);

    if ( gettimeofday(&before, NULL) < 0 ) {
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

    if ( gettimeofday(&after, NULL) < 0 ) {
	perror("time after");
	exit(2);
    }

    tfunc = subtime(&after, &before);
    printf("done\n");

    printf("Going to measure system call...");  fflush(stdout);

    if ( gettimeofday(&before, NULL) < 0 ) {
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

    if ( gettimeofday(&after, NULL) < 0 ) {
	perror("time after");
	exit(2);
    }

    tsyscall = subtime(&after, &before);
    printf("done\n");

    printf("Time for %d function calls: %8.6lf seconds\n", numIterations, tfunc);
    printf("Time for %d system calls:   %8.6lf seconds\n", numIterations, tsyscall);

    exit(0);
}


/*
 * Dummy function call with minimal activity 
 */
int dummy()
{
    int x = 1;
    return x;
}
    
/*
 * Given two struct timevals (passed by address), return a double
 * that represents the difference between them 
 */
double subtime ( struct timeval *t1, struct timeval *t2 )
{
    long diffs = t1->tv_sec - t2->tv_sec;
    long diffus = t1->tv_usec - t2->tv_usec;

    if ( diffus < 0 ) {
	diffs--;
	diffus = (t1->tv_usec+1000000) - t2->tv_usec;
    }


    return (double) ( ((diffs * 1000000.0) + diffus) / 1000000.0 );
}
