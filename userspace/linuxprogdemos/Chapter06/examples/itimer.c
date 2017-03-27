/*
   itimer.c  Chapter 6

*/

#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler ( int sig ) ;

char *ticktock[] = { "tick..", "tock.." };
struct itimerval itval = { {1, 0}, {2, 0} };

main ( void )
{
  struct sigaction sigact;
  sigset_t emptymask;

  sigemptyset(&emptymask);
  sigact.sa_handler = handler;
  sigact.sa_mask = emptymask;
  sigact.sa_flags = 0;

  if ( sigaction(SIGALRM, &sigact, NULL) < 0 )
  {  /* Error */
     perror ("sigaction failed");
  }

  if ( setitimer(ITIMER_REAL, &itval, NULL ) < 0 )
  {  /* Error */
     perror ("setitimer failed");
  }

  for ( ; ; ) 
    sigsuspend(&emptymask);
}

/* ------------------------------------------------------------ */

void handler ( int sig ) 
{
  static int ix = 0;

  write(1, ticktock[ix], strlen(ticktock[ix]));

  ix = (++ix) % 2;
}
