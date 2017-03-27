#include <signal.h>
#include <stdio.h>

/*
 * Program to demonstrate catching various signals
 * We sit in a tight loop, eating CPU time ;-) and responding
 * to various signals.
 */

main ( int argc, char *argv[] )
{
  void handler();
  struct sigaction action;
  sigset_t mask;

  /* 
   * Set up the sigaction structure to specify what happens
   * for the signals.
   */
  sigemptyset(&mask);
  action.sa_mask = mask;         /* No other signals to be blocked in handler */
  action.sa_handler = handler;   /* All signals handled in the same function */
  action.sa_flags = 0;           /* No fancy stuff... */

  /*
   * The program is interested in 4 signals, SIGINT and SIGQUIT can be
   * sent from the keyboard, the others need to be sent using kill
   */
  if ( sigaction(SIGINT, &action, NULL) < 0 ) {
    perror("sigaction(INT)");
    exit(1);
  }
  if ( sigaction(SIGQUIT, &action, NULL) < 0 ) {
    perror("sigaction(QUIT)");
    exit(1);
  }
  if ( sigaction(SIGUSR1, &action, NULL) < 0 ) {
    perror("sigaction(USR1)");
    exit(1);
  }
  if ( sigaction(SIGTERM, &action, NULL) < 0 ) {
    perror("sigaction(TERM)");
    exit(1);
  }

  /*
   * To make it easier to send us signals using kill, show our PID 
   */
  printf("If you want to send me a signal, my PID is %d\n", getpid());

  /*
   * That's all, now just eat some CPU...
   */
  while ( 1 ) ;
}

/*
 * The handler function.
 * Because it is passed the signal number, we can use it to deal with
 * multiple signals.
 */
void handler ( int signo ) 
{
    /*
     * Show which signal has been received. I know we shouldn't use printf()
     * here, but it does make life easier for this simple exercise and we will
     * more than likely get away with it...
     */
    printf("Signal no %d received\n", signo);

    /* 
     * The handler ensures that the program stops only on SIGTERM.
     * All other signals will be caught and the process continue,
     * even though SIGINT and SIGQUIT will normally cause termination.
     */
    if ( signo == SIGTERM ) {
	printf("Exiting...\n");
	exit(1);
    }
}
  
