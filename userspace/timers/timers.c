#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define LOOP_LIMIT  1E12

volatile int sigcount=0;

void catcher( int sig ) {

    struct itimerval value;
    int which = ITIMER_REAL;

    printf( "Signal catcher called for signal %d\n", sig );
    sigcount++;

    if( sigcount > 10 ) {

        /*
         * Disable the real time interval timer
         */

        getitimer( which, &value );

        value.it_value.tv_sec = 0;
        value.it_value.tv_usec = 0;

        setitimer( which, &value, NULL );
    }
}

int main( int argc, char *argv[] ) {

    int result = 0;

    struct itimerval value, ovalue, pvalue;
    int which = ITIMER_REAL;

    struct sigaction sact;
    volatile double count;
    time_t t;

    sigemptyset( &sact.sa_mask );
    sact.sa_flags = 0;
    sact.sa_handler = catcher;
    sigaction( SIGALRM, &sact, NULL );

    getitimer( which, &pvalue );

    /*
     * Set a real time interval timer to repeat every 200 milliseconds
     */

    value.it_interval.tv_sec = 0;        /* Zero seconds */
    value.it_interval.tv_usec = 200000;  /* Two hundred milliseconds */
    value.it_value.tv_sec = 0;           /* Zero seconds */
    value.it_value.tv_usec = 500000;     /* Five hundred milliseconds */

    result = setitimer( which, &value, &ovalue );

    /*
     * The interval timer value returned by setitimer() should be
     * identical to the timer value returned by getitimer().
     */

    if( ovalue.it_interval.tv_sec != pvalue.it_interval.tv_sec  ||
        ovalue.it_interval.tv_usec != pvalue.it_interval.tv_usec ||
        ovalue.it_value.tv_sec != pvalue.it_value.tv_sec ||
        ovalue.it_value.tv_usec != pvalue.it_value.tv_usec ) {
        printf( "Real time interval timer mismatch\n" );
        result = -1;
    }


    time( &t );
    printf( "Before loop, time is %s", ctime(&t) );

    for( count=0; ((count<LOOP_LIMIT) && (sigcount<10)); count++ );

    time( &t );
    printf( "After loop, time is %s\n", ctime(&t) );


    if( sigcount == 0 )
        printf( "The signal catcher never gained control\n" );
    else
        printf( "The signal catcher gained control\n" );

    printf( "The value of count is %.0f\n", count );

    return( result );
}

