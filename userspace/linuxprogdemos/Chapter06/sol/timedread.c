#include <stdio.h>
#include <signal.h>

/*
 * Program to demonstrate use of the alarm timer to implement
 * timeouts on reads from slow devices.
 * User has 5 seconds to enter their name, or else they will
 * be told off for being so slow ;-)
 */
main ( int argc, char *argv[] )
{
  char buf[128], *bp=buf;
  int nread;
  struct sigaction action;
  void read_timeout();
  sigset_t handler_mask;

  /*
   * Set up and install handler for Alarm signal.
   */
  action.sa_handler = read_timeout;
  sigemptyset(&handler_mask);
  action.sa_mask = handler_mask;
  action.sa_flags = 0;
  if ( sigaction(SIGALRM, &action, NULL) < 0 ) {
    perror("sigaction");
    exit(1);
  }
  
  /*
   * As the user for the input, then set the timer.
   */
  printf("Enter your name now (you have 5 seconds): "); fflush(stdout);
  memset(bp, '\0', 128);

  /*
   * Give the user 5 seconds. If they haven't entered anything by then,
   * then SIGALRM will be sent and we will be in the handler.
   */
  alarm(5);
  nread = read(0, bp, 128);

  /*
   * If we get here, then we have had some input from the user. So cancel
   * the alarm.
   */
  alarm(0);

  /*
   * Print out what the user typed (remove the newline first)
   */
  bp[strlen(bp)-1] = '\0';
  printf("You entered <%s> within the time limit!\n", bp);

  exit(0);
}


/*
 * Signal handler function called when the alarm goes off.
 * This indicates that the user hasn't entered their name quickly enough...
 */
void read_timeout(int signo)
{
  char *msg = "\nSorry! you  didn't type in time...\n";

  write(1, msg, strlen(msg));

  exit(1);
}
  
